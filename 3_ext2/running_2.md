```bash
> make
> insmod ext2-lite.ko
> touch /tmp/ext2‐lite.img
> truncate -s 128M /tmp/ext2‐lite.img
> mkfs.ext2 -b 1024 -L "ext2-lite" -O none -m 0 /tmp/ext2‐lite.img
> mkdir /mnt/ext2lite
> mount -t ext2-lite -o loop /tmp/ext2‐lite.img /mnt/ext2lite
```
