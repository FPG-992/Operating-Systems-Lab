FPG-992: Help me implement chrdev.c

GitHub Copilot: To implement `chrdev.c`, you need to complete the character device driver for the Lunix:TNG sensors. Here are the key steps:

1. **Implement `lunix_chrdev_state_needs_refresh`**

   - Check if the cached sensor data is outdated.
   - Compare the cached timestamp `state->buf_timestamp` with the sensor's `last_update` timestamp.
   - Return `1` if the data needs to be refreshed, `0` otherwise.

2. **Implement `lunix_chrdev_state_update`**

   - Acquire the sensor's spinlock to safely read the raw data.
   - Retrieve the raw measurement value from `sensor->msr_data[state->type]`.
   - Release the spinlock.
   - Convert the raw value to a human-readable format using the lookup tables (`lunix-lookup.h`).
   - Format the converted value into a string stored in `state->buf_data`.
   - Update `state->buf_lim` with the length of the data.
   - Update `state->buf_timestamp` with the new timestamp.

3. **Implement `lunix_chrdev_open`**

   - Extract the sensor number and type from the device's minor number.
   - Allocate and initialize a 

lunix_chrdev_state_struct

.
   - Initialize the semaphore `lock` in the state structure.
   - Assign the correct sensor to `state->sensor`.
   - Store the state in `filp->private_data`.

4. **Implement `lunix_chrdev_release`**

   - Retrieve the state from `filp->private_data`.
   - Free the allocated 

lunix_chrdev_state_struct

.

5. **Implement `lunix_chrdev_read`**

   - Acquire the semaphore `state->lock` to ensure exclusive access.
   - Check if there is new data using `lunix_chrdev_state_needs_refresh`.
     - If data is outdated, call `lunix_chrdev_state_update`.
     - If new data is not yet available, put the process to sleep using `wait_event_interruptible` on `sensor->wq`.
   - Copy the data from `state->buf_data` to the user buffer `usrbuf` using `copy_to_user`.
   - Update the file position `*f_pos` and handle EOF conditions.
   - Release the semaphore before returning.

6. **Implement 

lunix_chrdev_init

 and 

lunix_chrdev_destroy

**

   - In 

lunix_chrdev_init

:
     - Register the character device region for all sensors.
     - Initialize and add the `cdev` structure.
   - In 

lunix_chrdev_destroy

:
     - Delete the `cdev`.
     - Unregister the character device region.

Remember to handle concurrency and synchronization appropriately to prevent data races and ensure thread safety.

FPG-992: which function should we implement first? give me guidance this is good.

GitHub Copilot: 
