### με mount/
### με hexedit

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
11. Τι μέγεθος έχει το inode σε αυτό το σύστημα αρχείων;
12. Πόσα διαθέσιμα μπλοκ και πόσα διαθέσιμα inodes υπάρχουν σε αυτό το σύστημα αρχείων;
13. Τι είναι το superblock στο σύστημα αρχείων ext2;
14. Πού βρίσκεται μέσα στον δίσκο σε ένα σύστημα αρχείων ext2;
15. Για ποιο λόγο έχει νόημα να υπάρχουν εφεδρικά αντίγραφα του superblock
στο σύστημα αρχείων ext2;
16. Σε ποια μπλοκ βρίσκονται αποθηκευμένα εφεδρικά αντίγραφα του superblock
σε αυτό το σύστημα αρχείων;
17. Τι είναι ένα block group στο σύστημα αρχείων ext2;
18. Πόσα block groups έχει ένα σύστημα αρχείων ext2 και πώς κατανέμονται;
19. Πόσα block groups περιέχει αυτό το σύστημα αρχείων;
20. Τι είναι ο block group descriptor στο σύστημα αρχείων ext2;
21. Για ποιο λόγο έχει νόημα να υπάρχουν εφεδρικά αντίγραφα των block group
descriptors στο σύστημα αρχείων ext2;
22. Σε ποια μπλοκ βρίσκονται αποθηκευμένα εφεδρικά αντίγραφα των block group
descriptors σε αυτό το σύστημα αρχείων;
23. Τι είναι το block bitmap και τι το inode bitmap; Πού βρίσκονται μέσα στον
δίσκο;
24. Τι είναι τα inode tables; Πού βρίσκονται μέσα στον δίσκο;
25. Τι πεδία περιέχει το κάθε inode; Πού αποθηκεύεται μέσα στον δίσκο;
26. Πόσα μπλοκ και πόσα inodes περιέχει το κάθε block group σε αυτό το σύστημα αρχείων;
27. Σε ποιο inode αντιστοιχεί το αρχείο /dir2/helloworld σε αυτό το σύστημα
αρχείων;
28. Σε ποιο block group αντιστοιχεί αυτό το inode;
29. Σε ποιο μπλοκ του δίσκου υπάρχει το inode table που περιέχει το παραπάνω
inode;
30. Δείξτε όλα τα πεδία αυτού του inode [128 bytes].
31. Σε ποιο μπλοκ είναι αποθηκευμένα τα δεδομένα αυτού του αρχείου;
32. Τι μέγεθος έχει αυτό το αρχείο;
33. Δείξτε τα περιεχόμενα αυτού του αρχείου.

**Image 2**

**Image 3**
