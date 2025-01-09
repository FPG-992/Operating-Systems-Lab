# Part 1

Για τα fields του filesystems είδαμε [αυτό](https://wiki.osdev.org/Ext2). Το οποίο μπορεί να συνιωιστεί στο παρακάτω πίνακα:
```
Superblock Fields
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

Block Group Descriptor
0	3	4	Block address of block usage bitmap
4	7	4	Block address of inode usage bitmap
8	11	4	Starting block address of inode table
12	13	2	Number of unallocated blocks in group
14	15	2	Number of unallocated inodes in group
16	17	2	Number of directories in group
18	31	X	(Unused)

Inode Data Structure
0	1	2	Type and Permissions (see below)
2	3	2	User ID
4	7	4	Lower 32 bits of size in bytes
8	11	4	Last Access Time (in POSIX time)
12	15	4	Creation Time (in POSIX time)
16	19	4	Last Modification time (in POSIX time)
20	23	4	Deletion time (in POSIX time)
24	25	2	Group ID
26	27	2	Count of hard links (directory entries) to this inode. When this reaches 0, the data blocks are marked as unallocated.
28	31	4	Count of disk sectors (not Ext2 blocks) in use by this inode, not counting the actual inode structure nor directory entries linking to the inode.
32	35	4	Flags (see below)
36	39	4	Operating System Specific value #1
40	43	4	Direct Block Pointer 0
44	47	4	Direct Block Pointer 1
48	51	4	Direct Block Pointer 2
52	55	4	Direct Block Pointer 3
56	59	4	Direct Block Pointer 4
60	63	4	Direct Block Pointer 5
64	67	4	Direct Block Pointer 6
68	71	4	Direct Block Pointer 7
72	75	4	Direct Block Pointer 8
76	79	4	Direct Block Pointer 9
80	83	4	Direct Block Pointer 10
84	87	4	Direct Block Pointer 11
88	91	4	Singly Indirect Block Pointer (Points to a block that is a list of block pointers to data)
92	95	4	Doubly Indirect Block Pointer (Points to a block that is a list of block pointers to Singly Indirect Blocks)
96	99	4	Triply Indirect Block Pointer (Points to a block that is a list of block pointers to Doubly Indirect Blocks)
100	103	4	Generation number (Primarily used for NFS)
104	107	4	In Ext2 version 0, this field is reserved. In version >= 1, Extended attribute block (File ACL).
108	111	4	In Ext2 version 0, this field is reserved. In version >= 1, Upper 32 bits of file size (if feature bit set) if it's a file, Directory ACL if it's a directory
112	115	4	Block address of fragment
116	127	12	Operating System Specific Value #2

Directory Entry
0	3	4	Inode
4	5	2	Total size of this entry (Including all subfields)
6	6	1	Name Length least-significant 8 bits
7	7	1	Type indicator (only if the feature bit for "directory entries have file type byte" is set, else this is the most-significant 8 bits of the Name Length)
8	8+N-1	N	Name characters
```


## Εικόνα fsdisk1.img
### Ερώτηση 1: Τροποποιήστε κατάλληλα το αρχείο utopia.sh ώστε να προσθέσετε στην εικονική μηχανή utopia έναν επιπλέον δίσκο για την εικόνα fsdisk1.img. Ποια είναι η προσθήκη που κάνατε; Ποια συσκευή στο utopia είναι αυτή που μόλις προσθέσατε;
Τροποποιήστε κατάλληλα το αρχείο utopia.sh ώστε να προσθέσετε στην εικονική μηχανή utopia έναν επιπλέον δίσκο για την εικόνα fsdisk1.img. Ποια είναι η προσθήκη που κάνατε; Ποια συσκευή στο utopia είναι αυτή που μόλις προσθέσατε;

Για να προσθέσουμε τον δίσκο για την εικόνα dsdisk1.img προσθέσαμε επιπλέον **-drive** flags στην **exec** εντολή (αφού πρώτα έχουμε κατεβάσει το αρχείο `ext2-vdisks.tgz` και το εχουμε κάνει extract στο ίδιο directory που υπάρχει και το `utopia.sh`):

``` bash
exec $QEMU -enable-kvm -M pc -m $UTOPIA_MEMORY_MB \
    ...
    -drive file=./fsdisk1-7c2b425a6.img,format=raw,if=virtio \
    -drive file=./fsdisk2-a0173283d.img,format=raw,if=virtio \
    -drive file=./fsdisk3-982902777.img,format=raw,if=virtio \
    ...
```
Οπώς φαίνεται έχουμε προσθέσει και τους τρις δίσκους μιας και θα τους χρειαστούμε αργότερα.

Χρησιμοποιώντας, επίσης την εντολή `blkid` είδαμε ότι η συσκευή που προσθέσαμε είναι η `vdb`:
```bash
/dev/vdd: LABEL="fsdisk3.img" UUID="19032143-52ce-4917-8ec5-991c89ee421b" BLOCK_SIZE="1024" TYPE="ext2"
/dev/vdb: LABEL="fsdisk1.img" UUID="c63028e5-711b-410d-a263-e7ca2b15a8d3" BLOCK_SIZE="1024" TYPE="ext2"
/dev/vdc: LABEL="fsdisk2.img" UUID="d1266ad1-dae1-4275-8136-a29a4dfc9d1f" BLOCK_SIZE="1024" TYPE="ext2"
/dev/vda1: UUID="c3f61444-3056-4fc3-a339-9c8aea4314f7" BLOCK_SIZE="4096" TYPE="ext4" PARTUUID="8cc52ab1-01"
```

Το ίδιο μπορούμε να το επαληθεύσουμε αν χρησιμοποιήσουμε την εντολή `hexdump`:
```bash
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
```

---

### Ερώτηση 2: Τι μέγεθος έχει ο δίσκος που προσθέσατε στο utopia;
#### Προσέγγιση: tools
Εκτελούμε την εντολή `fdisk` στην συσκευή που αντιστοιχεί στην εικόνα fsdisk1.img και παίρνουμε το ακόλουθο αποτέλεσμα:
```bash
root@utopia:~# fdisk -l /dev/vdb
Disk /dev/vdb: 50 MiB, 52428800 bytes, 102400 sectors
Units: sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
```
Οπότε βλέπουμε ότι ο δίσκος έχει μέγεθος 50MiB (ισοδύναμα 52428800 bytes)

#### Προσέγγιση: hexedit
Μέσω της `hexdump` βλέπουμε ότι ο δίσκος περιέχει: `51200` μπλοκς. Και το μέγεθος κάθε μπλοκ είναι `1024`. Οπότε συνολικό μέγεθος: `52428800` bytes. 

---

### Ερώτηση 3: Τι σύστημα αρχείων περιέχει;
#### Προσέγγιση: tools
Σύμφωωνα με το [man-page](https://linux.die.net/man/8/blkid) της εντολής `blkid`: blkid can determine the type of content (e.g. filesystem, swap) a block device holds, and also attributes (tokens, NAME=value pairs) from the content metadata (e.g. LABEL or UUID fields). Συνεπώς, παρατηρούμε ότι είναι `ext2`:
```bash
root@utopia:~# blkid /dev/vdb
/dev/vdb: LABEL="fsdisk1.img" UUID="c63028e5-711b-410d-a263-e7ca2b15a8d3" BLOCK_SIZE="1024" TYPE="ext2"
```

#### Προσέγγιση: hexedit
Χρησιμοποιούμε την `hexdump` και παίρνουμε (read only the super block):
```bash
root@utopia:~# hexdump -s 1024 -n 58 -C /dev/vdb
00000400  18 32 00 00 00 c8 00 00  00 0a 00 00 90 c1 00 00  |.2..............|
00000410  0a 32 00 00 01 00 00 00  00 00 00 00 00 00 00 00  |.2..............|
00000420  00 20 00 00 00 20 00 00  28 07 00 00 a5 43 7d 67  |. ... ..(....C}g|
00000430  26 5d 7d 67 05 00 ff ff  53 ef                    |&]}g....S.|
0000043a
```
Στο **0x38** από το superblock start βλέπουμε το `53 ef` που είναι το magic number (little endian του EF53) που ανταποκρίνεται στα ext2/ext3/ext4.

---

### Ερώτηση 4: Πότε ακριβώς δημιουργήθηκε αυτό το σύστημα αρχείων; Δείξτε τη χρονοσφραγίδα [timestamp].
#### Προσέγγιση: tools
Σύμφωνα με το [man-page](https://linux.die.net/man/8/dumpe2fs) της εντολής `dumpe2fs`: _dumpe2fs prints the super block and blocks group information for the filesystem present on device._

Οπότε παίρνουμε:
```bash
root@utopia:~# dumpe2fs /dev/vdb
...
Filesystem created:       Tue Dec 12 15:23:16 2023
...
```

#### Προσέγγιση: hexedit
Στα byte 44 μέχρι 47 βλέπουμε το *Last mount time (in POSIX time)* το οποίο έιναι το `e4 7a 78 65` σε little endian. Για να βρούμε το date θα το μετατρέψουμε σε big endian, δηλαδή σε `65 78 7a e5`, μετά σε δεκαδική μορφή: `1702394597` και ύστερα χρησιμοποιούμε την εντολή date στο terminal για να πάρουμε την ακριβή ημερομινία:
```bash
root@utopia:~# date -d @1702394596
Tue Dec 12 03:23:17 PM UTC 2023
```

---

### Ερώτηση 5: Πότε ακριβώς προσαρτήθηκε τελευταία φορά; Δείξτε τη χρονοσφραγίδα.
#### Προσέγγιση: tools
Πάλι με χρήση `dumpe2fs` βλέπουμε ότι:
```bash
root@utopia:~# dumpe2fs /dev/vdb
...
Last mount time:          Tue Dec 12 15:23:16 2023
...
```

#### Προσέγγιση: hexedit
Βρίσκεται στα bytes 48-51 του hexdump δηλαδή, `e5 7a 78 65` το οποίο σε big endian και δεκαδική μορφή είναι το `1702394597`, άρα:
```bash
root@utopia:~# date -d @1702394597
Tue Dec 12 03:23:17 PM UTC 2023
```

---

### Ερώτηση 6: Σε ποιο μονοπάτι προσαρτήθηκε τελευταία φορά;
#### Προσέγγιση: tools
Πάλι με χρήση `dumpe2fs` βλέπουμε ότι:
```bash
root@utopia:~# dumpe2fs /dev/vdb
...
Last mounted on:          /cslab-bunker
...
```

#### Προσέγγιση: hexedit
Στο `hexdump` βλέπουμε ότι:
```bash
00000480  69 6d 67 00 00 00 00 00  2f 63 73 6c 61 62 2d 62  |img...../cslab-b|
00000490  75 6e 6b 65 72 00 00 00  00 00 00 00 00 00 00 00  |unker...........|
```

---


### Ερώτηση 7: Πότε ακριβώς τροποποιήθηκε τελευταία φορά; Δείξτε τη χρονοσφραγίδα.
#### Προσέγγιση: tools
Πάλι με χρήση `dumpe2fs` βλέπουμε ότι:
```bash
root@utopia:~# dumpe2fs /dev/vdb
...
Last write time:          Tue Dec 12 15:23:17 2023
...
```

#### Προσέγγιση: hexedit
Στο `hexdump` βλέπουμε ότι:
```bash
00000400  18 32 00 00 00 c8 00 00  00 0a 00 00 90 c1 00 00  |.2..............|
00000410  0a 32 00 00 01 00 00 00  00 00 00 00 00 00 00 00  |.2..............|
00000420  00 20 00 00 00 20 00 00  28 07 00 00 e4 7a 78 65  |. ... ..(....zxe|
00000430  e5 7a 78 65                                       |.zxe|
```
Το μετατρέπουμε σε big endian `65 78 7a e5` και ύστερα σε δεκαδικό `1702394597` και έχουμε:
```bash
root@utopia:~# date -d @1702394597
Tue Dec 12 03:23:17 PM UTC 2023
```

---


### Ερώτηση 8: Τι είναι το μπλοκ σε ένα σύστημα αρχείων;
Το μπλοκ (block) σε ένα σύστημα αρχείων είναι η βασική μονάδα αποθήκευσης δεδομένων που χρησιμοποιείται για την οργάνωση και τη διαχείριση του χώρου στον δίσκο. Είναι ένα συνεχόμενο τμήμα του αποθηκευτικού μέσου, με καθορισμένο μέγεθος, και αποτελεί το θεμέλιο για την αποθήκευση αρχείων και μεταδεδομένων στο σύστημα αρχείων.

---

### Ερώτηση 9: Τι μέγεθος μπλοκ [block size] χρησιμοποιεί αυτό το σύστημα αρχείων;
#### Προσέγγιση: tools
Πάλι με χρήση `dumpe2fs` βλέπουμε ότι:
```bash
root@utopia:~# dumpe2fs /dev/vdb
...
Block size:               1024
...
```

#### Προσέγγιση: hexedit
Το block size μπορούμε να το υπολογίσουμε στα bit 24-27 του `hexdump`, τα οποία μας δίνουν το `log2 (block size) - 10` (the number to shift 1,024 to the left by to obtain the block size):
```bash
root@utopia:~# hexdump -s 1024 -n 28 -C /dev/vdb
00000400  18 32 00 00 00 c8 00 00  00 0a 00 00 90 c1 00 00  |.2..............|
00000410  0a 32 00 00 01 00 00 00  00 00 00 00                |.2.........|
```
Οπότε έχουμε το `00 00 00 00`, οπότε το block size είναι το 1024.

---

### Ερώτηση 10: Τι είναι το inode σε ένα σύστημα αρχείων;
Το inode (Index Node) είναι μια δομή δεδομένων σε ένα σύστημα αρχείων που χρησιμοποιείται για να αποθηκεύει μεταδεδομένα σχετικά με ένα αρχείο ή έναν κατάλογο. Το inode δεν αποθηκεύει τα ίδια τα δεδομένα του αρχείου, αλλά περιλαμβάνει πληροφορίες όπως ταυτότητα αρχείου, μέγεθος αρχείου, τύπος αρχείου, δικαιώματα πρόσβασης, timestamps, hard links, και δείκτες στα μπλοκ δεδομένων που περιέχουν το περιεχόμενο του αρχείου.

---

### Ερώτηση 11: Τι μέγεθος έχει το inode σε αυτό το σύστημα αρχείων;
#### Προσέγγιση: tools
Με χρήση `dumpe2fs` βλέπουμε ότι:
```bash
root@utopia:~# dumpe2fs /dev/vdb
...
Inode size:               128
...
```

#### Προσέγγιση: hexedit
Το inode size βρίσκεται στα bytes 88, 89 του hexdump:
```bash
root@utopia:~# hexdump -s 1024 -n 90 -C /dev/vdb
00000400  18 32 00 00 00 c8 00 00  00 0a 00 00 90 c1 00 00  |.2..............|
00000410  0a 32 00 00 01 00 00 00  00 00 00 00 00 00 00 00  |.2..............|
00000420  00 20 00 00 00 20 00 00  28 07 00 00 e4 7a 78 65  |. ... ..(....zxe|
00000430  e5 7a 78 65 01 00 ff ff  53 ef 01 00 01 00 00 00  |.zxe....S.......|
00000440  e4 7a 78 65 00 00 00 00  00 00 00 00 01 00 00 00  |.zxe............|
00000450  00 00 00 00 0b 00 00 00  80 00                    |..........|
```
Άρα έχουμε `80 00`, δηλαδή το `00 80` το οποίο είναι το `128` σε δεκαδική μορφή.

---

### Ερώτηση 12: Πόσα διαθέσιμα μπλοκ και πόσα διαθέσιμα inodes υπάρχουν σε αυτό το σύστημα αρχείων;
#### Προσέγγιση: tools
Με χρήση `dumpe2fs` βλέπουμε ότι:
```bash
root@utopia:~# dumpe2fs /dev/vdb
...
Free blocks:              49552
Free inodes:              12810
...
```

#### Προσέγγιση: hexedit
Ta unallocated blocks βρίσκονται στα bytes 12-15, και τα unallocated index στα 16-19:
```bash
root@utopia:~# hexdump -s 1024 -n 20 -C /dev/vdb
00000400  18 32 00 00 00 c8 00 00  00 0a 00 00 90 c1 00 00  |.2..............|
00000410  0a 32 00 00                                       |.2..|
```
Και έχουμε `0a 32 00 00` unallocated inodes, και `90 c1 00 00` unallocated blocks. Δηλαδή σε δεκαδική μορφή `12554`, και `49552` αντίστοιχα.

---

### Ερώτηση 13: Τι είναι το superblock στο σύστημα αρχείων ext2;
Το superblock στο σύστημα αρχείων ext2 είναι μια κρίσιμη δομή δεδομένων που περιέχει βασικές πληροφορίες για τη διαμόρφωση και την κατάσταση του συστήματος αρχείων. Είναι ουσιαστικά ο "κεντρικός πίνακας ελέγχου" του συστήματος αρχείων και χρησιμοποιείται για τη διαχείριση και την οργάνωση των δεδομένων στον δίσκο.
Αποθηκεύει παραμέτρους όπως το μέγεθος του συστήματος αρχείων, το μέγεθος των μπλοκ, και τον αριθμό των inodes.
Επειδή το superblock είναι κρίσιμο για τη λειτουργία του συστήματος αρχείων, το ext2 κρατά αντίγραφα ασφαλείας του superblock σε διάφορα σημεία του δίσκου (backup superblocks). Αυτά τα αντίγραφα είναι χρήσιμα σε περίπτωση καταστροφής του κύριου superblock.

---

### Ερώτηση 14: Πού βρίσκεται μέσα στον δίσκο σε ένα σύστημα αρχείων ext2;
Το κύριο superblock βρίσκεται 1 KB (1024 bytes) μετά την αρχή του διαμερίσματος. Αυτό σημαίνει ότι αν το μέγεθος του μπλοκ είναι 1 KB, το superblock ξεκινά στο μπλοκ 1. Αν το μέγεθος του μπλοκ είναι 2 KB ή 4 KB, το superblock ξεκινά στο μέσο του πρώτου μπλοκ.

---


### Ερώτηση 15: Για ποιο λόγο έχει νόημα να υπάρχουν εφεδρικά αντίγραφα του superblock στο σύστημα αρχείων ext2;
Η ύπαρξη εφεδρικών αντιγράφων του superblock σε ένα σύστημα αρχείων έχει κρίσιμη σημασία για την ανθεκτικότητα και την αξιοπιστία του συστήματος. Ο βασικός λόγος για αυτήν τη σχεδίαση είναι η προστασία από σφάλματα ή καταστροφές που μπορεί να προκύψουν στον δίσκο ή στο κύριο superblock.

---

### Ερώτηση 16:  Σε ποια μπλοκ βρίσκονται αποθηκευμένα εφεδρικά αντίγραφα του superblock σε αυτό το σύστημα αρχείων;
_The first version of ext2 (revision 0) stores a copy at the start of every block group, along with backups of the group descriptor block(s). Because this can consume a considerable amount of space for large filesystems, later revisions can optionally reduce the number of backup copies by only putting backups in specific groups (this is the sparse superblock feature). The groups chosen are 0, 1 and powers of 3, 5 and 7._
#### Προσέγγιση: tools
Με χρήση `dumpe2fs` βλέπουμε ότι:
```bash
root@utopia:~# dumpe2fs /dev/vdb
...
Group 0: (Blocks 1-8192)
...
Group 1: (Blocks 8193-16384)
...
Group 2: (Blocks 16385-24576)
...
Group 3: (Blocks 24577-32768)
...
Group 4: (Blocks 32769-40960)
...
Group 5: (Blocks 40961-49152)
...
Group 6: (Blocks 49153-51199)
...
```
Οπότε τα εφεδρικά superblocks βρίσκονται στα blocks 8193 16385, 24577, 32769, 40961, 49153

#### Προσέγγιση: hexedit
Θα κοιτάξουμε στο `hexdump` εκεί που έχουμε **53 ef** magic number. Εκεί θα βρίσκονται τα superblocks:
```bash
root@utopia:~# hexdump -C /dev/vdb | grep '53 ef'
00000430  e5 7a 78 65 01 00 ff ff  53 ef 01 00 01 00 00 00  |.zxe....S.......|
00800430  e5 7a 78 65 01 00 ff ff  53 ef 00 00 01 00 00 00  |.zxe....S.......|
01000430  e5 7a 78 65 01 00 ff ff  53 ef 00 00 01 00 00 00  |.zxe....S.......|
01800430  e5 7a 78 65 01 00 ff ff  53 ef 00 00 01 00 00 00  |.zxe....S.......|
02000430  e5 7a 78 65 01 00 ff ff  53 ef 00 00 01 00 00 00  |.zxe....S.......|
02800430  e5 7a 78 65 01 00 ff ff  53 ef 00 00 01 00 00 00  |.zxe....S.......|
03000430  e5 7a 78 65 01 00 ff ff  53 ef 00 00 01 00 00 00  |.zxe....S.......|
```
Οπότε για να βρω το block number θα χρησιμοποιήσω τον τύπο: `block number = [((offset_of_magic)-56)/block_size]`, όπου *offset_of_magic* είναι το offset της γραμμής +8. Οπότε έχουμε:
```bash
0x430 -> 1072 + 8 -> 1080 - 56 -> 1024 / 1024 -> 1
0x800430 -> 8389680 + 8 -> 8389688 - 56 -> 8389632 / 1024 -> 8193
0x1000430 -> 16778288 + 8 -> 16778296 - 56 -> 16778240 / 1024 -> 16385
0x1800430 -> 25166896 + 8 -> 25166904 - 56 -> 25166848 / 1024 -> 24577
0x2000430 -> 33555504 + 8 -> 33555512 - 56 -> 33555456 / 1024 -> 32769
0x2800430 -> 41944112 + 8 -> 41944120 - 56 -> 41944064 / 1024 -> 40961
0x3000430 -> 50332720 + 8 -> 50332728 - 56 -> 50332672 / 1024 -> 49153
```

---

### Ερώτηση 17: Τι είναι ένα block group στο σύστημα αρχείων ext2;
Στο σύστημα αρχείων ext2, το block group είναι μια βασική μονάδα οργάνωσης του χώρου στον δίσκο. Ο δίσκος χωρίζεται σε πολλές ομάδες μπλοκ (block groups), καθεμία από τις οποίες περιέχει έναν καθορισμένο αριθμό μπλοκ και τις απαραίτητες δομές δεδομένων για τη διαχείριση των μπλοκ και των inodes.
Αυτή η προσέγγιση της ομαδοποίησης έχει σχεδιαστεί για να μειώσει την κατακερματισμένη αποθήκευση δεδομένων και να αυξήσει την ταχύτητα πρόσβασης, οργανώνοντας τα δεδομένα τοπικά σε κάθε block group.

---

### Ερώτηση 18: Πόσα block groups έχει ένα σύστημα αρχείων ext2 και πώς κατανέμονται;
Ο αριθμός των block groups σε ένα σύστημα αρχείων ext2 εξαρτάται από το συνολικό μέγεθος του συστήματος αρχείων και το μέγεθος κάθε block group. Τα block groups κατανέμονται ομοιόμορφα στο σύστημα αρχείων και καθένα από αυτά διαχειρίζεται έναν καθορισμένο αριθμό μπλοκ.

Ο αριθμός των block groups υπολογίζεται με τον τύπο:

$$
\text{Αριθμός Block Groups} = \left\lceil \frac{\text{Συνολικά Blocks στο Σύστημα Αρχείων}}{\text{Blocks ανά Block Group}} \right\rceil
$$

όπου:
    * Συνολικά Blocks: Ο συνολικός αριθμός μπλοκ στο σύστημα αρχείων
    * Blocks ανά Block Group: Εξαρτάται από το μέγεθος του block group, το οποίο καθορίζεται κατά τη δημιουργία του συστήματος αρχείων. Συνήθως, ένα block group περιέχει 8192 μπλοκ για μέγεθος μπλοκ 1 KB, 32768 μπλοκ για μέγεθος μπλοκ 4 KB, κ.λπ.

---

### Ερώτηση 19: Πόσα block groups περιέχει αυτό το σύστημα αρχείων;
#### Προσέγγιση: tools
Με χρήση `dumpe2fs` βλέπουμε ότι:
```bash
root@utopia:~# dumpe2fs /dev/vdb
...
Group 0: (Blocks 1-8192)
...
Group 1: (Blocks 8193-16384)
...
Group 2: (Blocks 16385-24576)
...
Group 3: (Blocks 24577-32768)
...
Group 4: (Blocks 32769-40960)
...
Group 5: (Blocks 40961-49152)
...
Group 6: (Blocks 49153-51199)
...
```
Άρα έχουμε 7 block groups.

#### Προσέγγιση: hexedit
Θα βρούμε το _Total number of blocks in file system_ (byte 4-7) και το _Number of blocks in each block group_ (byte 32-35):
```bash
root@utopia:~# hexdump -s 1024 -n 8 -C /dev/vdb
00000400  18 32 00 00 00 c8 00 00                           |.2......|
```
Άρα έχουμε `00 c8 00 00`, δηλαδή `51200` συνολικά μπλοκ.

```bash
root@utopia:~# hexdump -s 1024 -n 36 -C /dev/vdb
00000400  18 32 00 00 00 c8 00 00  00 0a 00 00 90 c1 00 00  |.2..............|
00000410  0a 32 00 00 01 00 00 00  00 00 00 00 00 00 00 00  |.2..............|
00000420  00 20 00 00                                       |. ..|
00000424
```
Άρα έχουμε `00 20 00 00 `, δηλαδή `8192` μπλοκ σε κάθε group. Συνεπώς έοχυμε 51200 / 8192 = 6.25, άρα 7 block groups.

---

### Ερώτηση 20: Τι είναι ο block group descriptor στο σύστημα αρχείων ext2;
Ο block group descriptor στο σύστημα αρχείων ext2 είναι μια κρίσιμη δομή δεδομένων που περιγράφει τις βασικές πληροφορίες για κάθε block group. Είναι ουσιαστικά ένας "χάρτης" που επιτρέπει στο λειτουργικό σύστημα να διαχειρίζεται και να εντοπίζει τις βασικές δομές του block group, όπως τα bitmap και τους πίνακες inodes.
```c
struct ext2_group_desc
{
	__u32	bg_block_bitmap        /* Blocks bitmap block */
	__u32	bg_inode_bitmap;       /* Inodes bitmap block */
	__u32	bg_inode_table;		   /* Inodes table block */
	__u16	bg_free_blocks_count;  /* Free blocks count */
	__u16	bg_free_inodes_count;  /* Free inodes count */
	__u16	bg_used_dirs_count;	   /* Directories count */
	__u16	bg_pad;
	__u32	bg_reserved[3];
};
```

---

### Ερώτηση 21: Για ποιο λόγο έχει νόημα να υπάρχουν εφεδρικά αντίγραφα των block group descriptors στο σύστημα αρχείων ext2;
Η ύπαρξη εφεδρικών αντιγράφων των block group descriptors στο σύστημα αρχείων ext2 έχει μεγάλη σημασία για την ανθεκτικότητα, την αξιοπιστία και την ανάκτηση του συστήματος αρχείων. Οι block group descriptors είναι ζωτικής σημασίας για τη λειτουργία του συστήματος, καθώς περιέχουν πληροφορίες για τη δομή και τη διαχείριση κάθε block group. Αν καταστραφούν, η πρόσβαση στα δεδομένα και στα μεταδεδομένα του συστήματος αρχείων μπορεί να γίνει αδύνατη.

---

### Ερώτηση 22: Σε ποια μπλοκ βρίσκονται αποθηκευμένα εφεδρικά αντίγραφα των block group descriptors σε αυτό το σύστημα αρχείων;
#### Προσέγγιση: tools
Με χρήση `dumpe2fs` βλέπουμε ότι:
```bash
root@utopia:~# dumpe2fs /dev/vdb
...
Group 1: (Blocks 8193-16384)
  Backup superblock at 8193, Group descriptors at 8194-8194
  ...
Group 2: (Blocks 16385-24576)
  Backup superblock at 16385, Group descriptors at 16386-16386
  ...
Group 3: (Blocks 24577-32768)
  Backup superblock at 24577, Group descriptors at 24578-24578
  ...
Group 4: (Blocks 32769-40960)
  Backup superblock at 32769, Group descriptors at 32770-32770
  ...
Group 5: (Blocks 40961-49152)
  Backup superblock at 40961, Group descriptors at 40962-40962
Group 6: (Blocks 49153-51199)
  Backup superblock at 49153, Group descriptors at 49154-49154
  ...
```

#### Προσέγγιση: hexedit
Ξέρουμε ότι οι block group descriptors βρίσκονται μετά από τα superblocks.
Άρα στα: 2, 8194 ,16386, 24578, 32770, 40962, 49154.
Για τους group descriptors έχουμε:
```
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
```
Φτιάξαμε ένα script το οποίο παράγει τα hexdumps που πρέπει να τρέξουμε για να πάρουμε το ζητούμενο:
```python
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
```
Με αποτέλεσμα:
```bash
hexdump -C -s 2048 -n 32 /dev/vdb # Block 2 (Offset: 2048 / 0x800)
hexdump -C -s 8390656 -n 32 /dev/vdb # Block 8194 (Offset: 8390656 / 0x800800)
hexdump -C -s 16779264 -n 32 /dev/vdb # Block 16386 (Offset: 16779264 / 0x1000800)
hexdump -C -s 25167872 -n 32 /dev/vdb # Block 24578 (Offset: 25167872 / 0x1800800)
hexdump -C -s 33556480 -n 32 /dev/vdb # Block 32770 (Offset: 33556480 / 0x2000800)
hexdump -C -s 41945088 -n 32 /dev/vdb # Block 40962 (Offset: 41945088 / 0x2800800)
hexdump -C -s 50333696 -n 32 /dev/vdb # Block 49154 (Offset: 50333696 / 0x3000800)
```

Τρέχοντας οποιοδήποτε βλέπουμε τους ίδιους αριθμούς, γεγονός που βγάζει νόημα καθώς ταιριάζουν με αυτούς που ξεκινάνε στα 2048 bytes (1024*2block) όπου είναι το block #2 που περιέχει το original block group descriptor table BGDT.
```bash
00000800  03 00 00 00 04 00 00 00  05 00 00 00 08 1f 1d 07  |................|
00000810  02 00 04 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
00000820
```

---

### Ερώτηση 23: Τι είναι το block bitmap και τι το inode bitmap; Πού βρίσκονται μέσα στον δίσκο;
Το block bitmap και το inode bitmap είναι κρίσιμες δομές δεδομένων στο σύστημα αρχείων ext2, οι οποίες χρησιμοποιούνται για τη διαχείριση του χώρου αποθήκευσης και των inodes, αντίστοιχα.

Το block bitmap είναι ένας χάρτης που παρακολουθεί την κατάσταση όλων των blocks δεδομένων μέσα σε ένα block group. Κάθε bit στο block bitmap αντιστοιχεί σε ένα block του block group και είναι 1 αν το block είναι κατειλημμένο, αλλίως 0. Όταν το σύστημα αρχείων χρειάζεται να αποθηκεύσει νέα δεδομένα, χρησιμοποιεί το block bitmap για να εντοπίσει ένα ελεύθερο block.

Το inode bitmap είναι ένας χάρτης που παρακολουθεί την κατάσταση όλων των inodes μέσα σε ένα block group. Κάθε bit στο inode bitmap αντιστοιχεί σε ένα inode του block group και είναι 1 αν το inode είναι κατειλημμένο (χρησιμοποιείται από ένα αρχείο ή κατάλογο), αλλίως 0. Όταν δημιουργείται ένα νέο αρχείο ή κατάλογος, το σύστημα βρίσκει ένα διαθέσιμο inode μέσω του inode bitmap.

Το block bitmap και το inode bitmap βρίσκονται στο block group όπου ανήκουν και είναι αποθηκευμένα σε προκαθορισμένα σημεία. Το block bitmap βρίσκεται μετά το superblock και τους block group descriptors. Το inode bitmap βρίσκεται αμέσως μετά το block bitmap.

---

### Ερώτηση 24: Τι είναι τα inode tables; Πού βρίσκονται μέσα στον δίσκο;
Ο Inode Table είναι ένας πίνακας που περιέχει όλα τα inodes ενός block group. Κάθε inode καταλαμβάνει ένα σταθερό μέγεθος (συνήθως 128 ή 256 bytes) και αποθηκεύει πληροφορίες για ένα μόνο αρχείο ή κατάλογο. Κάθε inode περιέχει το μέγεθος του αρχείου, τύπο αρχείου, δικαιώματα πρόσβασης, timestampts, και δείκτες στα μπλοκ δεδομένων που περιέχουν το περιεχόμενο του αρχείου.

Τα Inode Tables βρίσκονται σε κάθε block group του συστήματος αρχείων. Η θέση τους είναι σταθερή και ορίζεται μετά από τα bytes του Inode Bitmap.

---

### Ερώτηση 25: Τι πεδία περιέχει το κάθε inode; Πού αποθηκεύεται μέσα στον δίσκο;
Ένα inode (Index Node) στο σύστημα αρχείων ext2 είναι μια δομή δεδομένων που περιέχει τα μεταδεδομένα για ένα αρχείο ή κατάλογο. Το κάθε inode αντιπροσωπεύει ένα αρχείο ή κατάλογο και αποθηκεύει πληροφορίες για τη διαχείριση και την οργάνωση των δεδομένων στο σύστημα αρχείων. Τα inodes αποθηκεύονται στους πίνακες inodes (inode tables), οι οποίοι βρίσκονται μέσα σε κάθε block group.

Η δομή ενός inode μπορεί να παρουσιαστεί ως εξής:
```c
struct ext2_inode {
    uint16_t i_mode;        // Τύπος αρχείου και δικαιώματα
    uint16_t i_uid;         // User ID (ιδιοκτήτης)
    uint32_t i_size;        // Μέγεθος αρχείου σε bytes
    uint32_t i_atime;       // Χρόνος τελευταίας πρόσβασης
    uint32_t i_ctime;       // Χρόνος τελευταίας αλλαγής του inode
    uint32_t i_mtime;       // Χρόνος τελευταίας τροποποίησης
    uint32_t i_dtime;       // Χρόνος διαγραφής (αν διαγραφεί)
    uint16_t i_gid;         // Group ID (ιδιοκτήτης)
    uint16_t i_links_count; // Αριθμός σκληρών συνδέσμων
    uint32_t i_blocks;      // Αριθμός μπλοκ που καταλαμβάνει το αρχείο
    uint32_t i_flags;       // Σημαίες για χαρακτηριστικά αρχείου
    uint32_t i_direct[12];  // Άμεσοι δείκτες
    uint32_t i_indirect;    // Single indirect pointer
    uint32_t i_double_indirect; // Double indirect pointer
    uint32_t i_triple_indirect; // Triple indirect pointer
    uint32_t i_generation;  // Κωδικός γενιάς αρχείου
    uint32_t i_file_acl;    // Access Control List (ACL)
    uint32_t i_dir_acl;     // Directory ACL
    uint32_t i_faddr;       // Fragment address
    uint8_t i_osd1[12];     // Κρατημένα πεδία για λειτουργικό σύστημα
};
```

---

### Ερώτηση 26: Πόσα μπλοκ και πόσα inodes περιέχει το κάθε block group σε αυτό το σύστημα αρχείων;
#### Προσέγγιση: tools
Με χρήση `dumpe2fs` βλέπουμε ότι:
```bash
root@utopia:~# dumpe2fs /dev/vdb
...
Blocks per group:         8192
...
Inodes per group:         1832
...
```

#### Προσέγγιση: hexedit
Έχουμε ότι _Number of blocks in each block group_ αφορά τα bytes 32-35, και για τα inodes τα 40-43:
```bash
root@utopia:~# hexdump -C -s 1024 -n 36 /dev/vdb
00000400  18 32 00 00 00 c8 00 00  00 0a 00 00 90 c1 00 00  |.2..............|
00000410  0a 32 00 00 01 00 00 00  00 00 00 00 00 00 00 00  |.2..............|
00000420  00 20 00 00                                       |. ..|
00000424
```
Οπότε `00 20 00 00`, και σε δεκαδική μορφή: `8192`

```bash
root@utopia:~# hexdump -C -s 1024 -n 44 /dev/vdb
00000400  18 32 00 00 00 c8 00 00  00 0a 00 00 90 c1 00 00  |.2..............|
00000410  0a 32 00 00 01 00 00 00  00 00 00 00 00 00 00 00  |.2..............|
00000420  00 20 00 00 00 20 00 00  28 07 00 00              |. ... ..(...|
0000042c
```
Οπότε `28 07 00 00`, και σε δεκαδική μορφή: `1832`

---

### Ερώτηση 27: Σε ποιο inode αντιστοιχεί το αρχείο /dir2/helloworld σε αυτό το σύστημα αρχείων;
#### Προσέγγιση: tools
Κάνουμε mount το disk στο /mnt/fsdisk1
```bash
mkdir -p /mnt/fsdisk1
mount /dev/vdb /mnt/fsdisk1
```
Χρησιμοποιούμε την εντολή `ls -i`:
```bash
root@utopia:~# ls -i /mnt/fsdisk1/dir2/helloworld
9162 /mnt/fsdisk1/dir2/helloworld
```

#### Προσέγγιση: hexedit
Για να βρούμε το inode number του αρχείου, πρέπει να βρούμε πρώτα το inode number του directory.
Οπότε θα εντοπίσουμε το inode table του root και με βάση αυτό θα προχωρήσουμε.

Το inode table μπορύμε να το βρούμε (το offset του) μέσα στο block group descriptor, το οποίο βρίσκεται αμέσως μετά από το superblock.

Έχουμε ότι block_size=1024 bytes.

Οπότε διαβάζω το block descriptor (η αρχή του inode table εντοπίζεται στα bytes 9-12):
```bash
root@utopia:~# hexdump -C -s 2048 -n 12 /dev/vdb
00000800  03 00 00 00 04 00 00 00  05 00 00 00              |............|
0000080c
```
Οπότε έχουμε `05 00 00 00`, και σε δεκαδική μορφή `5`.
Το πολλαπλασιάζουμε με το block_size και παίρνουμε την διεύθυνση του inode table του root, δηλαδή `5120`.
Ύστερα θέλουμε το inode #2, οπότε θα προσθέσουμε `128` (inode size). Συνεπώς έχουμε `5248`

Τώρα θα εντοπίσουμε το block pointer 0 του inode table, ο οποίος βρίσκεται στα bytes 40-43
```bash
root@utopia:~# hexdump -C -s 5248 -n 44 /dev/vdb
00001480  ed 41 00 00 00 04 00 00  98 60 7c 67 e4 7a 78 65  |.A.......`|g.zxe|
00001490  e4 7a 78 65 00 00 00 00  00 00 05 00 02 00 00 00  |.zxe............|
000014a0  00 00 00 00 02 00 00 00  ea 00 00 00              |............|
000014ac
```
Και έχουμε `ea 00 00 00`, και σε δεκαδική μορφή `234`.
Οπότε πολλαπλασιάζουμε με το block_size και παίρνουμε το offset για το περιεχόμενο του inode #2: `239616`
```bash
root@utopia:~# hexdump -C -s 239616 -n 1024 /dev/vdb
0003a800  02 00 00 00 0c 00 01 00  2e 00 00 00 02 00 00 00  |................|
0003a810  0c 00 02 00 2e 2e 00 00  0b 00 00 00 14 00 0a 00  |................|
0003a820  6c 6f 73 74 2b 66 6f 75  6e 64 00 00 29 07 00 00  |lost+found..)...|
0003a830  0c 00 04 00 64 69 72 31  c9 23 00 00 c8 03 04 00  |....dir1.#......|
0003a840  64 69 72 32 00 00 00 00  00 00 00 00 00 00 00 00  |dir2............|
0003a850  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
0003ac00
```
Από τον πίνακα αυτόν βλέπουμε ότι το inode του dir2 είναι το `c9 23 00 00`, δηλαδή σε δεκαδική μορφή `9161`.

Έχοντας πλέον το inode number του /dir2 θα βρούμε σε ποιο block group βρίσκεται το αντίστοιχο inode table για να αποκτήσουμε πρόσβαση στα περιέχομαι του directory:
Θα χρησιμοποιήσουμε τον ακόλουθο τύπο:

$$
\text{Block Group} = \cfrac{\text{Inode Number} - 1}{\text{Inodes per Group}}
$$

Οπότε βγάζουμε ότι το block group θα είναι `5`. Δηλαδή το inode `9161` είναι το 1ο inode στο block group 5.

Θα βρούμε το inode table του block group 5:
```bash
root@utopia:~# hexdump -C -s $((2048 + 5*32)) -n 12 /dev/vdb
000008a0  03 a0 00 00 04 a0 00 00  05 a0 00 00              |............|
000008ac
```
Το οποίο είναι το `05 a0 00 00`, δηλαδή το `40965`. Οπότε θα διαβάσουμε το 1ο inode:
```bash
root@utopia:~# hexdump -C -s $((40965*1024)) -n 44 /dev/vdb
02801400  ed 41 00 00 00 04 00 00  9c 60 7c 67 e4 7a 78 65  |.A.......`|g.zxe|
02801410  e4 7a 78 65 00 00 00 00  00 00 02 00 02 00 00 00  |.zxe............|
02801420  00 00 00 00 02 00 00 00  f7 00 00 00              |............|
0280142c
```
Το οποίο είναι το `f7 00 00 00`, δηλαδή `247`.

Οπότε βρήκαμε το block για το directory entry του /dir2:
```bash
root@utopia:~# hexdump -C -s $((247*1024)) -n 64 /dev/vdb
0003dc00  c9 23 00 00 0c 00 01 00  2e 00 00 00 02 00 00 00  |.#..............|
0003dc10  0c 00 02 00 2e 2e 00 00  ca 23 00 00 e8 03 0a 00  |.........#......|
0003dc20  68 65 6c 6c 6f 77 6f 72  6c 64 00 00 00 00 00 00  |helloworld......|
0003dc30  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
0003dc40
```
Και όπως παρατηρούμε το inode του αρχείου `helloworld` είναι το `ca 23 00 00`.
Και σε δεκαδική μορφή: `9162`.

---

### Ερώτηση 28: Σε ποιο block group αντιστοιχεί αυτό το inode;
#### Προσέγγιση: tools
Από το [man page](https://linux.die.net/man/8/debugfs) της `debugfs`, έχουμε: _The debugfs program is an interactive file system debugger. It can be used to examine and change the state of an ext2, ext3, or ext4 file system._

Οπότε εφαρμόζοντάς την έχουμε:
```bash
root@utopia:~# debugfs /dev/vdb
debugfs:
```
Θα δούμε ότι το αρχείο πράγματι υπάρχει:
```bash
debugfs:  stat <9162>
Inode: 9162   Type: regular    Mode:  0644   Flags: 0x0
Generation: 2739270588    Version: 0x00000001
User:     0   Group:     0   Size: 42
File ACL: 0
Links: 1   Blockcount: 2
Fragment:  Address: 0    Number: 0    Size: 0
ctime: 0x65787ae4 -- Tue Dec 12 15:23:16 2023
atime: 0x677d3407 -- Tue Jan  7 14:02:47 2025
mtime: 0x65787ae4 -- Tue Dec 12 15:23:16 2023
BLOCKS:
(0):1025
TOTAL: 1
```
Και για να δούμε το block group:
```bash
...
debugfs:  show_super_stats
 Group  0: block bitmap at 3, inode bitmap at 4, inode table at 5
           7944 free blocks, 1821 free inodes, 2 used directories
 Group  1: block bitmap at 8195, inode bitmap at 8196, inode table at 8197
           7958 free blocks, 1831 free inodes, 1 used directory
 Group  2: block bitmap at 16387, inode bitmap at 16388, inode table at 16389
           7959 free blocks, 1832 free inodes, 0 used directories
 Group  3: block bitmap at 24579, inode bitmap at 24580, inode table at 24581
           7959 free blocks, 1832 free inodes, 0 used directories
 Group  4: block bitmap at 32771, inode bitmap at 32772, inode table at 32773
           7959 free blocks, 1832 free inodes, 0 used directories
 Group  5: block bitmap at 40963, inode bitmap at 40964, inode table at 40965
           7959 free blocks, 1830 free inodes, 1 used directory
 Group  6: block bitmap at 49155, inode bitmap at 49156, inode table at 49157
           1814 free blocks, 1832 free inodes, 0 used directories
...
```
Δηλαδή το inode 9162 βρίσκεται ανάμεσα στα inode tables των Group 1, και Group 2. Οπότε ανοίκει στο group 1.

#### Προσέγγιση: hexedit
Το inode 9162 είναι το δεύτερο inode του block group 5, οπότε διαβάζοντας το (inode table of bg 5):
```bash
root@utopia:~# hexdump -C -s $((40965*1024 + 128)) -n 44 /dev/vdb
02801480  a4 81 00 00 2a 00 00 00  07 34 7d 67 e4 7a 78 65  |....*....4}g.zxe|
02801490  e4 7a 78 65 00 00 00 00  00 00 01 00 02 00 00 00  |.zxe............|
028014a0  00 00 00 00 01 00 00 00  01 04 00 00              |............|
028014ac
```
Εχουμε `01 04 00 00`, σε δεκαδική `1025`, και για να βρούμε το block group, έχουμε:

$$
\text{Block Group} = \cfrac{\text{Inode Number} - 1}{\text{Inodes per Group}}
$$

Δηλαδή στο 1ο block group.

---

### Ερώτηση 29: Σε ποιο μπλοκ του δίσκου υπάρχει το inode table που περιέχει το παραπάνω inode;
#### Προσέγγιση: tools
Κάθε block group έχει το δικό του inode table. Βρήκαμε ότι το παραπάνω inode βρίσκεται στο block group 1,
οπότε θα κοιτάξω με την εντολή `dumpe2fs` σε ποιο μπλοκ του δίσκου υπάρχει το αντίστοιχο inode table:
```bash
root@utopia:~# dumpe2fs /dev/vdb
...
Group 1: (Blocks 8193-16384)
  ...
  Inode table at 8197-8425 (+4)
...
```

#### Προσέγγιση: hexedit
Μέσα από το block descriptor του block group 1 μπορώ να βρω την θέση που αρχίζει το inode table:
```bash
root@utopia:~# hexdump -C -s $((2048 + 32)) -n 12 /dev/vdb
00000820  03 20 00 00 04 20 00 00  05 20 00 00              |. ... ... ..|
0000082c
```
Οπότε έχουμε `05 20 00 00`, και σε δεκαδική μορφή: `8197`.

---

### Ερώτηση 30: Δείξτε όλα τα πεδία αυτού του inode [128 bytes].
#### Προσέγγιση: tools
Θα χρησιμοποιήσουμε, πάλι `debugfs`:
```bash
root@utopia:~# debugfs /dev/vdb
debugfs:  stat <9162>
Inode: 9162   Type: regular    Mode:  0644   Flags: 0x0
Generation: 2739270588    Version: 0x00000001
User:     0   Group:     0   Size: 42
File ACL: 0
Links: 1   Blockcount: 2
Fragment:  Address: 0    Number: 0    Size: 0
ctime: 0x65787ae4 -- Tue Dec 12 15:23:16 2023
atime: 0x677d3407 -- Tue Jan  7 14:02:47 2025
mtime: 0x65787ae4 -- Tue Dec 12 15:23:16 2023
BLOCKS:
(0):1025
TOTAL: 1
```

#### Προσέγγιση: hexedit
Το inode 9162 είναι το δεύτερο inode του block group 5, οπότε διαβάζοντας το:
```bash
root@utopia:~# hexdump -C -s $((40965*1024 + 128)) -n 128 /dev/vdb
02801480  a4 81 00 00 2a 00 00 00  07 34 7d 67 e4 7a 78 65  |....*....4}g.zxe|
02801490  e4 7a 78 65 00 00 00 00  00 00 01 00 02 00 00 00  |.zxe............|
028014a0  00 00 00 00 01 00 00 00  01 04 00 00 00 00 00 00  |................|
028014b0  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
028014e0  00 00 00 00 bc f3 45 a3  00 00 00 00 00 00 00 00  |......E.........|
028014f0  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
02801500
```

---


### Ερώτηση 31: Σε ποιο μπλοκ είναι αποθηκευμένα τα δεδομένα αυτού του αρχείου;
#### Προσέγγιση: tools
Χρησιμοποιούμε την `filefrag` για να δούμε σε ποια blocks ειναι σκοπρισμένα τα δεδομένα του αρχείου:
```bash
root@utopia:~# filefrag -e /mnt/fsdisk1/dir2/helloworld
Filesystem type is: ef53
Filesystem cylinder groups approximately 7
File size of /mnt/fsdisk1/dir2/helloworld is 42 (1 block of 1024 bytes)
 ext:     logical_offset:        physical_offset: length:   expected: flags:
   0:        0..       0:       1025..      1025:      1:             last,merged,eof
/mnt/fsdisk1/dir2/helloworld: 1 extent found
```
βλέπουμε ότι βρίσκονται στο block group 1, όπως εξάλλου περιμένουμε.

#### Προσέγγιση: hexedit
Έχουμε βρει ότι το inode table του αρχείου είναι το δεύτερο inode του 1ου block group.
Στο ίδιο μλποκ θα είναι αποθηκευμένα τα δεδομένα του:
```bash
root@utopia:~# hexdump -C -s $((1025*1024)) -n 128 /dev/vdb
00100400  57 65 6c 63 6f 6d 65 20  74 6f 20 74 68 65 20 4d  |Welcome to the M|
00100410  69 67 68 74 79 20 57 6f  72 6c 64 20 6f 66 20 46  |ighty World of F|
00100420  69 6c 65 73 79 73 74 65  6d 73 00 00 00 00 00 00  |ilesystems......|
00100430  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
00100480
```

---


### Ερώτηση 32: Τι μέγεθος έχει αυτό το αρχείο;
#### Προσέγγιση: tools
Με την απλή εντολή `ls -la` μπορούμε να βρούμε το μέγεθος του αρχείου:
```bash
root@utopia:~# ls -la /mnt/fsdisk1/dir2/helloworld
-rw-r--r-- 1 root root 42 Dec 12  2023 /mnt/fsdisk1/dir2/helloworld
```
Αρα είναι 42 bytes

#### Προσέγγιση: hexedit
Μέσω hexdump θα βρω το μεγεθος απο το inode table στις θέσεις 4-7:
```bash
root@utopia:~# hexdump -C -s $((40965*1024 + 128)) -n 8 /dev/vdb
02801480  a4 81 00 00 2a 00 00 00                           |....*...|
02801488
```
Άρα είνα `2a 00 00 00`, και σε δεκαδική μορφή: `42`.

---

### Ερώτηση 33: Δείξτε τα περιεχόμενα αυτού του αρχείου.
#### Προσέγγιση: tools
Θα χρησιμοποιήσω την cat:
```bash
root@utopia:~# cat /mnt/fsdisk1/dir2/helloworld
Welcome to the Mighty World of Filesystemsroot@utopia:~#
```

#### Προσέγγιση: hexedit
Όπως έχουμε δει νωρίτερα το block vector που αντιστοιχει στο inode του αρχειο βρισκεται στο block `1025`, οπότε με hexdump:
```bash
00100400  57 65 6c 63 6f 6d 65 20  74 6f 20 74 68 65 20 4d  |Welcome to the M|
00100410  69 67 68 74 79 20 57 6f  72 6c 64 20 6f 66 20 46  |ighty World of F|
00100420  69 6c 65 73 79 73 74 65  6d 73 00 00 00 00 00 00  |ilesystems......|
00100430  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
00100480
```

---

## Εικόνα fsdisk2.img
### Ερώτηση 1: Συνδέστε την εικόνα του δίσκου στην εικονική μηχανή σας, όπως κάνατε και για την εικόνα fsdisk1.img και προσαρτήστε τη στον κατάλογο /mnt.
Βλέπουμε ότι η εικόνα αντιστοιχεί στην συσκευή `/dev/vdc`:
```bash
root@utopia:~# hexdump -s 1024 -n 1024 -C /dev/vdc
00000400  10 14 00 00 00 50 00 00  00 00 00 00 63 4c 00 00  |.....P......cL..|
00000410  00 00 00 00 01 00 00 00  00 00 00 00 00 00 00 00  |................|
00000420  00 20 00 00 00 20 00 00  b0 06 00 00 e5 7a 78 65  |. ... .......zxe|
00000430  e9 7a 78 65 01 00 ff ff  53 ef 01 00 01 00 00 00  |.zxe....S.......|
00000440  e5 7a 78 65 00 00 00 00  00 00 00 00 01 00 00 00  |.zxe............|
00000450  00 00 00 00 0b 00 00 00  80 00 00 00 00 00 00 00  |................|
00000460  00 00 00 00 00 00 00 00  d1 26 6a d1 da e1 42 75  |.........&j...Bu|
00000470  81 36 a2 9a 4d fc 9d 1f  66 73 64 69 73 6b 32 2e  |.6..M...fsdisk2.|
00000480  69 6d 67 00 00 00 00 00  2f 63 73 6c 61 62 2d 62  |img...../cslab-b|
00000490  75 6e 6b 65 72 00 00 00  00 00 00 00 00 00 00 00  |unker...........|
000004a0  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
000004e0  00 00 00 00 00 00 00 00  00 00 00 00 e0 1a 14 38  |...............8|
000004f0  a9 b7 42 18 b8 92 e0 e1  ae 23 d8 91 01 00 00 00  |..B......#......|
00000500  0c 00 00 00 00 00 00 00  e5 7a 78 65 00 00 00 00  |.........zxe....|
00000510  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
00000560  01 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
00000570  00 00 00 00 00 00 00 00  93 03 00 00 00 00 00 00  |................|
00000580  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
00000800
```

Οπότε κάνω mount την συσκευή αυτή στο `/mnt/fsdisk2`
```bash
mkdir -p /mnt/fsdisk2
mount /dev/vdb /mnt/fsdisk2
```

---

### Ερώτηση 2: Χρησιμοποιήστε την εντολή touch για να δημιουργήσετε ένα νέο κενό αρχείο /file1 μέσα στο συγκεκριμένο σύστημα αρχείων. Βεβαιωθείτε ότι η εντολή σας αναφέρεται πράγματι στο συγκεκριμένο σύστημα αρχείων [σε ποιον κατάλογο το έχετε προσαρτήσει;], κι όχι στον ριζικό κατάλογο του συστήματος.
Εκτελούμε την ακόλουθη εντολή:
```bash
root@utopia:~# touch /mnt/fsdisk2/test_file
touch: cannot touch '/mnt/fsdisk2/test_file': No space left on device
```

---

### Ερώτηση 3: Πέτυχε η εντολή; Αν όχι, τι πρόβλημα υπήρξε;
Από ότι βλέπουμε το αρχείο δεν δημιθουργήθηκε, μιας και το filesystem φαινεται να ειναι γεματο.

---

### Ερώτηση 4: Ποια κλήση συστήματος προσπάθησε να τρέξει η touch, και με ποιον κωδικό λάθους απέτυχε; Υποστηρίξτε την απάντησή σας με χρήση της εντολής strace.
Παρατηρώντας το `strace`, έχουμε:
```bash
root@utopia:~# strace touch /mnt/fsdisk2/test_file
...
openat(AT_FDCWD, "/mnt/fsdisk2/test_file", O_WRONLY|O_CREAT|O_NOCTTY|O_NONBLOCK, 0666) = -1 ENOSPC (No space left on device)
...
```
Δηλαδή η `touch` χρησιμοποιεί την [openat](https://linux.die.net/man/2/openat) (Given a pathname for a file, open() returns a file descriptor, a small, nonnegative integer for use in subsequent system calls (read(2), write(2), lseek(2), fcntl(2), etc.)). Όμως το δίκσος είναι γεμάτος και δεν έχει διαθέσιμο χώρο για να επιστρέψει έναν file discriptor.

---

### Ερώτηση 5: Πόσα αρχεία και πόσους καταλόγους περιέχει το συγκεκριμένο σύστημα αρχείων;
#### Προσέγγιση: tools
Χρησιμοποιούμε την `find`, οπου από το [man page](https://linux.die.net/man/1/find) έχουμε: 
_find searches the directory tree rooted at each given file name by evaluating the given expression from left to right, according to the rules of precedence (see section OPERATORS), until the outcome is known (the left hand side is false for and operations, true for or), at which point find moves on to the next file name._

Για να βρούμε το σύνολο των αρχείων:
```bash
root@utopia:~# find /mnt/fsdisk2 -type f | wc -l
4868
```
Και για τους καταλόγους:
```bash
root@utopia:~# find /mnt/fsdisk2 -type d | wc -l
259
```

#### Προσέγγιση: hexedit
Επαληθεύουμε ότι το device είναι γεμάτο:
```bash
root@utopia:~# hexdump -C -s 1024 -n 20 /dev/vdc
00000400  10 14 00 00 00 50 00 00  00 00 00 00 63 4c 00 00  |.....P......cL..|
00000410  00 00 00 00                                       |....|
00000414
```
Και για να βρω τα συνολικά inodes του συστήματος θα κοιτάξω στα bytes 0-3 του superblock:
```bash
root@utopia:~# hexdump -C -s 1024 -n 4 /dev/vdc
00000400  10 14 00 00                                       |....|
00000404
```
Άρα έχει συνολικά `10 14 00 00` inodes, και σε δεκαδική μορφή: `5136`

---

### Ερώτηση 6: Πόσο χώρο καταλαμβάνουν τα δεδομένα και τα μεταδεδομένα του συγκεκριμένου συστήματος αρχείων;
#### Προσέγγιση: tools
Θα χρησιμοποιήσουμε dumpe2fs για να δούμε το πόσα blocks είναι assigned και θα το πολλαπλασιάσουμε με το blocksize.
```bash
root@utopia:~# dumpe2fs -h /dev/vdc
dumpe2fs 1.47.0 (5-Feb-2023)
Filesystem volume name:   fsdisk2.img
Last mounted on:          /cslab-bunker
Filesystem UUID:          d1266ad1-dae1-4275-8136-a29a4dfc9d1f
Filesystem magic number:  0xEF53
Filesystem revision #:    1 (dynamic)
Filesystem features:      (none)
Filesystem flags:         signed_directory_hash
Default mount options:    user_xattr acl
Filesystem state:         not clean
Errors behavior:          Continue
Filesystem OS type:       Linux
Inode count:              5136
Block count:              20480
Reserved block count:     0
Overhead clusters:        655
Free blocks:              19555
Free inodes:              0
First block:              1
Block size:               1024
Fragment size:            1024
Blocks per group:         8192
Fragments per group:      8192
Inodes per group:         1712
Inode blocks per group:   214
Filesystem created:       Tue Dec 12 15:23:17 2023
Last mount time:          Wed Jan  8 18:19:24 2025
Last write time:          Wed Jan  8 18:19:24 2025
Mount count:              3
Maximum mount count:      -1
Last checked:             Tue Dec 12 15:23:17 2023
Check interval:           0 (<none>)
Lifetime writes:          919 kB
Reserved blocks uid:      0 (user root)
Reserved blocks gid:      0 (group root)
First inode:              11
Inode size:               128
Default directory hash:   half_md4
Directory Hash Seed:      e01a1438-a9b7-4218-b892-e0e1ae23d891
```

Για τα data size: (blocks_count - free_blocks_count) * block_size = (20480 - 19555) * 1024 = 925 * 1024 = 947200 Bytes = 947.2 KB = 0.9033203125 MB

#### Προσέγγιση: hexedit

Θέλουμε να βρούμε τα:
s_inodes_count
s_blocks_count	
s_free_blocks_count
s_free_inodes_count	
s_log_block_size	
s_inode_size

```bash
root@utopia:~# hexdump -C -s 1024 -n 1024 /dev/vdc
00000400  10 14 00 00 00 50 00 00  00 00 00 00 63 4c 00 00  |.....P......cL..|
00000410  00 00 00 00 01 00 00 00  00 00 00 00 00 00 00 00  |................|
00000420  00 20 00 00 00 20 00 00  b0 06 00 00 b6 56 7d 67  |. ... .......V}g|
00000430  d3 5c 7d 67 02 00 ff ff  53 ef 01 00 01 00 00 00  |.\}g....S.......|
00000440  e5 7a 78 65 00 00 00 00  00 00 00 00 01 00 00 00  |.zxe............|
00000450  00 00 00 00 0b 00 00 00  80 00 00 00 00 00 00 00  |................|
00000460  00 00 00 00 00 00 00 00  d1 26 6a d1 da e1 42 75  |.........&j...Bu|
00000470  81 36 a2 9a 4d fc 9d 1f  66 73 64 69 73 6b 32 2e  |.6..M...fsdisk2.|
00000480  69 6d 67 00 00 00 00 00  2f 63 73 6c 61 62 2d 62  |img...../cslab-b|
00000490  75 6e 6b 65 72 00 00 00  00 00 00 00 00 00 00 00  |unker...........|
000004a0  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
000004e0  00 00 00 00 00 00 00 00  00 00 00 00 e0 1a 14 38  |...............8|
000004f0  a9 b7 42 18 b8 92 e0 e1  ae 23 d8 91 01 00 00 00  |..B......#......|
00000500  0c 00 00 00 00 00 00 00  e5 7a 78 65 00 00 00 00  |.........zxe....|
00000510  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
00000560  01 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
00000570  00 00 00 00 00 00 00 00  97 03 00 00 00 00 00 00  |................|
00000580  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
00000640  00 00 00 00 00 00 00 00  8f 02 00 00 00 00 00 00  |................|
00000650  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
00000800
root@utopia:~#
```

```bash
Offset (bytes)	Size (bytes)	Description
0	4	s_inodes_count
4	4	s_blocks_count
8	4	s_r_blocks_count
12	4	s_free_blocks_count
16	4	s_free_inodes_count
20	4	s_first_data_block
24	4	s_log_block_size
28	4	s_log_frag_size
32	4	s_blocks_per_group
36	4	s_frags_per_group
40	4	s_inodes_per_group
44	4	s_mtime
48	4	s_wtime
52	2	s_mnt_count
54	2	s_max_mnt_count
56	2	s_magic
58	2	s_state
60	2	s_errors
62	2	s_minor_rev_level
64	4	s_lastcheck
68	4	s_checkinterval
72	4	s_creator_os
76	4	s_rev_level
80	2	s_def_resuid
82	2	s_def_resgid
-- EXT2_DYNAMIC_REV Specific --
84	4	s_first_ino
88	2	s_inode_size
```

s_inodes_count = 5136
s_blocks_count = 20480
s_free_blocks_count = 19555
s_free_inodes_count = 0	  
s_log_block_size = 0
s_inode_size = 128

block_size = 1024bytes

Για τα data size: (blocks_count - free_blocks_count) * block_size = (20480 - 19555) * 1024 = 925 * 1024 = 947200 Bytes = 947.2 KB = 0.9033203125 MB

---

### Ερώτηση 7: Πόσο είναι το μέγεθος του συγκεκριμένου συστήματος αρχείων;
#### Προσέγγιση: tools
Από την `df` βλέπουμε ότι είναι 20 mega bytes.

#### Προσέγγιση: hexedit

Block_Count * Block_size = 20480 * 1024 =
20971520 Bytes = 20971.52 KB = 20MB MB

---

### Ερώτηση 8: Πόσα μπλοκ είναι διαθέσιμα/ελεύθερα στο συγκεκριμένο σύστημα αρχείων; Ισοδύναμα, έχει ελεύθερο χώρο το συγκεκριμένο σύστημα αρχείων;
#### Προσέγγιση: tools
Χρησιμοποιούμε την εντολή `dumpe2fs`:
```bash
root@utopia:~# dumpe2fs /dev/vdc
...
Free blocks:              19555
...
```

#### Προσέγγιση: hexedit
Για να βρω τα unallocated block θα κοιτάξω στις θέσεις 12-15 του hexdump:
```bash
root@utopia:~# hexdump -C -s 1024 -n 16 /dev/vdc
00000400  10 14 00 00 00 50 00 00  00 00 00 00 63 4c 00 00  |.....P......cL..|
00000410
```
Οπότε έχουμε `63 4c 00 00`, που σε δεκαδική μορφή: `19555`

---

### Ερώτηση 9: Αφού υπάρχουν διαθέσιμα μπλοκ, τι σας αποτρέπει από το να δημιουργήσετε νέο αρχείο;
Κάθε αρχείο ή κατάλογος σε ένα σύστημα αρχείων ext2 απαιτεί ένα inode. Αν τα inodes έχουν εξαντληθεί.

#### Προσέγγιση: tools
```bash
root@utopia:~# df -i /mnt/fsdisk2
Filesystem     Inodes IUsed IFree IUse% Mounted on
/dev/vdc         5136  5136     0  100% /mnt/fsdisk2
```

#### Προσέγγιση: hexedit
```bash
root@utopia:~# hexdump -C -s 1024 -n 20 /dev/vdc
00000400  10 14 00 00 00 50 00 00  00 00 00 00 63 4c 00 00  |.....P......cL..|
00000410  00 00 00 00                                       |....|
00000414
```

---

## Εικόνα fsdisk3.img
Αρχικά βρίσκουμε σε πιο device αντιστοιχεί το fsdisk3.img:
```bash
root@utopia:/# hexdump -C -s 1024 -n 1024 /dev/vdd
00000400  10 14 00 00 00 50 00 00  00 04 00 00 32 39 38 37  |.....P......2987|
00000410  f9 13 00 00 01 00 00 00  00 00 00 00 00 00 00 00  |................|
00000420  00 20 00 00 00 20 00 00  b0 06 00 00 e9 7a 78 65  |. ... .......zxe|
00000430  ea 7a 78 65 01 00 ff ff  53 ef 01 00 01 00 00 00  |.zxe....S.......|
00000440  e9 7a 78 65 00 00 00 00  00 00 00 00 01 00 00 00  |.zxe............|
00000450  00 00 00 00 0b 00 00 00  80 00 00 00 00 00 00 00  |................|
00000460  00 00 00 00 00 00 00 00  19 03 21 43 52 ce 49 17  |..........!CR.I.|
00000470  8e c5 99 1c 89 ee 42 1b  66 73 64 69 73 6b 33 2e  |......B.fsdisk3.|
00000480  69 6d 67 00 00 00 00 00  2f 63 73 6c 61 62 2d 62  |img...../cslab-b|
00000490  75 6e 6b 65 72 00 00 00  00 00 00 00 00 00 00 00  |unker...........|
000004a0  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
000004e0  00 00 00 00 00 00 00 00  00 00 00 00 b1 b5 d2 0e  |................|
000004f0  a7 8a 4e 76 93 48 d3 97  0e b6 92 68 01 00 00 00  |..Nv.H.....h....|
00000500  0c 00 00 00 00 00 00 00  e9 7a 78 65 00 00 00 00  |.........zxe....|
00000510  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
00000560  01 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
00000570  00 00 00 00 00 00 00 00  1d 00 00 00 00 00 00 00  |................|
00000580  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
00000800
```
Οπότε έχουμε την `/dev/vdd`.

Eπιβεβαιώnoyme το περιεχόμενο της εικόνας:
```bash
root@utopia:/# sha256sum /home/user/shared/ext2-vdisks/fsdisk3-982902777.img
982902777d0e66e14379f642365b4fa71a5473348d9af2453e80dbea135bb50d  /home/user/shared/ext2-vdisks/fsdisk3-982902777.img
```
```bash
root@utopia:/# sha256sum /dev/vdd
982902777d0e66e14379f642365b4fa71a5473348d9af2453e80dbea135bb50d  /dev/vdd
```
Το hash είναι το ίδιο, οπότε προχωράμε κανονικά στην άσκηση.

### Ερώτηση 1: Ποιο εργαλείο στο Linux αναλαμβάνει τον έλεγχο ενός συστήματος αρχείων ext2 για αλλοιώσεις;
Το εργαλείο που αναλαμβάνει τον έλεγχο ενός συστήματος αρχείων ext2 για αλλοιώσεις στο Linux είναι το e2fsck.
Σύμφωνα με το [man-page](https://linux.die.net/man/8/e2fsck): _e2fsck is used to check the ext2/ext3/ext4 family of file systems_

Το εργαλείο αυτό μπορεί να ανιχνεύσει και να διορθώσει διάφορες μορφές αλλοιώσεων, όπως aσυμφωνίες στον πίνακα inodes, kατεστραμμένα blocks, κα.

---

### Ερώτηση 2: Ποιοι παράγοντες θα μπορούσαν δυνητικά να οδηγήσουν σε αλλοιώσεις στο σύστημα αρχείων; Αναφέρετε ενδεικτικά δέκα πιθανές αλλοιώσεις.
Οι αλλοιώσεις στο σύστημα αρχείων μπορούν να προκύψουν από διάφορους παράγοντες, οι οποίοι επηρεάζουν τη λειτουργία και την ακεραιότητα των δεδομένων. Μερικοί από αυτούς είναι:
* Απροσδόκητη διακοπή ρεύματος
* Ελαττωματικός δίσκος (Hardware Failure)
* Λάθος στον Kernel ή στο Filesystem Driver
* Ιοί και Κακόβουλο Λογισμικό
* Φθαρμένα Blocks
* Αποσύνδεση δίσκου κατά τη λειτουργεία χωρίς unmount


### Ερώτηση 3: Τρέξτε το εργαλείο αυτό και επιδιορθώστε το σύστημα αρχείων. Αναφέρετε όλες τις αλλοιώσεις που εντοπίσατε, εξαντλητικά.
Εφαρμόζοντας το εργαλείο έχουμε:
```bash
root@utopia:/# e2fsck -n /dev/vdd
e2fsck 1.47.0 (5-Feb-2023)
fsdisk3.img contains a file system with errors, check forced.
Pass 1: Checking inodes, blocks, and sizes
Pass 2: Checking directory structure
First entry 'BOO' (inode=1717) in directory inode 1717 (/dir-2) should be '.'
Fix? no

Pass 3: Checking directory connectivity
Pass 4: Checking reference counts
Inode 3425 ref count is 1, should be 2.  Fix? no

Pass 5: Checking group summary information
Block bitmap differences:  +34
Fix? no

Free blocks count wrong for group #0 (7960, counted=7961).
Fix? no

Free blocks count wrong (926431538, counted=19801).
Fix? no


fsdisk3.img: ********** WARNING: Filesystem still has errors **********

fsdisk3.img: 23/5136 files (0.0% non-contiguous), 18446744072783140558/20480 blocks
```
Οπότε παρατηρώ τα σφάλματα:
1. First entry 'BOO' (inode=1717) in directory inode 1717 (/dir-2) should be '.'
2. Inode 3425 ref count is 1, should be 2.
3. Block bitmap differences: +34
4. Free blocks count wrong for group #0 (7960, counted=7961).
5. Free blocks count wrong (926431538, counted=19801).

Για να επιδιορθώσει αυτόματα το εργαλείο τα παραπάνω προβλήματα τρέχουμε:
```bash
root@utopia:/# e2fsck -y /dev/vdd
e2fsck 1.47.0 (5-Feb-2023)
fsdisk3.img contains a file system with errors, check forced.
Pass 1: Checking inodes, blocks, and sizes
Pass 2: Checking directory structure
First entry 'BOO' (inode=1717) in directory inode 1717 (/dir-2) should be '.'
Fix? yes

Pass 3: Checking directory connectivity
Pass 4: Checking reference counts
Inode 3425 ref count is 1, should be 2.  Fix? yes

Pass 5: Checking group summary information
Block bitmap differences:  +34
Fix? yes

Free blocks count wrong (926431538, counted=19800).
Fix? yes


fsdisk3.img: ***** FILE SYSTEM WAS MODIFIED *****
fsdisk3.img: 23/5136 files (0.0% non-contiguous), 680/20480 blocks
```

Και επαληθεύοντας ότι τα προβλήματα, αυτά έχουν λυθεί:
```bash
root@utopia:/# e2fsck -n /dev/vdd
e2fsck 1.47.0 (5-Feb-2023)
fsdisk3.img: clean, 23/5136 files, 680/20480 blocks
```

---

### Ερώτηση 4: Επαναφέρετε το δίσκο στην πρότερή του κατάσταση, από την αρχική εικόνα. Εντοπίστε τις αλλοιώσεις με χρήση της μεθόδου hexedit.
Έχουμε αποθηκεύσει το backup στο `/home/user/shared/ext2-vdisks/fsdisk3-982902777-backup.img`, οπότε επαναφέρουμε το filesystem στην κατάσταση με τα λάθη:
```bash
root@utopia:/# cp /home/user/shared/ext2-vdisks/fsdisk3-982902777-backup.img /home/user/shared/ext2-vdisks/fsdisk3-982902777.img
```

Επιβεβαιώνουμε ότι το ανακτημένο αρχείο είναι ίδιο με το αρχικό:
```bash
root@utopia:/# sha256sum /home/user/shared/ext2-vdisks/fsdisk3-982902777.img
982902777d0e66e14379f642365b4fa71a5473348d9af2453e80dbea135bb50d  /home/user/shared/ext2-vdisks/fsdisk3-982902777.img
```
```bash
root@utopia:/# sha256sum /dev/vdd
982902777d0e66e14379f642365b4fa71a5473348d9af2453e80dbea135bb50d  /dev/vdd
```

Θα βρούμε το: **First entry 'BOO' (inode=1717) in directory inode 1717 (/dir-2) should be '.'**

Θα βρω πόσα inodes έχει κάθε block:
```bash
root@utopia:/# hexdump -C -s 1024 -n 44 /dev/vdd
00000400  10 14 00 00 00 50 00 00  00 04 00 00 32 39 38 37  |.....P......2987|
00000410  f9 13 00 00 01 00 00 00  00 00 00 00 00 00 00 00  |................|
00000420  00 20 00 00 00 20 00 00  b0 06 00 00              |. ... ......|
0000042c
```
Άρα `b0 06 00 00`, και σε δεκαδική μορφή: `1712`

Το inode αυτό ανοίκει στο group ceil((1717 - 1) / 1712) = `1`, και στο offset (1717 - 1) mod 1712 = `4`.

Οπότε θα βρούμε το inode table του group 1:
```bash
root@utopia:/# hexdump -C -s $((2048 + 1*32)) -n 12 /dev/vdd
00000820  03 20 00 00 04 20 00 00  05 20 00 00              |. ... ... ..|
0000082c
```
So `05 20 00 00`, `8197`

Οπότε θα διαβάσουμε το 4ο inode (Direct Block Pointer 0):
```bash
root@utopia:/# hexdump -C -s $((8197*1024 + 4*128)) -n 44 /dev/vdd
00801600  ed 41 00 00 00 04 00 00  e9 7a 78 65 e9 7a 78 65  |.A.......zxe.zxe|
00801610  e9 7a 78 65 00 00 00 00  00 00 02 00 02 00 00 00  |.zxe............|
00801620  00 00 00 00 04 00 00 00  dc 20 00 00              |......... ..|
0080162c
```
So `dc 20 00 00 `, `8412`

Οπότε βρήκαμε το block για το directory entry:
```bash
root@utopia:/# hexdump -C -s $((8412*1024)) -n 64 /dev/vdd
00837000  b5 06 00 00 0c 00 03 00  42 4f 4f 00 02 00 00 00  |........BOO.....|
00837010  0c 00 02 00 2e 2e 00 00  b6 06 00 00 10 00 06 00  |................|
00837020  66 69 6c 65 2d 31 00 00  b7 06 00 00 10 00 06 00  |file-1..........|
00837030  66 69 6c 65 2d 32 00 00  b8 06 00 00 c8 03 06 00  |file-2..........|
00837040
```
Παρατηρώ ότι στην πρώτη θέση έχω το όνομα: `42 4f 4f`, το οποίο συμαίνει: `BOO`, και όχι το `.`.

---

Προχωράμε στο επόμενο: **Inode 3425 ref count is 1, should be 2.**

Ομοίως με πριν θα εντοπίσουμε πόσα hard links έχει το inode 3425. Αρχικά ανοίκει στο 2 block με offset 0.

Το inode table του block 2:
```bash
root@utopia:/# hexdump -C -s $((2048 + 2*32)) -n 12 /dev/vdd
00000840  03 40 00 00 04 40 00 00  05 40 00 00              |.@...@...@..|
0000084c
```
Άρα έχουμε το `05 40 00 00`, δηλαδή το: `16389`

Για τα hard links του inode έχουμε:
```bash
root@utopia:/# hexdump -C -s $((16389*1024)) -n 28 /dev/vdd
01001400  ed 41 00 00 00 04 00 00  e9 7a 78 65 e9 7a 78 65  |.A.......zxe.zxe|
01001410  e9 7a 78 65 00 00 00 00  00 00 01 00              |.zxe........|
0100141c
```
Άρα `01 00`, δηλαδή `1`.

---

Θα δούμε το: **Block bitmap differences: +34**

Αρχικά χρειαζόμαστε τον αριθμό των blocks per group:
```bash
root@utopia:/# hexdump -C -s 1024 -n 36 /dev/vdd
00000400  10 14 00 00 00 50 00 00  00 04 00 00 40 2e 00 00  |.....P......@...|
00000410  f9 13 00 00 01 00 00 00  00 00 00 00 00 00 00 00  |................|
00000420  00 20 00 00                                       |. ..|
00000424
```
Είναι `00 20 00 00`, δηλαδή `8192`. Οπότε το block 34 βρίσκεται στο block #0.

Θα πάρουμε το block bitmap από τον block discriptor για το group 0:
```bash
root@utopia:/# hexdump -C -s 2048 -n 4 /dev/vdd
00000800  03 00 00 00                                       |....|
00000804
```
Το οποίο είναι το `3`. Και διαβάζοντας το:
```bash
root@utopia:/# hexdump -C -s $((3*1024)) -n 64 /dev/vdd
00000c00  ff ff ff ff fd ff ff ff  ff ff ff ff ff ff ff ff  |................|
00000c10  ff ff ff ff ff ff ff ff  ff ff ff ff ff 00 00 00  |................|
00000c20  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
00000c40
```
Κάθε hex byte αποθηκεύει 8 bits, οπότε το #34 βρίσκεται στο byte 5 (34/8), το οποίο είναι το `fd` δηλαδή σε binary: `11111101`,
βλέποντας ξεκάθαρα το πρόβλημα.

---

Θα δούμε το σφάλμα: **Free blocks count wrong for group #0 (7960, counted=7961)**

Από τον block descriptor παίρνω τα ελεύθερα blocks του group 0:
```bash
root@utopia:/# hexdump -C -s 2048 -n 14 /dev/vdd
00000800  03 00 00 00 04 00 00 00  05 00 00 00 18 1f        |..............|
0000080e
```
Τα οποία είναι `18 1f`, ή `7960`

To bitmap table για το block 0:
```bash
root@utopia:/# hexdump -C -s 2048 -n 4 /dev/vdd
00000800  03 00 00 00                                       |....|
00000804
```
Βρίσκεται στο block `03 00 00 00`, ή `3`.
```bash
root@utopia:/# hexdump -C -s $((3*1024)) -n 1024 /dev/vdd
00000c00  ff ff ff ff fd ff ff ff  ff ff ff ff ff ff ff ff  |................|
00000c10  ff ff ff ff ff ff ff ff  ff ff ff ff ff 00 00 00  |................|
00000c20  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
00001000
```
Έχουμε ότι `00001000` - `00000c20` in hex μας κάνουν `992` bytes.
Οπότε έχουμε συνολικά `992*8 + 3*8 + 1` = `7961` ελεύθερα blocks.

---

Τέλος, **Free blocks count wrong (926431538, counted=19801)**

Θα διαβάσουμε το Superblock του /dev/vdd ώστε να δούμε το s_free_blocks_count

```bash
root@utopia:/home/user/shared# hexdump -C -s 1024 -n 264 /dev/vdd
00000400  10 14 00 00 00 50 00 00  00 04 00 00 32 39 38 37  |.....P......2987|
00000410  f9 13 00 00 01 00 00 00  00 00 00 00 00 00 00 00  |................|
00000420  00 20 00 00 00 20 00 00  b0 06 00 00 e9 7a 78 65  |. ... .......zxe|
00000430  ea 7a 78 65 01 00 ff ff  53 ef 01 00 01 00 00 00  |.zxe....S.......|
00000440  e9 7a 78 65 00 00 00 00  00 00 00 00 01 00 00 00  |.zxe............|
00000450  00 00 00 00 0b 00 00 00  80 00 00 00 00 00 00 00  |................|
00000460  00 00 00 00 00 00 00 00  19 03 21 43 52 ce 49 17  |..........!CR.I.|
00000470  8e c5 99 1c 89 ee 42 1b  66 73 64 69 73 6b 33 2e  |......B.fsdisk3.|
00000480  69 6d 67 00 00 00 00 00  2f 63 73 6c 61 62 2d 62  |img...../cslab-b|
00000490  75 6e 6b 65 72 00 00 00  00 00 00 00 00 00 00 00  |unker...........|
000004a0  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
000004e0  00 00 00 00 00 00 00 00  00 00 00 00 b1 b5 d2 0e  |................|
000004f0  a7 8a 4e 76 93 48 d3 97  0e b6 92 68 01 00 00 00  |..Nv.H.....h....|
00000500  0c 00 00 00 00 00 00 00                           |........|
00000508
```

```bash
Offset (bytes)	Size (bytes)	Description
12                  4	        s_free_blocks_count = 32 39 38 37 = 926431538
```
Είναι αδύνατο να έχουμε 926431538 free blocks γιατί με block_size = 1024bytes θα είχαμε δίσκο με τεράστια χωρητικότητα (πολλές τάξεις μεγέθους παραπάνω από ότι έχουμε τώρα).

---

### Ερώτηση 5: Επιδιορθώστε κάθε αλλοίωση ξεχωριστά με χρήση της μεθόδου hexedit. Για κάθε μία από τις αλλοιώσεις που επιδιορθώνετε, τρέξτε το εργαλείο fsck με τρόπο που δεν προκαλεί καμία αλλαγή [“dry run”] και επιβεβαιώστε ότι πλέον δεν την εντοπίζει.
Χρησιμοποιούμε hexedit γιατί το hexdump είναι ΜΟΝΟ για view.

Θα βρούμε το: **First entry 'BOO' (inode=1717) in directory inode 1717 (/dir-2) should be '.'**

Θα πάμε στο 8412 * 1024 = 0x837000 που ξεκινάει το block που περιέχει το block 
Θα αλλάξουμε το name_len από 3 σε 1 
θα βάλουμε τον χαρακτήρα . που είναι το 2E στην θέση του B και θα μηδενίσουμε τα 2 O (BOO)

Πριν:
```bash
B5 06 00 00  0C 00 03 00  42 4F 4F 00 
```

Μετά
```bash
B5 06 00 00  0C 00 01 00  2E 00 00 00
```

---

Προχωράμε στο επόμενο: **Inode 3425 ref count is 1, should be 2.**
Θα πάμε στην διεύθυνση 0x1001400 + 26 bytes δηλαδή στο offset = 0x100141A  όπου είναι το index node του block 2 και θα αλλάξουμε το i_links_count από 01 00 σε 02 00

```bash
root@utopia:~# fsck.ext2 -n /dev/vdd
e2fsck 1.47.0 (5-Feb-2023)
fsdisk3.img contains a file system with errors, check forced.
Pass 1: Checking inodes, blocks, and sizes
Pass 2: Checking directory structure
Pass 3: Checking directory connectivity
Pass 4: Checking reference counts
Pass 5: Checking group summary information
Block bitmap differences:  +34
Fix? no

Free blocks count wrong for group #0 (7960, counted=7961).
Fix? no

Free blocks count wrong (926431538, counted=19801).
Fix? no


fsdisk3.img: ********** WARNING: Filesystem still has errors **********

fsdisk3.img: 23/5136 files (0.0% non-contiguous), 18446744072783140558/20480 blocks
root@utopia:~#
```
Βλέπουμε πως και το inode ref count από 1 έγινε 2 και δεν έχουμε error, και το **Inode 3425 ref count is 1, should be 2.** δεν παρουσιάζεται.

---

Συνεχίζουμε στο **Block bitmap differences: +34**

Το Block 34 θα έπρεπε να φαίνεται ως allocated 
Πρέπει να κάνουμε το bit στο block map για το block 34 1 για να δείξουμε πως είναι free. 
To Block 34 ανήκει στο Group 0 
Κάθε byte στο block bitmap αντιπροσωπεύει 8 συνεχόμενα blocks 
roundup(34/8) = 5 
34 % 8 = το 2ο bit αντιπροσωπεύει το 34

Το block bitmap ξεκινάει στο 3ο block (3*1024) = 0xC00. 0xC00 + 4 bytes = 0XC04 εκεί που ξεκινάει το 5ο block
```bash
FD
```
Το κάνουμε FF
```bash
FF
```
Βλέπουμε πως έφτιξε τρέχοντας fsck.ext2 -n /dev/vdd
```bash
root@utopia:~# fsck.ext2 -n /dev/vdd
e2fsck 1.47.0 (5-Feb-2023)
fsdisk3.img contains a file system with errors, check forced.
Pass 1: Checking inodes, blocks, and sizes
Pass 2: Checking directory structure
Pass 3: Checking directory connectivity
Pass 4: Checking reference counts
Pass 5: Checking group summary information
Free blocks count wrong (926431538, counted=19800).
Fix? no

fsdisk3.img: 23/5136 files (0.0% non-contiguous), 18446744072783140558/20480 blocks
```

---

Συνεχίζουμε στο **Free blocks count wrong for group #0 (7960, counted=7961).**
Το οποίο φτιάχτηκε φτιάχνοτας το bitmap lock

---

Τέλος έμεινε το **Free blocks count wrong (926431538, counted=19800).**

Το s_free_blocks_count είναι λάθος όποτε θα το αλλάξουμε. Ξεκινάει στο 12ο Byte (+1024) = 0x40C

Βάζουμε την τιμή 19800 = 59 4D 00 00 (le) αντί για 926431538 (32 39 38 37) (le) που μας δείχνει εσφαλμένα.

```bash
32 39 38 37 before
59 4D 00 00 after
```

Βλέπουμε πως το πρόβλημα λύθηκε και έχουμε:

```bash
root@utopia:~# fsck.ext2 -n /dev/vdd
e2fsck 1.47.0 (5-Feb-2023)
fsdisk3.img: clean, 23/5136 files, 679/20480 blocks
root@utopia:~#
```
