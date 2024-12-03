/*
 * lunix-chrdev.c
 *
 * Implementation of character devices
 * for Lunix:TNG
 *
 * < Your name here >
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
static int lunix_chrdev_state_needs_refresh(struct lunix_chrdev_state_struct *state)
{
    struct lunix_sensor_struct *sensor;
    unsigned long flags;
    uint32_t sensor_timestamp;

    WARN_ON(!(sensor = state->sensor));

    /* Acquire the sensor's spinlock */
    spin_lock_irqsave(&sensor->lock, flags);
    /* Read the sensor's timestamp */
    sensor_timestamp = sensor->msr_data[state->type]->last_update;
    /* Release the spinlock */
    spin_unlock_irqrestore(&sensor->lock, flags);

    /* Compare with the cached timestamp */
    if (sensor_timestamp != state->buf_timestamp)
        return 1; /* Needs refresh */
    else
        return 0; /* No refresh needed */
}

/*
 * Updates the cached state of a character device
 * based on sensor data. Must be called with the
 * character device state lock held.
 */
static int lunix_chrdev_state_update(struct lunix_chrdev_state_struct *state)
{
    struct lunix_sensor_struct *sensor;
    struct lunix_msr_data_struct *msr_data;
    unsigned long flags;
    uint32_t raw_data;
    long converted_data;
    uint32_t sensor_timestamp;
    int ret;

    sensor = state->sensor;

    /* Acquire the sensor's spinlock */
    spin_lock_irqsave(&sensor->lock, flags);

    /* Read raw sensor data and timestamp */
    msr_data = sensor->msr_data[state->type];
    raw_data = msr_data->values[0];
    sensor_timestamp = msr_data->last_update;

    /* Release the spinlock */
    spin_unlock_irqrestore(&sensor->lock, flags);

    /* Check if data is fresh */
    if (sensor_timestamp == state->buf_timestamp)
        return -EAGAIN; /* No new data */

    /* Convert raw data using the lookup tables */
    switch (state->type) {
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

    /* Format the converted data as a string and store it in state->buf */
    state->buf_lim = snprintf(state->buf, LUNIX_CHRDEV_BUFSZ, "%ld.%03ld\n",
                              converted_data / 1000, abs(converted_data % 1000));

    /* Update the cached timestamp */
    state->buf_timestamp = sensor_timestamp;

    return 0;
}

/*************************************
 * Implementation of file operations
 * for the Lunix character device
 *************************************/

static int lunix_chrdev_open(struct inode *inode, struct file *filp)
{
    int ret;
    unsigned int minor;
    struct lunix_chrdev_state_struct *state;
    int sensor_num, type;

    debug("entering\n");
    ret = -ENODEV;
    if ((ret = nonseekable_open(inode, filp)) < 0)
        goto out;

    /*
     * Associate this open file with the relevant sensor based on
     * the minor number of the device node [/dev/sensor<NO>-<TYPE>]
     */
    minor = iminor(inode);

    sensor_num = minor >> 3;       /* Each sensor has 8 minor numbers */
    type = minor & 0x7;            /* The lower 3 bits represent the type */

    if (sensor_num >= lunix_sensor_cnt || type >= N_LUNIX_MSR) {
        ret = -ENODEV;
        goto out;
    }

    /* Allocate a new Lunix character device private state structure */
    state = kmalloc(sizeof(struct lunix_chrdev_state_struct), GFP_KERNEL);
    if (!state) {
        ret = -ENOMEM;
        goto out;
    }

    state->sensor = &lunix_sensors[sensor_num];
    state->type = type;
    sema_init(&state->lock, 1);
    state->buf_lim = 0;
    state->buf_timestamp = 0;

    filp->private_data = state;

    ret = 0;

out:
    debug("leaving, with ret = %d\n", ret);
    return ret;
}

static int lunix_chrdev_release(struct inode *inode, struct file *filp)
{
    kfree(filp->private_data);
    debug("released private data successfully \n")
    return 0;
}

static long lunix_chrdev_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	return -EINVAL;
}

static ssize_t lunix_chrdev_read(struct file *filp, char __user *usrbuf, size_t cnt, loff_t *f_pos)
{
    ssize_t ret = 0;
    struct lunix_sensor_struct *sensor;
    struct lunix_chrdev_state_struct *state;
    int remaining_bytes;
    int bytes_to_copy;

    state = filp->private_data;
    WARN_ON(!state);

    sensor = state->sensor;
    WARN_ON(!sensor);

    /* Lock the state */
    if (down_interruptible(&state->lock))
        return -ERESTARTSYS;

    /* If f_pos == 0, update the cached data */
    if (*f_pos == 0) {
        /* Keep trying to update the state until new data is available */
        while (lunix_chrdev_state_update(state) == -EAGAIN) {
            /* Unlock the semaphore */
            up(&state->lock);

            /* Wait for new data */
            if (wait_event_interruptible(sensor->wq, lunix_chrdev_state_needs_refresh(state))) {
                /* If signal, return */
                return -ERESTARTSYS;
            }

            /* Lock again */
            if (down_interruptible(&state->lock))
                return -ERESTARTSYS;
        }
    }

    /* Check if we're at EOF */
    if (*f_pos >= state->buf_lim) {
        /* EOF */
        ret = 0;
        goto out;
    }

    /* Determine the number of bytes to copy */
    remaining_bytes = state->buf_lim - *f_pos;
    bytes_to_copy = min(remaining_bytes, (int)cnt);

    /* Copy data to user */
    if (copy_to_user(usrbuf, state->buf + *f_pos, bytes_to_copy)) {
        ret = -EFAULT;
        goto out;
    }

    *f_pos += bytes_to_copy;
    ret = bytes_to_copy;

    /* Auto-rewind on EOF */
    if (*f_pos >= state->buf_lim)
        *f_pos = 0;

out:
    up(&state->lock);
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
    /*
     * Register the character device with the kernel, asking for
     * a range of minor numbers (number of sensors * 8 measurements / sensor)
     * beginning with LUNIX_CHRDEV_MAJOR:0
     */
    int ret;
    dev_t dev_no;
    unsigned int lunix_minor_cnt = lunix_sensor_cnt << 3;

    debug("initializing character device\n");
    cdev_init(&lunix_chrdev_cdev, &lunix_chrdev_fops);
    lunix_chrdev_cdev.owner = THIS_MODULE;

    dev_no = MKDEV(LUNIX_CHRDEV_MAJOR, 0);

    /* Register the device numbers */
    ret = register_chrdev_region(dev_no, lunix_minor_cnt, "lunix");

    if (ret < 0) {
        debug("failed to register region, ret = %d\n", ret);
        goto out;
    }

    /* Add the character device to the system */
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
