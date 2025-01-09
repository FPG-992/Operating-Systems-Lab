```bash

the succesful run

Linux utopia 6.11.0 #1 SMP PREEMPT_DYNAMIC Fri Jan  3 21:03:29 EET 2025 x86_64

The programs included with the Debian GNU/Linux system are free software;
the exact distribution terms for each program are described in the
individual files in /usr/share/doc/*/copyright.

Debian GNU/Linux comes with ABSOLUTELY NO WARRANTY, to the extent
permitted by applicable law.
Last login: Thu Jan  9 12:00:59 2025 from 10.0.2.2
root@utopia:~# ls
root@utopia:~# lsmod
Module                  Size  Used by
root@utopia:~# ls
root@utopia:~# cd /home/user/shared
root@utopia:/home/user/shared# ls
ext2-lite  linux-6.11
root@utopia:/home/user/shared#
root@utopia:/home/user/shared# cd ext2-lite/
root@utopia:/home/user/shared/ext2-lite# ls
ext2-lite-helpcode
root@utopia:/home/user/shared/ext2-lite# cd ext2-lite-helpcode/
root@utopia:/home/user/shared/ext2-lite/ext2-lite-helpcode# ls
balloc.c  dir.c  ext2.h        ext2-lite.mod    ext2-lite.mod.o  file.c  ialloc.c  inode.c  Makefile       Module.symvers  namei.o  super.o
balloc.o  dir.o  ext2-lite.ko  ext2-lite.mod.c  ext2-lite.o      file.o  ialloc.o  inode.o  modules.order  namei.c         super.c
root@utopia:/home/user/shared/ext2-lite/ext2-lite-helpcode# ls
balloc.c  dir.c  ext2.h        ext2-lite.mod    ext2-lite.mod.o  file.c  ialloc.c  inode.c  Makefile       Module.symvers  namei.o  super.o
balloc.o  dir.o  ext2-lite.ko  ext2-lite.mod.c  ext2-lite.o      file.o  ialloc.o  inode.o  modules.order  namei.c         super.c
root@utopia:/home/user/shared/ext2-lite/ext2-lite-helpcode# insmod ext2-lite.ko
root@utopia:/home/user/shared/ext2-lite/ext2-lite-helpcode# lsmod
Module                  Size  Used by
ext2_lite              53248  0
root@utopia:/home/user/shared/ext2-lite/ext2-lite-helpcode# touch /tmp/ext2-lite.img
truncate -s 128M /tmp/ext2-lite.img
mkfs.ext2 -b 1024 -L "ext2-lite" -O none -m 0 /tmp/ext2-lite.img
mount -t ext2-lite -o loop /tmp/ext2-lite.img /mnt
mke2fs 1.47.0 (5-Feb-2023)
Discarding device blocks: done
Creating filesystem with 131072 1k blocks and 32768 inodes
Filesystem UUID: d1d34a72-2abf-4112-8417-f060d791b653
Superblock backups stored on blocks:
        8193, 16385, 24577, 32769, 40961, 49153, 57345, 65537, 73729, 81921,
        90113, 98305, 106497, 114689, 122881

Allocating group tables: done
Writing inode tables: done
Writing superblocks and filesystem accounting information: done

root@utopia:/home/user/shared/ext2-lite/ext2-lite-helpcode# cd /mnt/
root@utopia:/mnt# ls
lost+found
root@utopia:/mnt# ls
lost+found
root@utopia:/mnt# touch hello.txt
root@utopia:/mnt# ls
hello.txt  lost+found
root@utopia:/mnt# cat hello.txt
root@utopia:/mnt# cat lost+found/
cat: lost+found/: Is a directory
root@utopia:/mnt# ls
hello.txt  lost+found
root@utopia:/mnt# cd lost+found/
root@utopia:/mnt/lost+found# ls
root@utopia:/mnt/lost+found# cat
ls
ls

```
