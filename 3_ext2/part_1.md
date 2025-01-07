### Ερώτηση 1.
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


### Ερώτηση 2.
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
Εκτελόυμε την εντολή `hexdump /dev/vdb` και από το output παρατηρούμε το τελευταίο offset να είναι το 3200000 (σε hex μορφή). Άρα σε δεκαδική μορφή έχουμε 5242880010. Δηλαδή το μέγεθος του δίσκου είναι 50MiB.


### Ερώτηση 3
#### Προσέγγιση: tools
Σύμφωωνα με το [man-page](https://linux.die.net/man/8/blkid) της εντολής `blkid`: blkid can determine the type of content (e.g. filesystem, swap) a block device holds, and also attributes (tokens, NAME=value pairs) from the content metadata (e.g. LABEL or UUID fields). Συνεπώς, παρατηρούμε ότι είναι `ext2`:
```bash
root@utopia:~# blkid /dev/vdb
/dev/vdb: LABEL="fsdisk1.img" UUID="c63028e5-711b-410d-a263-e7ca2b15a8d3" BLOCK_SIZE="1024" TYPE="ext2"
```

#### Προσέγγιση: hexedit
Χρησιμοποιούμε την `hexdump` και παίρνουμε (read only the super block):
```bash
root@utopia:~# hexdump -s 1024 -n 1024 -C /dev/vdb
00000400  18 32 00 00 00 c8 00 00  00 0a 00 00 90 c1 00 00  |.2..............|
00000410  0a 32 00 00 01 00 00 00  00 00 00 00 00 00 00 00  |.2..............|
00000420  00 20 00 00 00 20 00 00  28 07 00 00 e4 7a 78 65  |. ... ..(....zxe|
00000430  e5 7a 78 65 01 00 ff ff  53 ef 01 00 01 00 00 00  |.zxe....S.......|
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
00000570  00 00 00 00 00 00 00 00  0d 00 00 00 00 00 00 00  |................|
00000580  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
00000800
```
Στο **0x38** από το superblock start βλέπουμε το `53 ef` που είναι το magic number (little endian του EF53) που ανταποκρίνεται στα ext2/ext3/ext4.


### Ερώτηση 4
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

### Ερώτηση 5
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


### Ερώτηση 6
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


### Ερώτηση 7
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


### Ερώτηση 8
Το μπλοκ (block) σε ένα σύστημα αρχείων είναι η βασική μονάδα αποθήκευσης δεδομένων που χρησιμοποιείται για την οργάνωση και τη διαχείριση του χώρου στον δίσκο. Είναι ένα συνεχόμενο τμήμα του αποθηκευτικού μέσου, με καθορισμένο μέγεθος, και αποτελεί το θεμέλιο για την αποθήκευση αρχείων και μεταδεδομένων στο σύστημα αρχείων.


### Ερώτηση 9
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


### Ερώτηση 10
Το inode (Index Node) είναι μια δομή δεδομένων σε ένα σύστημα αρχείων που χρησιμοποιείται για να αποθηκεύει μεταδεδομένα σχετικά με ένα αρχείο ή έναν κατάλογο. Το inode δεν αποθηκεύει τα ίδια τα δεδομένα του αρχείου, αλλά περιλαμβάνει πληροφορίες όπως ταυτότητα αρχείου, μέγεθος αρχείου, τύπος αρχείου, δικαιώματα πρόσβασης, timestamps, hard links, και δείκτες στα μπλοκ δεδομένων που περιέχουν το περιεχόμενο του αρχείου.


### Ερώτηση 11
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


### Ερώτηση 12
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


### Ερώτηση 13
Το superblock στο σύστημα αρχείων ext2 είναι μια κρίσιμη δομή δεδομένων που περιέχει βασικές πληροφορίες για τη διαμόρφωση και την κατάσταση του συστήματος αρχείων. Είναι ουσιαστικά ο "κεντρικός πίνακας ελέγχου" του συστήματος αρχείων και χρησιμοποιείται για τη διαχείριση και την οργάνωση των δεδομένων στον δίσκο.
Αποθηκεύει παραμέτρους όπως το μέγεθος του συστήματος αρχείων, το μέγεθος των μπλοκ, και τον αριθμό των inodes.
Επειδή το superblock είναι κρίσιμο για τη λειτουργία του συστήματος αρχείων, το ext2 κρατά αντίγραφα ασφαλείας του superblock σε διάφορα σημεία του δίσκου (backup superblocks). Αυτά τα αντίγραφα είναι χρήσιμα σε περίπτωση καταστροφής του κύριου superblock.


### Ερώτηση 14
Το κύριο superblock βρίσκεται 1 KB (1024 bytes) μετά την αρχή του διαμερίσματος. Αυτό σημαίνει ότι αν το μέγεθος του μπλοκ είναι 1 KB, το superblock ξεκινά στο μπλοκ 1. Αν το μέγεθος του μπλοκ είναι 2 KB ή 4 KB, το superblock ξεκινά στο μέσο του πρώτου μπλοκ.


### Ερώτηση 15
Η ύπαρξη εφεδρικών αντιγράφων του superblock σε ένα σύστημα αρχείων έχει κρίσιμη σημασία για την ανθεκτικότητα και την αξιοπιστία του συστήματος. Ο βασικός λόγος για αυτήν τη σχεδίαση είναι η προστασία από σφάλματα ή καταστροφές που μπορεί να προκύψουν στον δίσκο ή στο κύριο superblock. 


### Ερώτηση 16
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


### Ερώτηση 17
Στο σύστημα αρχείων ext2, το block group είναι μια βασική μονάδα οργάνωσης του χώρου στον δίσκο. Ο δίσκος χωρίζεται σε πολλές ομάδες μπλοκ (block groups), καθεμία από τις οποίες περιέχει έναν καθορισμένο αριθμό μπλοκ και τις απαραίτητες δομές δεδομένων για τη διαχείριση των μπλοκ και των inodes.
Αυτή η προσέγγιση της ομαδοποίησης έχει σχεδιαστεί για να μειώσει την κατακερματισμένη αποθήκευση δεδομένων και να αυξήσει την ταχύτητα πρόσβασης, οργανώνοντας τα δεδομένα τοπικά σε κάθε block group.


### Ερώτηση 18
Ο αριθμός των block groups σε ένα σύστημα αρχείων ext2 εξαρτάται από το συνολικό μέγεθος του συστήματος αρχείων και το μέγεθος κάθε block group. Τα block groups κατανέμονται ομοιόμορφα στο σύστημα αρχείων και καθένα από αυτά διαχειρίζεται έναν καθορισμένο αριθμό μπλοκ.

Ο αριθμός των block groups υπολογίζεται με τον τύπο:

$$
\text{Αριθμός Block Groups} = \left\lceil \frac{\text{Συνολικά Blocks στο Σύστημα Αρχείων}}{\text{Blocks ανά Block Group}} \right\rceil
$$

όπου:
    * Συνολικά Blocks: Ο συνολικός αριθμός μπλοκ στο σύστημα αρχείων
    * Blocks ανά Block Group: Εξαρτάται από το μέγεθος του block group, το οποίο καθορίζεται κατά τη δημιουργία του συστήματος αρχείων. Συνήθως, ένα block group περιέχει 8192 μπλοκ για μέγεθος μπλοκ 1 KB, 32768 μπλοκ για μέγεθος μπλοκ 4 KB, κ.λπ.


### Ερώτηση 19
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


### Ερώτηση 20
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


### Ερώτηση 21
Η ύπαρξη εφεδρικών αντιγράφων των block group descriptors στο σύστημα αρχείων ext2 έχει μεγάλη σημασία για την ανθεκτικότητα, την αξιοπιστία και την ανάκτηση του συστήματος αρχείων. Οι block group descriptors είναι ζωτικής σημασίας για τη λειτουργία του συστήματος, καθώς περιέχουν πληροφορίες για τη δομή και τη διαχείριση κάθε block group. Αν καταστραφούν, η πρόσβαση στα δεδομένα και στα μεταδεδομένα του συστήματος αρχείων μπορεί να γίνει αδύνατη.


### Ερώτηση 22
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

### Ερώτηση 23
Το block bitmap και το inode bitmap είναι κρίσιμες δομές δεδομένων στο σύστημα αρχείων ext2, οι οποίες χρησιμοποιούνται για τη διαχείριση του χώρου αποθήκευσης και των inodes, αντίστοιχα.

Το block bitmap είναι ένας χάρτης που παρακολουθεί την κατάσταση όλων των blocks δεδομένων μέσα σε ένα block group. Κάθε bit στο block bitmap αντιστοιχεί σε ένα block του block group και είναι 1 αν το block είναι κατειλημμένο, αλλίως 0. Όταν το σύστημα αρχείων χρειάζεται να αποθηκεύσει νέα δεδομένα, χρησιμοποιεί το block bitmap για να εντοπίσει ένα ελεύθερο block.

Το inode bitmap είναι ένας χάρτης που παρακολουθεί την κατάσταση όλων των inodes μέσα σε ένα block group. Κάθε bit στο inode bitmap αντιστοιχεί σε ένα inode του block group και είναι 1 αν το inode είναι κατειλημμένο (χρησιμοποιείται από ένα αρχείο ή κατάλογο), αλλίως 0. Όταν δημιουργείται ένα νέο αρχείο ή κατάλογος, το σύστημα βρίσκει ένα διαθέσιμο inode μέσω του inode bitmap.

Το block bitmap και το inode bitmap βρίσκονται στο block group όπου ανήκουν και είναι αποθηκευμένα σε προκαθορισμένα σημεία. Το block bitmap βρίσκεται μετά το superblock και τους block group descriptors. Το inode bitmap βρίσκεται αμέσως μετά το block bitmap.

### Ερώτηση 24
Ο Inode Table είναι ένας πίνακας που περιέχει όλα τα inodes ενός block group. Κάθε inode καταλαμβάνει ένα σταθερό μέγεθος (συνήθως 128 ή 256 bytes) και αποθηκεύει πληροφορίες για ένα μόνο αρχείο ή κατάλογο. Κάθε inode περιέχει το μέγεθος του αρχείου, τύπο αρχείου, δικαιώματα πρόσβασης, timestampts, και δείκτες στα μπλοκ δεδομένων που περιέχουν το περιεχόμενο του αρχείου.

Τα Inode Tables βρίσκονται σε κάθε block group του συστήματος αρχείων. Η θέση τους είναι σταθερή και ορίζεται μετά από τα bytes του Inode Bitmap.


### Ερώτηση 25
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


### Ερώτηση 26
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

### Ερώτηση 27
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

Now we calculate the inode table location from the given inode in order to access the directories content:

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

### Ερώτηση 28
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


### Ερώτηση 29
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
