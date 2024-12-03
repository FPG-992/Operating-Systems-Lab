/*
 * lunix-chrdev.c
 *
 * Implementation of character devices
 * for Lunix:TNG
 *
 * <Filippos & Petros>
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
 * Just a quick [unlocked] check to see if the cached
 * chrdev state needs to be updated from sensor measurements.
 */
/*
 * Declare a prototype so we can define the "unused" attribute and keep
 * the compiler happy. This function is not yet used, because this helpcode
 * is a stub.
 */
static int __attribute__((unused)) lunix_chrdev_state_needs_refresh(struct lunix_chrdev_state_struct *);

static int lunix_chrdev_state_needs_refresh(struct lunix_chrdev_state_struct *state)
{
	struct lunix_sensor_struct *sensor = state->sensor;
	
	WARN_ON(!sensor);

	/*check if sensors data is newer than cached*/	
	if (sensor->msr_data[state->type]->last_update > state->buf_timestamp)
		return 1; //needs refresh
	else
		return 0; //does not need refresh
	
}

/*
 * Updates the cached state of a character device
 * based on sensor data. Must be called with the
 * character device state lock held.
 */
static int lunix_chrdev_state_update(struct lunix_chrdev_state_struct *state)
{
	struct lunix_sensor_struct *sensor = state->sensor;
	uint32_t raw_data;
	uint32_t timestamp;
	unsigned long flags;
	long converted_data;
	int ret;

	WARN_ON(!sensor); //check if sensor is valid

	/*Acquire spinlock to safely read sensor data*/
	spin_lock_irqsave(&sensor->lock, flags);

	/*check if new data is available*/
	timestamp = sensor->msr_data[state->type]->last_update;
	if (timestamp == state->buf_timestamp) {
		spin_unlock_irqrestore(&sensor->lock, flags);
		return -EAGAIN;
	}
	
	/*Read raw data*/
	raw_data = sensor->msr_data[state->type]->values[0];

	/*Release spinlock*/
	spin_unlock_irqrestore(&sensor->lock, flags);

	/*Convert raw data to meaningful data*/
	switch(state->type) {
		case BATT:
			converted_data = lookup_voltage[raw_data];
			break;
		case TEMP:
			converted_data = lookup_temperature[raw_data];
			break;
		case LIGHT:
			converted_data = lookup_light[raw_data];
			break;
		default:
			return -EINVAL;
	}

	/*now we need to format the data to the buffer*/
	ret = snprintf(state->buf_data, LUNIX_CHRDEV_BUFSZ, "%ld.%03ld\n", converted_data / 1000, abs(converted_data % 1000));

	/*update the cached timestamp and buffer limit*/
	state->buf_timestamp = timestamp;
	state->buf_lim = ret;


	return 0;
}

/*************************************
 * Implementation of file operations
 * for the Lunix character device
 *************************************/

static int lunix_chrdev_open(struct inode *inode, struct file *filp)
{
    int ret = 0;
    unsigned int minor = iminor(inode);
    unsigned int sensor_num = minor >> 3;
    unsigned int type_num = minor & 0x7;
    struct lunix_chrdev_state_struct *state;

    /* Allocate and initialize the driver's state */
    state = kmalloc(sizeof(struct lunix_chrdev_state_struct), GFP_KERNEL);
    if (!state)
        return -ENOMEM;

    /* Initialize the semaphore */
    sema_init(&state->lock, 1);

    /* Associate the driver's state with the file */
    filp->private_data = state;

    /* Ensure sensor_num and type_num are valid */
    if (sensor_num >= lunix_sensor_cnt || type_num >= N_LUNIX_MSR) {
        ret = -ENODEV;
        goto out_free_state;
    }

    /* Set the sensor and measurement type */
    state->sensor = &lunix_sensors[sensor_num];
    state->type = type_num;

    /* Initialize buffer timestamp */
    state->buf_timestamp = 0;

    /* Open file in non-seekable mode */
    ret = nonseekable_open(inode, filp);
    if (ret < 0)
        goto out_free_state;

    debug("leaving open, with ret = %d\n", ret);
    return ret;

out_free_state:
    kfree(state);
    return ret;
}

static int lunix_chrdev_release(struct inode *inode, struct file *filp)
{
	struct lunix_chrdev_state_struct *state = filp->private_data;
	/* free drivers state */
	kfree(state);

	return 0;
}

static long lunix_chrdev_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	return -EINVAL;
}

static ssize_t lunix_chrdev_read(struct file *filp, char __user *usrbuf, size_t cnt, loff_t *f_pos)
{
    ssize_t ret=0;
    struct lunix_sensor_struct *sensor;
    struct lunix_chrdev_state_struct *state;
    size_t available, to_copy;

    state = filp->private_data;
    WARN_ON(!state);

    sensor = state->sensor;
    WARN_ON(!sensor);

    if (down_interruptible(&state->lock))
        return -ERESTARTSYS;

    if (*f_pos == 0) {
        while (lunix_chrdev_state_update(state) == -EAGAIN) {
            /* No new data, go to sleep */
            up(&state->lock); // Release lock before sleeping

            if (filp->f_flags & O_NONBLOCK) {
                ret = -EAGAIN;
                goto out_no_lock;
            }

            /* Wait until new data is available */
            ret = wait_event_interruptible(sensor->wq, lunix_chrdev_state_needs_refresh(state));
            if (ret == -ERESTARTSYS)
                goto out_no_lock;

            /* Reacquire the lock */
            if (down_interruptible(&state->lock)) {
                ret = -ERESTARTSYS;
                goto out_no_lock;
            }
        }
    }

    /* Determine how much data is available to read */
    available = state->buf_lim - *f_pos;
    if (available == 0) {
        /* No data to read, end of file */
        ret = 0;
        goto out;
    }

    /* Number of bytes to copy */
    to_copy = min(available, cnt);

    /* Copy data to user space */
    if (copy_to_user(usrbuf, state->buf_data + *f_pos, to_copy)) {
        ret = -EFAULT;
        goto out;
    }

    /* Update the file position */
    *f_pos += to_copy;
    ret = to_copy;

    /* Reset file position if we have reached the end */
    if (*f_pos >= state->buf_lim)
        *f_pos = 0;

out:
    up(&state->lock);
    return ret;

out_no_lock:
    return ret;
}

static int lunix_chrdev_mmap(struct file *filp, struct vm_area_struct *vma)
{
	return -EINVAL;
}

static struct file_operations lunix_chrdev_fops = 
{
	.owner          = THIS_MODULE,
	.open           = lunix_chrdev_open,
	.release        = lunix_chrdev_release,
	.read           = lunix_chrdev_read,
	.unlocked_ioctl = lunix_chrdev_ioctl,
	.mmap           = lunix_chrdev_mmap
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

    /* Register the character device region */
    ret = register_chrdev_region(dev_no, lunix_minor_cnt, "lunix");
    if (ret < 0) {
        debug("failed to register region, ret = %d\n", ret);
        goto out;
    }

    /* Add the character device */
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

	debug("entering\n");
	dev_no = MKDEV(LUNIX_CHRDEV_MAJOR, 0);
	cdev_del(&lunix_chrdev_cdev);
	unregister_chrdev_region(dev_no, lunix_minor_cnt);
	debug("leaving\n");
}
