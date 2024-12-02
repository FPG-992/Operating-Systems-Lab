### **1. Implement the Character Device Driver (`lunix-chrdev.c` and `lunix-chrdev.h`)**

Your main task is to **implement the character device driver**, which corresponds to part **(g)** of the assignment. Specifically:

- **Complete the implementation in `lunix-chrdev.c` and `lunix-chrdev.h`.**

  - These files contain skeleton code with placeholders and comments indicating where you need to add your code.
  - You'll need to implement functions such as:
    - `lunix_chrdev_open()`: Handle opening the device.
    - `lunix_chrdev_release()`: Handle closing the device.
    - `lunix_chrdev_read()`: Provide sensor data to user space.
    - (Optionally) `lunix_chrdev_ioctl()`: Handle custom IOCTL commands.
    - (Optionally) `lunix_chrdev_mmap()`: Support memory-mapped I/O.

- **Implement File Operations:**

  - Define the `file_operations` structure (`lunix_chrdev_fops`) with pointers to your implemented functions.
  - Ensure your driver supports the standard file operations: `open`, `read`, `release`, and others as needed.

- **Interact with Sensor Data Structures:**

  - Use the provided `lunix_sensor_struct` and related data structures to access sensor measurements.
  - Implement data formatting and conversion as necessary before sending data to user space.

### **2. Understand and Utilize the Provided Code**

- The other files you've provided (`lunix-main.c`, `lunix-sensors.c`, `lunix-protocol.c`, etc.) implement parts **(b)** to **(f)** of the assignment.
- **These files are responsible for:**

  - Collecting data from the base station.
  - Processing the Lunix:TNG protocol.
  - Updating sensor buffers with new measurements.

- **Your driver should:**

  - Retrieve data from these sensor buffers.
  - Ensure synchronization when accessing shared data structures.

### **3. Set Up Your Development Environment**

- **Virtual Machine Setup:**

  - Use the provided virtual machine (utopia VM) for development.
  - **Note:** The VM has Linux kernel version **5.10**, but your module targets kernel version **6.11**.
  - **Action:** Download, compile, and install Linux kernel **6.11** inside the VM.

- **Kernel Module Development:**

  - Develop your driver as a **kernel module** that can be loaded into the kernel.

### **4. Implement Synchronization Mechanisms**

- **Concurrency Handling:**

  - Multiple processes may access the device simultaneously.
  - Use synchronization primitives to protect shared resources:
    - **Semaphores** (`struct semaphore`): For process context synchronization.
    - **Spinlocks** (`spinlock_t`): For interrupt context or when sleeping is not allowed.

- **Implement Sleeping and Waking Mechanisms:**

  - When no new data is available, processes should **sleep**.
  - Use **wait queues** (`wait_queue_head_t`) to block processes until data is available.
  - Wake up sleeping processes when new sensor data arrives.

### **5. Format and Provide Sensor Data**

- **Data Formatting:**

  - Sensor data may need to be converted from raw measurements to human-readable formats.
  - Use the provided lookup tables in `lunix-lookup.h` for converting raw data.

- **Implement `lunix_chrdev_state_update()`:**

  - This function should check if new data is available and update the driver's internal state.
  - Ensure proper locking when accessing shared data.

### **6. Test Your Driver (Part (h))**

- **Create Device Files:**

  - Use the provided script (e.g., `mk-lunix-devs.sh`) to create the necessary device files in `/dev/`.

- **Testing with User Programs:**

  - Write user-space programs or use command-line tools (`cat`, `dd`) to read from your device files.
  - Verify that your driver correctly provides sensor data.

- **Concurrent Access Testing:**

  - Test reading from the same device file from multiple processes simultaneously.
  - Ensure that your synchronization mechanisms are working correctly.

### **7. Optional Enhancements**

- **Support IOCTL Commands:**

  - Implement custom `ioctl` operations if you wish to provide additional functionality.

- **Memory-Mapped I/O (`mmap`):**

  - Implement the `lunix_chrdev_mmap()` function to allow user-space programs to map sensor data directly into their address space.

### **8. Reference Materials**

- **"Linux Device Drivers, 3rd Edition" (LDD3):**

  - **Chapter 3 (Char Drivers):** Understand the basics of character device drivers.
  - **Chapter 5 (Concurrency and Race Conditions):** Learn about synchronization in the kernel.
  - **Chapter 6 (Advanced Char Driver Operations):** Explore advanced topics like `ioctl` and `poll`.
  - **Chapter 15 (Memory Mapping and DMA):** If implementing `mmap`, this chapter is essential.

- **`scull` Driver Example:**

  - The `scull` (Simple Character Utility for Loading Localities) driver in LDD3 is a good reference.
  - It provides examples of device initialization, file operations, synchronization, and more.

### **9. Practical Tips**

- **Follow the Comments in the Skeleton Code:**

  - Look for `/* ? */` and `/* FIXME */` comments indicating where you need to add code.

- **Use Kernel Logging for Debugging:**

  - Use `printk()` or `pr_debug()` to log messages.
  - Monitor kernel logs with `dmesg` or `tail -f /var/log/kern.log`.

- **Handle Errors Gracefully:**

  - Check return values of kernel functions.
  - Use appropriate error codes (e.g., `-EAGAIN`, `-ENOMEM`, `-EINVAL`).

- **Pay Attention to Context:**

  - Be aware of the difference between process context and interrupt context.
  - Avoid sleeping in interrupt context.

- **Ensure Correct Use of Pointers and Memory Management:**

  - Allocate and free memory appropriately.
  - Avoid memory leaks and dangling pointers.

### **10. Steps to Proceed**

1. **Set Up the Environment:**

   - Compile and install Linux kernel 6.11 in the VM.
   - Reboot the VM into the new kernel.

2. **Study the Provided Code:**

   - Read through all the provided files to understand how they interact.

3. **Implement the Driver:**

   - Start with `lunix-chrdev.c` and `lunix-chrdev.h`.
   - Implement the `open`, `read`, and `release` functions.
   - Use synchronization primitives where necessary.

4. **Compile and Load the Module:**

   - Use the provided `Makefile` to compile your module.
   - Load the module using `insmod`.
   - Check for errors in the kernel log.

5. **Create Device Nodes:**

   - Use `mknod` or a script to create device files in `/dev/`.

6. **Test the Driver:**

   - Use simple commands like `cat /dev/lunix0-temp` to read data.
   - Write test programs if needed.

7. **Debug and Iterate:**

   - If you encounter issues, use kernel logs and debugging techniques to identify and fix them.

### **If You Need Help**

- **Ask Specific Questions:**

  - If you're stuck on a particular function or concept, feel free to ask for clarification.

- **Use Online Resources:**

  - The Linux Kernel documentation and community forums can be valuable resources.

### **Summary**

- **Implement the character device driver by completing `lunix-chrdev.c` and `lunix-chrdev.h`.**
- **Ensure proper synchronization and handling of sensor data.**
- **Test your driver thoroughly to confirm it meets the assignment requirements.**