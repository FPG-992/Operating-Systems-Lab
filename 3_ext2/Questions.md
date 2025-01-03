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

2. Τι μέγεθος έχει ο δίσκος που προσθέσατε στο utopia;
### με mount/

### με hexedit

3. Τι σύστημα αρχείων περιέχει;
4. Πότε ακριβώς δημιουργήθηκε αυτό το σύστημα αρχείων; Δείξτε τη χρονοσφραγίδα [timestamp].
5. Πότε ακριβώς προσαρτήθηκε τελευταία φορά; Δείξτε τη χρονοσφραγίδα.
6. Σε ποιο μονοπάτι προσαρτήθηκε τελευταία φορά;
7. Πότε ακριβώς τροποποιήθηκε τελευταία φορά; Δείξτε τη χρονοσφραγίδα.
8. Τι είναι το μπλοκ σε ένα σύστημα αρχείων;
9. Τι μέγεθος μπλοκ [block size] χρησιμοποιεί αυτό το σύστημα αρχείων;
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