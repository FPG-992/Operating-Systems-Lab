/*
 * lunix-chrdev.c
 *
 * Implementation of character devices
 * for Lunix:TNG
 *
 * Konstantinos Vosinas
 * Konstantinos Andriopoulos
 *
 */

#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/cdev.h>
#include <linux/poll.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/ioctl.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mmzone.h>
#include <linux/vmalloc.h>
#include <linux/spinlock.h>

#include "lunix.h"
#include "lunix-chrdev.h"
#include "lunix-lookup.h"

/*
 * Global data
 */
struct cdev lunix_chrdev_cdev;

/*
 * Checks if the cached chrdev state needs to be updated from sensor measurements.
 */
static int lunix_chrdev_state_needs_refresh(struct lunix_chrdev_state_struct *state)
{
	struct lunix_sensor_struct *sensor;

	WARN_ON(!(sensor = state->sensor));

	/* Check if new data is available */
	if (state->buf_timestamp != sensor->msr_data[state->type]->last_update)
		return 1;

	return 0;
}

/*
 * Updates the cached state of a character device based on sensor data.
 * Must be called with the character device state lock held.
 */
static int lunix_chrdev_state_update(struct lunix_chrdev_state_struct *state)
{
	struct lunix_sensor_struct *sensor;
	uint32_t raw_data, last_update;
	long converted_value;
	int ret = 0;

	sensor = state->sensor;
	WARN_ON(!sensor);

	/* Check if update is needed */
	if (!lunix_chrdev_state_needs_refresh(state))
		return -EAGAIN;

	/* Acquire the sensor's spinlock */
	spin_lock_irq(&sensor->lock);

	/* Read raw sensor data and timestamp */
	raw_data = sensor->msr_data[state->type]->values[0];
	last_update = sensor->msr_data[state->type]->last_update;

	/* Release the spinlock */
	spin_unlock_irq(&sensor->lock);

	/* Update the cached timestamp */
	state->buf_timestamp = last_update;

	/* Convert raw data using the lookup tables */
	switch (state->type) {
	case BATT:
		converted_value = lookup_voltage[raw_data];
		break;
	case TEMP:
		converted_value = lookup_temperature[raw_data];
		break;
	case LIGHT:
		converted_value = lookup_light[raw_data];
		break;
	default:
		return -EINVAL;
	}

	/* Format the converted data and store it in state->buf_data */
	state->buf_lim = snprintf(state->buf_data, LUNIX_CHRDEV_BUFSZ, "%ld.%03ld\n",
	                          converted_value / 1000, abs(converted_value % 1000));

	return ret;
}

/*************************************
 * Implementation of file operations
 * for the Lunix character device
 *************************************/

static int lunix_chrdev_open(struct inode *inode, struct file *filp)
{
	int ret = 0;
	unsigned int minor_num, type, sensor_num;
	struct lunix_chrdev_state_struct *state;

	debug("entering open\n");

	if ((ret = nonseekable_open(inode, filp)) < 0)
		goto out;

	minor_num = iminor(inode);
	type = minor_num % 8;      /* Measurement type */
	sensor_num = minor_num / 8; /* Sensor number */

	if (type >= N_LUNIX_MSR) {
		ret = -EINVAL;
		goto out;
	}

	state = kmalloc(sizeof(*state), GFP_KERNEL);
	if (!state) {
		ret = -ENOMEM;
		debug("couldn't allocate memory\n");
		goto out;
	}

	state->type = type;
	state->sensor = &lunix_sensors[sensor_num];
	state->buf_lim = 0;
	state->buf_timestamp = 0;
	sema_init(&state->lock, 1);

	filp->private_data = state;

out:
	debug("leaving open, with ret = %d\n", ret);
	return ret;
}

static int lunix_chrdev_release(struct inode *inode, struct file *filp)
{
	kfree(filp->private_data);
	debug("released private data successfully\n");
	return 0;
}

static long lunix_chrdev_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	/* No ioctl commands are supported */
	return -EINVAL;
}

static ssize_t lunix_chrdev_read(struct file *filp, char __user *usrbuf, size_t cnt, loff_t *f_pos)
{
	ssize_t ret = 0;
	struct lunix_chrdev_state_struct *state;
	struct lunix_sensor_struct *sensor;
	ssize_t available_bytes;

	state = filp->private_data;
	WARN_ON(!state);

	sensor = state->sensor;
	WARN_ON(!sensor);

	if (down_interruptible(&state->lock))
		return -ERESTARTSYS;

	/* Update state if necessary */
	if (*f_pos == 0) {
		while (lunix_chrdev_state_update(state) == -EAGAIN) {
			up(&state->lock);

			/* Wait until new data is available */
			if (wait_event_interruptible(sensor->wq, lunix_chrdev_state_needs_refresh(state)))
				return -ERESTARTSYS;

			if (down_interruptible(&state->lock))
				return -ERESTARTSYS;
		}
	}

	/* Determine the number of bytes to copy */
	available_bytes = state->buf_lim - *f_pos;
	if (available_bytes < 0)
		available_bytes = 0;

	if (cnt > available_bytes)
		cnt = available_bytes;

	if (cnt == 0) {
		/* EOF */
		ret = 0;
		goto out;
	}

	/* Copy data to user space */
	if (copy_to_user(usrbuf, state->buf_data + *f_pos, cnt)) {
		ret = -EFAULT;
		goto out;
	}

	*f_pos += cnt;
	ret = cnt;

	/* Auto-rewind on EOF */
	if (*f_pos >= state->buf_lim)
		*f_pos = 0;

out:
	up(&state->lock);
	return ret;
}

static int lunix_chrdev_mmap(struct file *filp, struct vm_area_struct *vma)
{
	/* Memory mapping is not supported */
	return -EINVAL;
}

static const struct file_operations lunix_chrdev_fops = {
	.owner          = THIS_MODULE,
	.open           = lunix_chrdev_open,
	.release        = lunix_chrdev_release,
	.read           = lunix_chrdev_read,
	.unlocked_ioctl = lunix_chrdev_ioctl,
	.mmap           = lunix_chrdev_mmap,
};

int lunix_chrdev_init(void)
{
	int ret;
	dev_t dev_no;
	unsigned int lunix_minor_cnt = lunix_sensor_cnt << 3;

	debug("initializing character device\n");
	cdev_init(&lunix_chrdev_cdev, &lunix_chrdev_fops);
	lunix_chrdev_cdev.owner = THIS_MODULE;

	dev_no = MKDEV(LUNIX_CHRDEV_MAJOR, 0);

	ret = register_chrdev_region(dev_no, lunix_minor_cnt, "lunix");
	if (ret < 0) {
		debug("failed to register region, ret = %d\n", ret);
		goto out;
	}

	ret = cdev_add(&lunix_chrdev_cdev, dev_no, lunix_minor_cnt);
	if (ret < 0) {
		debug("failed to add character device\n");
		goto out_with_chrdev_region;
	}

	debug("completed successfully\n");
	return 0;

out_with_chrdev_region:
	unregister_chrdev_region(dev_no, lunix_minor_cnt);
out:
	return ret;
}

void lunix_chrdev_destroy(void)
{
	dev_t dev_no;
	unsigned int lunix_minor_cnt = lunix_sensor_cnt << 3;

	debug("entering destroy\n");
	dev_no = MKDEV(LUNIX_CHRDEV_MAJOR, 0);
	cdev_del(&lunix_chrdev_cdev);
	unregister_chrdev_region(dev_no, lunix_minor_cnt);
	debug("leaving destroy\n");
}
