### Compile the module and add it in the Kernel
```bash
> make
> insmod ext2-lite.ko
```

### Verify that the filesystem has been registered
After `insmod` write the following command and on the buttom you should see `ext2-lite`
```bash
> cat /proc/filesystems
```

### Prepare a block device or loop device
Create a file of a fixed size to act as a `disk`:
```bash
> dd if=/dev/zero of=/tmp/ext2lite.img bs=1M count=16
```

Associate the file with a loop device:
```bash
> losetup /dev/loop0 /tmp/ext2lite.img
```

Format that loop device
```bash
> mkfs.ext2 /dev/loop0
```

### Mount using your new filesystem type
```bash
> mkdir /mnt/ext2lite
> mount /dev/loop0 /mnt/ext2lite
```

After that you can do everything you wish inside `/mnt/ext2lite` (read, wrtie, create files, direcoties, etc.)!

Don't forget to unmount the filesystem and remove the module from the kernel if you don't want to use it:
```bash
> umount /mnt/ext2lite
> rmmod ext2-lite.ko
```
