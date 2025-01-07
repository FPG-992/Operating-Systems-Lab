https://wiki.osdev.org/Ext2
https://www.nongnu.org/ext2-doc/ext2.html

Starting
Byte

Ending
Byte

Size
in Bytes

Field Description
0	3	4	Total number of inodes in file system
4	7	4	Total number of blocks in file system
8	11	4	Number of blocks reserved for superuser (see offset 80)
12	15	4	Total number of unallocated blocks
16	19	4	Total number of unallocated inodes
20	23	4	Block number of the block containing the superblock (also the starting block number, NOT always zero.)
24	27	4	log2 (block size) - 10. (In other words, the number to shift 1,024 to the left by to obtain the block size)
28	31	4	log2 (fragment size) - 10. (In other words, the number to shift 1,024 to the left by to obtain the fragment size)
32	35	4	Number of blocks in each block group
36	39	4	Number of fragments in each block group
40	43	4	Number of inodes in each block group
44	47	4	Last mount time (in POSIX time)
48	51	4	Last written time (in POSIX time)
52	53	2	Number of times the volume has been mounted since its last consistency check (fsck)
54	55	2	Number of mounts allowed before a consistency check (fsck) must be done
56	57	2	Ext2 signature (0xef53), used to help confirm the presence of Ext2 on a volume
58	59	2	File system state (see below)
60	61	2	What to do when an error is detected (see below)
62	63	2	Minor portion of version (combine with Major portion below to construct full version field)
64	67	4	POSIX time of last consistency check (fsck)
68	71	4	Interval (in POSIX time) between forced consistency checks (fsck)
72	75	4	Operating system ID from which the filesystem on this volume was created (see below)
76	79	4	Major portion of version (combine with Minor portion above to construct full version field)
80	81	2	User ID that can use reserved blocks
82	83	2	Group ID that can use reserved blocks


**Image 1**
Κάνουμε mount το disk στο /mnt/fdisk1
mkdir -p /mnt/fdisk1
mount /dev/vdb /mnt/fdisk1

1. Τροποποιήστε κατάλληλα το αρχείο utopia.sh ώστε να προσθέσετε στην
εικονική μηχανή utopia έναν επιπλέον δίσκο για την εικόνα fsdisk1.img.
Ποια είναι η προσθήκη που κάνατε; Ποια συσκευή στο utopia είναι αυτή που
μόλις προσθέσατε;
    
exec $QEMU -enable-kvm -M pc -m $UTOPIA_MEMORY_MB \
    -smp 16 -drive file=$QCOW2_PRIVATE_FILE,if=virtio \
    -drive file=./fsdisk1-7c2b425a6.img,format=raw,if=virtio \ #Προσθέσαμε αυτές τις τρεις γραμμές για να προσθέσουμε έναν επιπλέον δίσκο για κάθε εικόνα
    -drive file=./fsdisk2-a0173283d.img,format=raw,if=virtio \
    -drive file=./fsdisk3-982902777.img,format=raw,if=virtio \
    -net nic -net user,hostfwd=tcp:$UTOPIA_SSH_INTERFACE:$UTOPIA_SSH_PORT-:22 \
    -vnc 127.0.0.1:0 \
    -nographic -monitor /dev/null  \
    -chardev socket,id=sensors0,host=lunix.cslab.ece.ntua.gr,port=49152,ipv4=on \
    -device isa-serial,chardev=sensors0,id=serial1,index=1 \
    -fsdev local,id=fsdev0,path="$SHARED_FS_DIR",security_model=none \
    -device virtio-9p-pci,fsdev=fsdev0,mount_tag=shared \
    $*

#Προσθέσαμε αυτές τις τρεις γραμμές για να προσθέσουμε έναν επιπλέον δίσκο για κάθε εικόνα
Virtio: An I/O virtualization framework for Linux
-drive file=./fsdisk1-7c2b425a6.img,format=raw,if=virtio \ 
-drive file=./fsdisk2-a0173283d.img,format=raw,if=virtio \
-drive file=./fsdisk3-982902777.img,format=raw,if=virtio \

Χρησιμοποιόντας blkid - command-line utility to locate/print block device attributes
βλέπουμε πως :
/dev/vdd: LABEL="fsdisk3.img" UUID="19032143-52ce-4917-8ec5-991c89ee421b" BLOCK_SIZE="1024" TYPE="ext2"
/dev/vdb: LABEL="fsdisk1.img" UUID="c63028e5-711b-410d-a263-e7ca2b15a8d3" BLOCK_SIZE="1024" TYPE="ext2"
/dev/vdc: LABEL="fsdisk2.img" UUID="d1266ad1-dae1-4275-8136-a29a4dfc9d1f" BLOCK_SIZE="1024" TYPE="ext2"
/dev/vda1: UUID="c3f61444-3056-4fc3-a339-9c8aea4314f7" BLOCK_SIZE="4096" TYPE="ext4" PARTUUID="8cc52ab1-01"

το disk1 αντιστοιχεί στο vdb

### με hexedit

root@utopia:~# hexdump -s 1024 -n 1024 -C /dev/vdb
00000400  18 32 00 00 00 c8 00 00  00 0a 00 00 90 c1 00 00  |.2..............|
00000410  0a 32 00 00 01 00 00 00  00 00 00 00 00 00 00 00  |.2..............|
00000420  00 20 00 00 00 20 00 00  28 07 00 00 a6 66 78 67  |. ... ..(....fxg|
00000430  2f 6a 78 67 02 00 ff ff  53 ef 01 00 01 00 00 00  |/jxg....S.......|
00000440  e4 7a 78 65 00 00 00 00  00 00 00 00 01 00 00 00  |.zxe............|
00000450  00 00 00 00 0b 00 00 00  80 00 00 00 00 00 00 00  |................|
00000460  00 00 00 00 00 00 00 00  c6 30 28 e5 71 1b 41 0d  |.........0(.q.A.|
00000470  a2 63 e7 ca 2b 15 a8 d3  66 73 64 69 73 6b 31 2e  |.c..+...fsdisk1.|
00000480  69 6d 67 00 00 00 00 00  2f 63 73 6c 61 62 2d 62  |img...../cslab-b|
00000490  75 6e 6b 65 72 00 00 00  00 00 00 00 00 00 00 00  |unker...........|
000004a0  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
000004e0  00 00 00 00 00 00 00 00  00 00 00 00 a7 f1 37 f1  |..............7.|
000004f0  07 8d 4b b9 9b 0d e1 b1  22 bc 50 38 01 00 00 00  |..K.....".P8....|
00000500  0c 00 00 00 00 00 00 00  e4 7a 78 65 00 00 00 00  |.........zxe....|
00000510  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
00000560  01 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
00000570  00 00 00 00 00 00 00 00  15 00 00 00 00 00 00 00  |................|
00000580  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
00000640  00 00 00 00 00 00 00 00  60 06 00 00 00 00 00 00  |........`.......|
00000650  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
00000800
root@utopia:~#

Διαβάζοντας το superblock βλέπουμε το fdisk1.img που είναι ο δίσκος 1

2. Τι μέγεθος έχει ο δίσκος που προσθέσατε στο utopia;
### με mount/
Εκτελούμε:
fdisk -l /dev/vdb
Disk /dev/vdb: 50 MiB, 52428800 bytes, 102400 sectors
Units: sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes

Άρα το Disk είναι 50MiB, ισοδύναμα 52428800 bytes | MiB is a base 2 unit, which contains 1024*1024 bytes

### με hexedit
hexdump /dev/vdb
....
3001000 0000 0000 0000 0000 0000 0000 0000 0000
*
30010e0 0000 0000 ff00 ffff ffff ffff ffff ffff
30010f0 ffff ffff ffff ffff ffff ffff ffff ffff
*
3001400 0000 0000 0000 0000 0000 0000 0000 0000
*
3200000

Το 3200000 είναι το highest offset value σε δεκαεξαδικό, που σε δεκαδικό είναι 52428800bytes = 50MiB

3. Τι σύστημα αρχείων περιέχει;

### με mount/
The blkid program is the command-line interface to working with libblkid(3) library. It can determine the type of content (e.g. filesystem, swap) a block device holds, and also attributes (tokens, NAME=value pairs) from the content metadata (e.g. LABEL or UUID fields).

root@utopia:~# blkid /dev/vdb
/dev/vdb: LABEL="fsdisk1.img" UUID="c63028e5-711b-410d-a263-e7ca2b15a8d3" BLOCK_SIZE="1024" TYPE="ext2"

βλέπουμε πως είναι ext2

### με hexedit

root@utopia:~# hexdump -s 1024 -n 1024 -C /dev/vdb
00000400  18 32 00 00 00 c8 00 00  00 0a 00 00 90 c1 00 00  |.2..............|
00000410  0a 32 00 00 01 00 00 00  00 00 00 00 00 00 00 00  |.2..............|
00000420  00 20 00 00 00 20 00 00  28 07 00 00 a6 66 78 67  |. ... ..(....fxg|
00000430  2f 6a 78 67 02 00 ff ff  53 ef 01 00 01 00 00 00  |/jxg....S.......|
00000440  e4 7a 78 65 00 00 00 00  00 00 00 00 01 00 00 00  |.zxe............|
00000450  00 00 00 00 0b 00 00 00  80 00 00 00 00 00 00 00  |................|
00000460  00 00 00 00 00 00 00 00  c6 30 28 e5 71 1b 41 0d  |.........0(.q.A.|
00000470  a2 63 e7 ca 2b 15 a8 d3  66 73 64 69 73 6b 31 2e  |.c..+...fsdisk1.|
00000480  69 6d 67 00 00 00 00 00  2f 63 73 6c 61 62 2d 62  |img...../cslab-b|
00000490  75 6e 6b 65 72 00 00 00  00 00 00 00 00 00 00 00  |unker...........|
000004a0  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
000004e0  00 00 00 00 00 00 00 00  00 00 00 00 a7 f1 37 f1  |..............7.|
000004f0  07 8d 4b b9 9b 0d e1 b1  22 bc 50 38 01 00 00 00  |..K.....".P8....|
00000500  0c 00 00 00 00 00 00 00  e4 7a 78 65 00 00 00 00  |.........zxe....|
00000510  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
00000560  01 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
00000570  00 00 00 00 00 00 00 00  15 00 00 00 00 00 00 00  |................|
00000580  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
00000640  00 00 00 00 00 00 00 00  60 06 00 00 00 00 00 00  |........`.......|
00000650  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
00000800

στο 0x38 από το superblock start βλέπουμε το 53 ef που είναι το magic number (little endian του EF53) που ανταποκρίνεται στα ext2/ext3/ext4 

4. Πότε ακριβώς δημιουργήθηκε αυτό το σύστημα αρχείων; Δείξτε τη χρονοσφραγίδα [timestamp].

### με mount/

root@utopia:~# dumpe2fs /dev/vdb
dumpe2fs 1.47.0 (5-Feb-2023)
Filesystem volume name:   fsdisk1.img
Last mounted on:          /cslab-bunker
Filesystem UUID:          c63028e5-711b-410d-a263-e7ca2b15a8d3
Filesystem magic number:  0xEF53
Filesystem revision #:    1 (dynamic)
Filesystem features:      (none)
Filesystem flags:         signed_directory_hash
Default mount options:    user_xattr acl
Filesystem state:         clean
Errors behavior:          Continue
Filesystem OS type:       Linux
Inode count:              12824
Block count:              51200
Reserved block count:     2560
Overhead clusters:        1632
Free blocks:              49552
Free inodes:              12810
First block:              1
Block size:               1024
Fragment size:            1024
Blocks per group:         8192
Fragments per group:      8192
Inodes per group:         1832
Inode blocks per group:   229
Filesystem created:       Tue Dec 12 15:23:16 2023
Last mount time:          Fri Jan  3 22:37:26 2025
Last write time:          Fri Jan  3 22:52:31 2025
Mount count:              2
Maximum mount count:      -1
Last checked:             Tue Dec 12 15:23:16 2023
Check interval:           0 (<none>)
Lifetime writes:          21 kB
Reserved blocks uid:      0 (user root)
Reserved blocks gid:      0 (group root)
First inode:              11
Inode size:               128
Default directory hash:   half_md4
Directory Hash Seed:      a7f137f1-078d-4bb9-9b0d-e1b122bc5038

Βλέπουμε πως δημιουργήθηκε στις Tue Dec 12 15:23:16 2023
### με hexedit

στο 0x444 βλέπουμε σε little endian το unix time (seconds passed after 1970) το οποίο μετατρέπουμε σε BigEndian για να βρούμε το Date =>0x65787AE4 και μετά σε δεκαδικό
=> 1702394596 και γράφουμε στο terminal date -d @1702394596 που ισοδυναμεί με 

root@utopia:~# date -d @1702394596
Tue Dec 12 03:23:16 PM UTC 2023

5. Πότε ακριβώς προσαρτήθηκε τελευταία φορά; Δείξτε τη χρονοσφραγίδα.
### με mount/ 
Last mount time:          
Fri Jan  3 22:37:26 2025

### με hexedit 
Βρίσκεται στα byte 44-47 του hexdump, = 0x677866A6 (από Le) = 1735943846 σε decimal = 

date -d @1735943846
Fri Jan  3 10:37:26 PM UTC 2025

6. Σε ποιο μονοπάτι προσαρτήθηκε τελευταία φορά;
s_last_mounted
64 bytes directory path where the file system was last mounted. While not normally used, it could serve for auto-finding the mountpoint when not indicated on the command line. Again the path should be zero terminated for compatibility reasons. Valid path is constructed from ISO-Latin-1 characters. located in 136-200bytes offset
### με mount/ 
Last mounted on:  /cslab-bunker

### με hexedit
βλέπουμε και με το hexdump πως 
00000400  18 32 00 00 00 c8 00 00  00 0a 00 00 90 c1 00 00  |.2..............|
00000410  0a 32 00 00 01 00 00 00  00 00 00 00 00 00 00 00  |.2..............|
00000420  00 20 00 00 00 20 00 00  28 07 00 00 a6 66 78 67  |. ... ..(....fxg|
00000430  2f 6a 78 67 02 00 ff ff  53 ef 01 00 01 00 00 00  |/jxg....S.......|
00000440  e4 7a 78 65 00 00 00 00  00 00 00 00 01 00 00 00  |.zxe............|
00000450  00 00 00 00 0b 00 00 00  80 00 00 00 00 00 00 00  |................|
00000460  00 00 00 00 00 00 00 00  c6 30 28 e5 71 1b 41 0d  |.........0(.q.A.|
00000470  a2 63 e7 ca 2b 15 a8 d3  66 73 64 69 73 6b 31 2e  |.c..+...fsdisk1.|
00000480  69 6d 67 00 00 00 00 00  2f 63 73 6c 61 62 2d 62  |img...../cslab-b|
00000490  75 6e 6b 65 72 00 00 00  00 00 00 00 00 00 00 00  |unker...........|
000004a0  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|

το /cslab-b|
|unker είναι το s_last_mounted είναι σε μορφή directory, τι μόνη που δίνει το superblock?

7. Πότε ακριβώς τροποποιήθηκε τελευταία φορά; Δείξτε τη χρονοσφραγίδα.
s_wtime
Unix time, as defined by POSIX, of the last write access to the file system.
4	4	s_wtime
offset byte value_name

### με mount/
Last write time:          Fri Jan  3 22:52:31 2025

### hexedit
root@utopia:/home/user/shared# hexdump -s 1024 -n 52 -C /dev/vdb
00000400  18 32 00 00 00 c8 00 00  00 0a 00 00 90 c1 00 00  |.2..............|
00000410  0a 32 00 00 01 00 00 00  00 00 00 00 00 00 00 00  |.2..............|
00000420  00 20 00 00 00 20 00 00  28 07 00 00 a6 66 78 67  |. ... ..(....fxg|
00000430  2f 6a 78 67                                       |/jxg|
00000434

από le 2f 6a 78 67   σε be ->0x67786a2f σε decimal = 1735944751 
root@utopia:/home/user/shared# date -d @1735944751
Fri Jan  3 10:52:31 PM UTC 2025

8. Τι είναι το μπλοκ σε ένα σύστημα αρχείων;
Block είναι η μικρότερη μονάδα αποθήκευσης στον δίσκο, η οποία αποτελεί τον δίσκο. Είναι λογικά blocks συνεχόμενου χώρου και είναι σταθερού μεγέθους.

9. Τι μέγεθος μπλοκ [block size] χρησιμοποιεί αυτό το σύστημα αρχείων;
s_log_block_size
The block size is computed using this 32bit value as the number of bits to shift left the value 1024. This value may only be non-negative.

block size = 1024 << s_log_block_size;

offset size
24	4	s_log_block_size
### με mount/
dumpe2fs /dev/vdb
Block size:               1024 αναφέρεται σε Kib (Kibibytes) = 1024bytes

### hexedit
root@utopia:/home/user/shared# hexdump -s 1024 -n 32 -C /dev/vdb
00000400  18 32 00 00 00 c8 00 00  00 0a 00 00 90 c1 00 00  |.2..............|
00000410  0a 32 00 00 01 00 00 00  00 00 00 00 00 00 00 00  |.2..............|
έχουμε πως s_log_block_size = 0 ==> 1024 * 2^0 = 1024bytes

10. Τι είναι το inode σε ένα σύστημα αρχείων;

Το index node είναι μια δομή που αντιπροσωπεύει κάθε αντικείμενο σε ένα filesystem.
Η δομή του inode περιέχει pointers (δείκτες) στα block του συστήματος αρχείων που περιέχουν τα δεδομένα που υπάρχουν στο αντικείμενο και όλα τα υπόλοιπα μεταδεδομένα του αντικειμένου εκτός από
το όνομά του. (permissions, owner, group, flags, size, number of blocks used, access time, change time,
modification time, deletion time, number of links, fragments, version (for NFS) and extended attributes (EAs) and/or Access Control Lists (ACLs).

11. Τι μέγεθος έχει το inode σε αυτό το σύστημα αρχείων;
s_inode_size
16bit value indicating the size of the inode structure. In revision 0, this value is always 128 (EXT2_GOOD_OLD_INODE_SIZE). 
In revision 1 and later, this value must be a perfect power of 2 and must be smaller or equal to the block size (1<<s_log_block_size).
88	2	s_inode_size
offset bytes name

### με mount/
με dumpe2fs /dev/vdb
Inode size:               128bytes

### hexedit
root@utopia:~# hexdump -s 1024 -n 90 -C /dev/vdb
00000400  18 32 00 00 00 c8 00 00  00 0a 00 00 90 c1 00 00  |.2..............|
00000410  0a 32 00 00 01 00 00 00  00 00 00 00 00 00 00 00  |.2..............|
00000420  00 20 00 00 00 20 00 00  28 07 00 00 a6 66 78 67  |. ... ..(....fxg|
00000430  2f 6a 78 67 02 00 ff ff  53 ef 01 00 01 00 00 00  |/jxg....S.......|
00000440  e4 7a 78 65 00 00 00 00  00 00 00 00 01 00 00 00  |.zxe............|
00000450  00 00 00 00 0b 00 00 00  80 00                    |..........|

βλέπουμε 80 00 σε little endian -> 0x0080 σε big endian -> σε δεκαδικό = 128bytes
12. Πόσα διαθέσιμα μπλοκ και πόσα διαθέσιμα inodes υπάρχουν σε αυτό το σύστημα αρχείων;
s_free_blocks_count
32bit value indicating the total number of free blocks, including the number of reserved blocks (see s_r_blocks_count). This is a sum of all free blocks of all the block groups.

s_free_inodes_count
32bit value indicating the total number of free inodes. This is a sum of all free inodes of all the block groups
12	4	s_free_blocks_count
16	4	s_free_inodes_count
offset bytes value_name
### με mount/
Free blocks:              49552
Free inodes:              12810
### hexedit
root@utopia:~# hexdump -s 1024 -n 20 -C /dev/vdb
00000400  18 32 00 00 00 c8 00 00  00 0a 00 00 90 c1 00 00  |.2..............|
00000410  0a 32 00 00     
                                  |.2..|
le be decimal
90 c1 00 00 = 0x0000c190 = free blocks: 49552 (decimal)
0a 32 00 00  = 0x0000320a = free inodes: 12810 (decimal)

13. Τι είναι το superblock στο σύστημα αρχείων ext2;
Το superblock περιέχει όλες τις πληροφορίες σχετικά με το configuration του συστήματος αρχείων όπως:
total number of inodes και blocks στο filesystem, πόσα είναι διαθέσιμα, πόσα inodes και blocks έχει το κάθε group, πότε έγινε mount
το filesystem, πότε έγινε modify, ποιο version του filesystem είναι και ποιο Operating System το δημιούργησε.
Όλα τα δεδομένα του superblock αποθηκεύονται στον δίσκο σε little endian format ώστε να είναι φορητό μεταξύ συστημάτων.

14. Πού βρίσκεται μέσα στον δίσκο σε ένα σύστημα αρχείων ext2;
Βρίσκεται στο offset 1024 bytes από την αρχή του δίσκου.

15. Για ποιο λόγο έχει νόημα να υπάρχουν εφεδρικά αντίγραφα του superblock
στο σύστημα αρχείων ext2;
Έχει νόημα γιατί σε περίπτωση όπου υπάρξει corrupt στο σημείο του δίσκου όπου είναι αποθηκευμένο το superblock, δεν θα μπορούμε να 
έχουμε πρόσβαση στις πληροφορίες που δίνει, δεν θα μπορούμε να κάνουμε mount και δεν θα μπορούμε να χρησιμοποιήσουμε το filesystem καθώς
δεν θα έχουμε τα arguments για τις συναρτήσεις που θα χρειαστούμε για να τρέξουμε το filesystem (VFS).

16. Σε ποια μπλοκ βρίσκονται αποθηκευμένα εφεδρικά αντίγραφα του superblock
σε αυτό το σύστημα αρχείων;
56	2	s_magic
offset size value_name
s_magic
16bit value identifying the file system as Ext2. The value is currently fixed to EXT2_SUPER_MAGIC of value 0xEF53.

The first version of ext2 (revision 0) stores a copy at the start of every block group, along with backups of the group descriptor block(s).
Because this can consume a considerable amount of space for large filesystems,
later revisions can optionally reduce the number of backup copies by only putting backups in specific groups (this is the sparse superblock feature).
The groups chosen are 0, 1 and powers of 3, 5 and 7.
Revision 1 and higher of the filesystem also store extra fields, such as a volume name,
a unique identification number, the inode size, and space for optional filesystem features to store configuration info.
### με mount/ 
από dumpe2fs /dev/vdb έχουμε πως τα εφεδρικά είναι στα blocks:
8193
16385
24577
32769
40961
49153

Group 0: (Blocks 1-8192)
  Primary superblock at 1, Group descriptors at 2-2
  Block bitmap at 3 (+2)
  Inode bitmap at 4 (+3)
  Inode table at 5-233 (+4)
  7944 free blocks, 1821 free inodes, 2 directories
  Free blocks: 248-1024, 1026-8192
  Free inodes: 12-1832
Group 1: (Blocks 8193-16384)
  Backup superblock at 8193, Group descriptors at 8194-8194
  Block bitmap at 8195 (+2)
  Inode bitmap at 8196 (+3)
  Inode table at 8197-8425 (+4)
  7958 free blocks, 1831 free inodes, 1 directories
  Free blocks: 8427-16384
  Free inodes: 1834-3664
Group 2: (Blocks 16385-24576)
  Backup superblock at 16385, Group descriptors at 16386-16386
  Block bitmap at 16387 (+2)
  Inode bitmap at 16388 (+3)
  Inode table at 16389-16617 (+4)
  7959 free blocks, 1832 free inodes, 0 directories
  Free blocks: 16618-24576
  Free inodes: 3665-5496
Group 3: (Blocks 24577-32768)
  Backup superblock at 24577, Group descriptors at 24578-24578
  Block bitmap at 24579 (+2)
  Inode bitmap at 24580 (+3)
  Inode table at 24581-24809 (+4)
  7959 free blocks, 1832 free inodes, 0 directories
  Free blocks: 24810-32768
  Free inodes: 5497-7328
Group 4: (Blocks 32769-40960)
  Backup superblock at 32769, Group descriptors at 32770-32770
  Block bitmap at 32771 (+2)
  Inode bitmap at 32772 (+3)
  Inode table at 32773-33001 (+4)
  7959 free blocks, 1832 free inodes, 0 directories
  Free blocks: 33002-40960
  Free inodes: 7329-9160
Group 5: (Blocks 40961-49152)
  Backup superblock at 40961, Group descriptors at 40962-40962
  Block bitmap at 40963 (+2)
  Inode bitmap at 40964 (+3)
  Inode table at 40965-41193 (+4)
  7959 free blocks, 1830 free inodes, 1 directories
  Free blocks: 41194-49152
  Free inodes: 9163-10992
Group 6: (Blocks 49153-51199)
  Backup superblock at 49153, Group descriptors at 49154-49154
  Block bitmap at 49155 (+2)
  Inode bitmap at 49156 (+3)
  Inode table at 49157-49385 (+4)
  1814 free blocks, 1832 free inodes, 0 directories
  Free blocks: 49386-51199
  Free inodes: 10993-12824

### hexedit
root@utopia:~# hexdump -C /dev/vdb | grep '53 ef' κοιτάμε που έχουμε 53 ef magic number, σε little endian,
 εκεί είναι τα copy του Superblock
00000430  2f 6a 78 67 02 00 ff ff  53 ef 01 00 01 00 00 00  |/jxg....S.......|
00800430  e5 7a 78 65 01 00 ff ff  53 ef 00 00 01 00 00 00  |.zxe....S.......|
01000430  e5 7a 78 65 01 00 ff ff  53 ef 00 00 01 00 00 00  |.zxe....S.......|
01800430  e5 7a 78 65 01 00 ff ff  53 ef 00 00 01 00 00 00  |.zxe....S.......|
02000430  e5 7a 78 65 01 00 ff ff  53 ef 00 00 01 00 00 00  |.zxe....S.......|
02800430  e5 7a 78 65 01 00 ff ff  53 ef 00 00 01 00 00 00  |.zxe....S.......|
03000430  e5 7a 78 65 01 00 ff ff  53 ef 00 00 01 00 00 00  |.zxe....S.......|
root@utopia:~#

block number = [((offset_of_magic)-56)/block_size]
Το +8 είναι το offset in line 
0x430 = 1072 + 8 = 1080 - 56 = 1024 / 1024 = 1
0x800430 = 8389680 + 8 = 8389688 - 56 = 8389632 / 1024 = 8193
0x1000430 = 16778288 + 8 = 16778296 - 56 = 16778240 / 1024 = 16385
0x1800430 = 25166896 + 8 = 25166904 - 56 = 25166848 / 1024 = 24577
0x2000430 = 33555504 + 8 = 33555512 - 56 = 33555456 / 1024 = 32769
0x2800430 = 41944112 + 8 = 41944120 - 56 = 41944064 / 1024 = 40961
0x3000430 = 50332720 + 8 = 50332728 - 56 = 50332672 / 1024 = 49153

17. Τι είναι ένα block group στο σύστημα αρχείων ext2;
Τα blocks ομαδοποιούνται σε block groups ώστε να μειωθεί ο κατακερματισμός και το head seeking όταν
διαβάζουμε μεγάλα δεδομένα που βρίσκονται σε σειριακή κατανομή.
Πληροφορίες για κάθε blockgroup βρίσκουμε στο descriptor table που βρίσκονται στα block αμέσως μετά τα superblocks.
Ο αλγόριθμος που κάνει allocate τα block προσπαθεί να τα βάλει στο ίδιο blockgroup με το index node που τα περιέχει.

18. Πόσα block groups έχει ένα σύστημα αρχείων ext2 και πώς κατανέμονται;

Από το Superblock, παίρνουμε το μέγεθος κάθε block, τον συνολικό αριθμό των inodes, τον συνολικό αριθμό των blocks, τον αριθμό των blocks ανά block group και τον αριθμό των inodes σε κάθε block group. 
Από αυτές τις πληροφορίες μπορούμε να συμπεράνουμε τον αριθμό των block groups που υπάρχουν με τον εξής τρόπο:

Στρογγυλοποίηση προς τα πάνω του συνολικού αριθμού των blocks διαιρούμενου με τον αριθμό των blocks ανά block group.
Στρογγυλοποίηση προς τα πάνω του συνολικού αριθμού των inodes διαιρούμενου με τον αριθμό των inodes ανά block group.
Και τα δύο (και να τα ελέγξουμε μεταξύ τους).
https://wiki.osdev.org/Ext2#What_is_a_Block_Group?

Ο αλγόριθμος που κάνει allocate τα block προσπαθεί να τα βάλει στο ίδιο blockgroup με το index node που τα περιέχει.

Κατανέμονται στο χώρο του δίσκου σειριακά.

19. Πόσα block groups περιέχει αυτό το σύστημα αρχείων;

### με mount/
Από το dumpe2fs βλέπουμε πως έχουμε 7 block groups με το καθένα να περιέχει τα πρώτα n+1(8192blocks)

### hexedit
θα βρούμε το 
s_blocks_count (offset 4) και το s_blocks_per_group και θα τα διαιρέσουμε (offset 32), rounding up.
4	4	s_blocks_count
32	4	s_blocks_per_group

root@utopia:~# hexdump -s 1024 -n 8 -C /dev/vdb
00000400  18 32 00 00 00 c8 00 00                           |.2......|
00000408
root@utopia:~
00 c8 00 00 le -> 0x0000c800 = 51200 decimal total blocks

root@utopia:~# hexdump -s 1024 -n 36 -C /dev/vdb
00000400  18 32 00 00 00 c8 00 00  00 0a 00 00 90 c1 00 00  |.2..............|
00000410  0a 32 00 00 01 00 00 00  00 00 00 00 00 00 00 00  |.2..............|
00000420  00 20 00 00                                       |. ..|
00000424

00 20 00 00 le -> 0x00002000 = 8192 decimal

51200 / 8192 = 6.25 = 7 block groups 

20. Τι είναι ο block group descriptor στο σύστημα αρχείων ext2;
Ο group descriptor έχει πληροφορίες σχετικά με το group που περιγράφει, μαζί με τον αριθμό του block που ξεκινάει το index node (inode) του.

struct ext2_group_desc
{
	__u32	bg_block_bitmap;	/* Blocks bitmap block */
	__u32	bg_inode_bitmap;	/* Inodes bitmap block */
	__u32	bg_inode_table;		/* Inodes table block */
	__u16	bg_free_blocks_count;	/* Free blocks count */
	__u16	bg_free_inodes_count;	/* Free inodes count */
	__u16	bg_used_dirs_count;	/* Directories count */
	__u16	bg_pad;
	__u32	bg_reserved[3];
};

https://www.science.smith.edu/~nhowe/262/oldlabs/ext2.html

21. Για ποιο λόγο έχει νόημα να υπάρχουν εφεδρικά αντίγραφα των block group
descriptors στο σύστημα αρχείων ext2;

Σε περίπτωση που γίνει corrupt το block που περιέχει τα block group descriptors ή το block group descriptor array, για να μπορέσουμε να βρούμε τις πληροφορίες για τα groups.

22. Σε ποια μπλοκ βρίσκονται αποθηκευμένα εφεδρικά αντίγραφα των block group
descriptors σε αυτό το σύστημα αρχείων;

The block group descriptor table is an array of block group descriptor, used to define parameters of all the block groups.
 It provides the location of the inode bitmap and inode table, block bitmap, number of free blocks and inodes, and some other useful information.

The block group descriptor table starts on the first block following the superblock. 
This would be the third block on a 1KiB block file system, or the second block for 2KiB and larger block file systems. Shadow copies of the block group descriptor table are also stored with every copy of the superblock.
https://www.nongnu.org/ext2-doc/ext2.html#block-group-descriptor-table
### με mount/
Group 0: (Blocks 1-8192)
  Primary superblock at 1, Group descriptors at 2-2
Group 1: (Blocks 8193-16384)
  Backup superblock at 8193, Group descriptors at 8194-8194
Group 2: (Blocks 16385-24576)
  Backup superblock at 16385, Group descriptors at 16386-16386
Group 3: (Blocks 24577-32768)
  Backup superblock at 24577, Group descriptors at 24578-24578
Group 4: (Blocks 32769-40960)
  Backup superblock at 32769, Group descriptors at 32770-32770
Group 5: (Blocks 40961-49152)
  Backup superblock at 40961, Group descriptors at 40962-40962
Group 6: (Blocks 49153-51199)
  Backup superblock at 49153, Group descriptors at 49154-49154

### hexedit
ξέρουμε πως είναι στο block μετά το superblock από την θεωρία. Άρα στα: 2, 8194 ,16386, 24578, 32770, 40962, 49154
με hexdump κοιτάμε με offset την διεύθυνση που ξεκινάει το κάθε block 
offset = block_number * block_size και μετά σε hex

Table 3.12. Block Group Descriptor Structure

Offset (bytes)	Size (bytes)	Description
0	4	bg_block_bitmap
4	4	bg_inode_bitmap
8	4	bg_inode_table
12	2	bg_free_blocks_count
14	2	bg_free_inodes_count
16	2	bg_used_dirs_count
18	2	bg_pad
20	12	bg_reserved

32 bytes total.

generate_command.py
BLOCK_SIZE = 1024
BLOCK_NUMBERS = [2, 8194, 16386, 24578, 32770, 40962, 49154]
DEVICE = "/dev/vdb"

def calculate_offset(block_number):
    return block_number * BLOCK_SIZE

def generate_commands(block_numbers, device):
    for block in block_numbers:
        offset = calculate_offset(block)
        offset_hex = hex(offset)
        print(f"hexdump -C -s {offset} -n 32 {device} # Block {block} (Offset: {offset} / {offset_hex})")

if __name__ == "__main__":
    generate_commands(BLOCK_NUMBERS, DEVICE)

hexdump -C -s 2048 -n 32 /dev/vdb # Block 2 (Offset: 2048 / 0x800)
hexdump -C -s 8390656 -n 32 /dev/vdb # Block 8194 (Offset: 8390656 / 0x800800)
hexdump -C -s 16779264 -n 32 /dev/vdb # Block 16386 (Offset: 16779264 / 0x1000800)
hexdump -C -s 25167872 -n 32 /dev/vdb # Block 24578 (Offset: 25167872 / 0x1800800)
hexdump -C -s 33556480 -n 32 /dev/vdb # Block 32770 (Offset: 33556480 / 0x2000800)
hexdump -C -s 41945088 -n 32 /dev/vdb # Block 40962 (Offset: 41945088 / 0x2800800)
hexdump -C -s 50333696 -n 32 /dev/vdb # Block 49154 (Offset: 50333696 / 0x3000800)
>>>

τρέχοντας τα commands βλέπουμε:

hexdump -C -s 8390656 -n 32 /dev/vdb # Block 8194 (Offset: 8390656 / 0x800800)
hexdump -C -s 16779264 -n 32 /dev/vdb # Block 16386 (Offset: 16779264 / 0x1000800)
hexdump -C -s 25167872 -n 32 /dev/vdb # Block 24578 (Offset: 25167872 / 0x1800800)
hexdump -C -s 33556480 -n 32 /dev/vdb # Block 32770 (Offset: 33556480 / 0x2000800)
hexdump -C -s 41945088 -n 32 /dev/vdb # Block 40962 (Offset: 41945088 / 0x2800800)
hexdump -C -s 50333696 -n 32 /dev/vdb # Block 49154 (Offset: 50333696 / 0x3000800)
00000800  03 00 00 00 04 00 00 00  05 00 00 00 08 1f 1d 07  |................|
00000810  02 00 04 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
00000820
00800800  03 00 00 00 04 00 00 00  05 00 00 00 0a 1f 1d 07  |................|
00800810  02 00 04 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
00800820
01000800  03 00 00 00 04 00 00 00  05 00 00 00 0a 1f 1d 07  |................|
01000810  02 00 04 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
01000820
01800800  03 00 00 00 04 00 00 00  05 00 00 00 0a 1f 1d 07  |................|
01800810  02 00 04 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
01800820
02000800  03 00 00 00 04 00 00 00  05 00 00 00 0a 1f 1d 07  |................|
02000810  02 00 04 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
02000820
02800800  03 00 00 00 04 00 00 00  05 00 00 00 0a 1f 1d 07  |................|
02800810  02 00 04 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
02800820
03000800  03 00 00 00 04 00 00 00  05 00 00 00 0a 1f 1d 07  |................|
03000810  02 00 04 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
03000820

Παρατηρούμε τους ίδιους αριθμούς, γεγονός που βγάζει νόημα καθώς ταιριάζουν με αυτούς που ξεκινάνε στα 2048 bytes (1024*2block) όπου είναι το block #2 που περιέχει το original block group descriptor table BGDT
root@utopia:/home# hexdump -C -s 2048 -n 32 /dev/vdb # Block 49154 (Offset: 50333696 / 0x3000800)
00000800  03 00 00 00 04 00 00 00  05 00 00 00 08 1f 1d 07  |................|
00000810  02 00 04 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
00000820

23. Τι είναι το block bitmap και τι το inode bitmap; Πού βρίσκονται μέσα στον
δίσκο;
Είναι μια δομή στον δίσκο που δείχνει ποια blocks σε ένα συγκεκριμένο group είναι allocated και ποια όχι.
Κάθε block bitmap δείχνει το current state ενός block μέσα σε ένα block group. 1 = used, 0 = free/available
Τα πρώτα 8 blocks του group αντιστοιχούν στα bits 0...7 του byte 0 
Τα επόμενα 8 blocks στα bits 0...7 του byte 1 και ούτω καθεξής

Σε μικρά συστήματα αρχείων, βρίσκονται συνήθως στο πρώτο block, ή στο δεύτερο εάν υπάρχει superblock backup σε κάθε block group. 

Μπορούμε να το δούμε διαβάζοντας την τιμή bg_block_bitmap που αντιστοιχεί στο αντίστοιχο group descriptor.

root@utopia:~# hexdump -C -s 2048 -n 32 /dev/vdb # Block 49154 (Offset: 50333696 / 0x3000800)
00000800  03 00 00 00 04 00 00 00  05 00 00 00 08 1f 1d 07  |................|
00000810  02 00 04 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
00000820

εδώ βλέπουμε πως αντιστοιχεί στο 3ο block, δηλαδή στο επόμενο block από αυτό των group descriptors

Group 0: (Blocks 1-8192)
  Primary superblock at 1, Group descriptors at 2-2
  Block bitmap at 3 (+2)
  Inode bitmap at 4 (+3)

bg_block_bitmap
32bit block id of the first block of the “block bitmap” for the group represented.

The actual block bitmap is located within its own allocated blocks starting at the block ID specified by this value.

24. Τι είναι τα inode tables; Πού βρίσκονται μέσα στον δίσκο;
Τα Inode Tables - Index Node Tables είναι ένας πίνακας όπου αποθηκεύονται τα inodes του κάθε block group.
Τα inode tables βρίσκονται μέσα σε κάθε block group, τα οποία block groups αποθηκεύονται σειριακά μέσα στο δίσκο το ένα μετά το άλλο. 

25. Τι πεδία περιέχει το κάθε inode; Πού αποθηκεύεται μέσα στον δίσκο;
struct ext2_inode {
        __u16   i_mode;         /* File type and access rights */
        __u16   i_uid;          /* Low 16 bits of Owner Uid */
        __u32   i_size;         /* Size in bytes */
        __u32   i_atime;        /* Access time */
        __u32   i_ctime;        /* Creation time */
        __u32   i_mtime;        /* Modification time */
        __u32   i_dtime;        /* Deletion Time */
        __u16   i_gid;          /* Low 16 bits of Group Id */
        __u16   i_links_count;  /* Links count */
        __u32   i_blocks;       /* Blocks count */
        __u32   i_flags;        /* File flags */
	...
	__u32   i_block[EXT2_N_BLOCKS];  /* Pointers to blocks */
	...
};

Αποθηκεύονται μέσα στα inode tables

???

26. Πόσα μπλοκ και πόσα inodes περιέχει το κάθε block group σε αυτό το σύστημα αρχείων;

### με mount/
dumpe2fs
Blocks per group:         8192
Inodes per group:         1832

### hexedit
Superblock
32	4	s_blocks_per_group
40	4	s_inodes_per_group
offset size_in_bytes value_name

root@utopia:~# hexdump -C -s 1024 -n 36 /dev/vdb # Block 49154 (Offset: 50333696 / 0x3000800)
00000400  18 32 00 00 00 c8 00 00  00 0a 00 00 90 c1 00 00  |.2..............|
00000410  0a 32 00 00 01 00 00 00  00 00 00 00 00 00 00 00  |.2..............|
00000420  00 20 00 00                                       |. ..|

root@utopia:~# hexdump -C -s 1024 -n 44 /dev/vdb # Block 49154 (Offset: 50333696 / 0x3000800)
00000400  18 32 00 00 00 c8 00 00  00 0a 00 00 90 c1 00 00  |.2..............|
00000410  0a 32 00 00 01 00 00 00  00 00 00 00 00 00 00 00  |.2..............|
00000420  00 20 00 00 00 20 00 00  28 07 00 00              |. ... ..(...|

s_blocks_per_group = 0x2000 = 8192 decimal
s_inodes_per_group = 0x728 = 1832 decimal


27. Σε ποιο inode αντιστοιχεί το αρχείο /dir2/helloworld σε αυτό το σύστημα
αρχείων;

### με mount/


### hexedit


28. Σε ποιο block group αντιστοιχεί αυτό το inode;

### με mount/


### hexedit


29. Σε ποιο μπλοκ του δίσκου υπάρχει το inode table που περιέχει το παραπάνω
inode;

### με mount/


### hexedit


30. Δείξτε όλα τα πεδία αυτού του inode [128 bytes].

### με mount/


### hexedit


31. Σε ποιο μπλοκ είναι αποθηκευμένα τα δεδομένα αυτού του αρχείου;

### με mount/


### hexedit


32. Τι μέγεθος έχει αυτό το αρχείο;

### με mount/


### hexedit


33. Δείξτε τα περιεχόμενα αυτού του αρχείου.

### με mount/


### hexedit


**Image 2**

1. Συνδέστε την εικόνα του δίσκου στην εικονική μηχανή σας, όπως κάνατε και
για την εικόνα fsdisk1.img και προσαρτήστε τη στον κατάλογο /mnt.

mkdir -p /mnt/fdisk2
mount /dev/vdc /mnt/fdisk2

2. Χρησιμοποιήστε την εντολή touch για να δημιουργήσετε ένα νέο κενό αρχείο /file1 μέσα στο συγκεκριμένο σύστημα αρχείων. Βεβαιωθείτε ότι η
εντολή σας αναφέρεται πράγματι στο συγκεκριμένο σύστημα αρχείων [σε
ποιον κατάλογο το έχετε προσαρτήσει;], κι όχι στον ριζικό κατάλογο του συστήματος.

3. Πέτυχε η εντολή; Αν όχι, τι πρόβλημα υπήρξε;

4. Ποια κλήση συστήματος προσπάθησε να τρέξει η touch, και με ποιον κωδικό λάθους απέτυχε; Υποστηρίξτε την απάντησή σας με χρήση της εντολής
strace.

5. Πόσα αρχεία και πόσους καταλόγους περιέχει το συγκεκριμένο σύστημα αρχείων;

6. Πόσο χώρο καταλαμβάνουν τα δεδομένα και τα μεταδεδομένα του συγκεκριμένου συστήματος αρχείων;

7. Πόσο είναι το μέγεθος του συγκεκριμένου συστήματος αρχείων;

8. Πόσα μπλοκ είναι διαθέσιμα/ελεύθερα στο συγκεκριμένο σύστημα αρχείων;
Ισοδύναμα, έχει ελεύθερο χώρο το συγκεκριμένο σύστημα αρχείων;

9. Αφού υπάρχουν διαθέσιμα μπλοκ, τι σας αποτρέπει από το να δημιουργήσετε
νέο αρχείο;

**Image 3**

root@utopia:~# sha256sum /dev/vdd
982902777d0e66e14379f642365b4fa71a5473348d9af2453e80dbea135bb50d  /dev/vdd
982902777d0e66e14379f642365b4fa71a5473348d9af2453e80dbea135bb50d  fsdisk3-982902777.img
It is a match

1. Ποιο εργαλείο στο Linux αναλαμβάνει τον έλεγχο ενός συστήματος αρχείων
ext2 για αλλοιώσεις;

To fsck.ext2 - e2fsck

2. Ποιοι παράγοντες θα μπορούσαν δυνητικά να οδηγήσουν σε αλλοιώσεις στο
σύστημα αρχείων; Αναφέρετε ενδεικτικά δέκα πιθανές αλλοιώσεις.

2.1 κατακερματισμός δίσκου
2.2 Τερματισμός λειτουργείας όσο γίνεται εγγραφή, διάβασμα αρχείων. 
2.3 Malware και ιοί 
2.4 Αποσύνδεση δίσκου κατά τη λειτουργεία χωρίς unmount
2.5 Διακοπή ρεύματος - sudden power loss
2.6 Bugs στο software λόγω update 
2.7 Αστοχία hardware - sata controller
2.8 Γεμάτος Δίσκος
2.9 Kernel panic που θα προκαλέσει τέλος της λειτουργείας
2.10 Bad sectors στον δίσκο

3. Τρέξτε το εργαλείο αυτό και επιδιορθώστε το σύστημα αρχείων. Αναφέρετε
όλες τις αλλοιώσεις που εντοπίσατε, εξαντλητικά.

3.1
First entry 'BOO' (inode=1717) in directory inode 1717 (/dir-2) should be '.'
3.2
Inode 3425 ref count is 1, should be 2.  Fix<y>? yes
3.3
Block bitmap differences:  +34
Fix<y>? yes
3.4
Free blocks count wrong (926431538, counted=19800).
Fix<y>? yes

root@utopia:~# fsck.ext2 /dev/vdd
e2fsck 1.47.0 (5-Feb-2023)
fsdisk3.img contains a file system with errors, check forced.
Pass 1: Checking inodes, blocks, and sizes
Pass 2: Checking directory structure
First entry 'BOO' (inode=1717) in directory inode 1717 (/dir-2) should be '.'
Fix<y>? yes
Pass 3: Checking directory connectivity
Pass 4: Checking reference counts
Inode 3425 ref count is 1, should be 2.  Fix<y>? yes
Pass 5: Checking group summary information
Block bitmap differences:  +34
Fix<y>? yes
Free blocks count wrong (926431538, counted=19800).
Fix<y>? yes

fsdisk3.img: ***** FILE SYSTEM WAS MODIFIED *****
fsdisk3.img: 23/5136 files (0.0% non-contiguous), 680/20480 blocks
root@utopia:~# fsck.ext2 /dev/vdd
e2fsck 1.47.0 (5-Feb-2023)
fsdisk3.img: clean, 23/5136 files, 680/20480 blocks
root@utopia:~# fsck.ext2 /dev/vdd
e2fsck 1.47.0 (5-Feb-2023)
fsdisk3.img: clean, 23/5136 files, 680/20480 blocks
root@utopia:~#

4. Επαναφέρετε το δίσκο στην πρότερή του κατάσταση, από την αρχική εικόνα.  
Εντοπίστε τις αλλοιώσεις με χρήση της μεθόδου hexedit.

Θα διαβάσουμε αρχικά τιμές από το superblock.

Offset (bytes)	Size (bytes)	Description
0	4	s_inodes_count = 10 14 00 00 = 5144
4	4	s_blocks_count = 00 50 00 00 = 20480
8	4	s_r_blocks_count = 00 04 00 00 = 1024
12	4	s_free_blocks_count = 32 39 38 37 =!!!!!!!!!!!!! 926431538 !!!!!!!!!!!!!!
16	4	s_free_inodes_count = f9 13 00 00 = 5113
20	4	s_first_data_block = 01 00 00 00 = 1
24	4	s_log_block_size = 0 = 1024 << 0 = 1024 bytes 
28	4	s_log_frag_size = 0 = 1024<<0 = 1024 bytes
32	4	s_blocks_per_group = 00 20 00 00 = 8192 blocks
36	4	s_frags_per_group = 00 20 00 00 = 8192 blocks
40	4	s_inodes_per_group = b0 06 00 00 = 1712
44	4	s_mtime = e9 7a 78 65 = posix time be -> decimal ....
48	4	s_wtime = ea 7a 78 65 = same
52	2	s_mnt_count = 01 00 
54	2	s_max_mnt_count = ff ff
56	2	s_magic = 53 ef = ext2 magic number (identifier) in little endian
58	2	s_state = 01 00 = 0x0001 = clean
60	2	s_errors = 01 00 = continue 
62	2	s_minor_rev_level = 00 00
64	4	s_lastcheck = e9 7a 78 65 
68	4	s_checkinterval = 00 00 00 00  
72	4	s_creator_os = 00 00 00 00 = LINUX X:)
76	4	s_rev_level = 01 00 00 00
80	2	s_def_resuid = 0
82	2	s_def_resgid = 0
-- EXT2_DYNAMIC_REV Specific --
84	4	s_first_ino = 0b 00 00 00 = 11
88	2	s_inode_size = 80 00 = 128 
90	2	s_block_group_nr = 00 00 = 0
92	4	s_feature_compat = 00 00 00 00
96	4	s_feature_incompat = 0
100	4	s_feature_ro_compat = 0
104	16	s_uuid = 19 03 21 43 52 ce 49 17 8e c5 99 1c 89 ee 42 1b
120	16	s_volume_name = 66 73 64 69 73 6b 33 2e 69 6d 67 00 00 00 00 00
136	64	s_last_mounted
200	4	s_algo_bitmap
-- Performance Hints --
204	1	s_prealloc_blocks
205	1	s_prealloc_dir_blocks
206	2	(alignment)
-- Journaling Support --
208	16	s_journal_uuid
224	4	s_journal_inum
228	4	s_journal_dev
232	4	s_last_orphan
-- Directory Indexing Support --
236	4 x 4	s_hash_seed
252	1	s_def_hash_version
253	3	padding - reserved for future expansion
-- Other options --
256	4	s_default_mount_options
260	4	s_first_meta_bg




5. Επιδιορθώστε κάθε αλλοίωση ξεχωριστά με χρήση της μεθόδου hexedit. Για
κάθε μία από τις αλλοιώσεις που επιδιορθώνετε, τρέξτε το εργαλείο fsck με
τρόπο που δεν προκαλεί καμία αλλαγή [“dry run”] και επιβεβαιώστε ότι πλέον
δεν την εντοπίζει.
