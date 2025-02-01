Παρακάτω ακολουθεί ένα εκτενές συνοπτικό “φύλλο προετοιμασίας”, σχεδιασμένο για να καλύπτει τα πιο **συχνά επαναλαμβανόμενα θέματα** που προκύπτουν στις εξετάσεις αυτού του μαθήματος/υλικού. Καλύπτει κυρίως ζητήματα:

1. **Ανάπτυξης βασικών device drivers** (κυρίως character drivers) στο Linux.  
2. **Χρήσης κλήσεων συστήματος** (system calls) και βασικών μηχανισμών στον πυρήνα (spinlocks, waitqueues, interrupt context vs process context).  
3. **Βασικά στοιχεία των εντολών κελύφους (shell)**, pipes, redirections, forks, κ.λπ.  
4. **Συστήματα αρχείων (κυρίως ext2)**, δομή superblock, inodes, bitmaps, πώς αποθηκεύονται τα δεδομένα, κ.λπ.  
5. **Θέματα κλασικά** που εξετάζονται σε συστήματα που συνδυάζουν εγγραφή/ανάγνωση σε αρχεία και διεργασιών.

Το υλικό εστιάζει σε όσα έχουν μεγαλύτερη **πιθανότητα εμφάνισης** στην εξέταση. Εστιάζει σε:

- Σύντομη θεωρία \+  
- Ενδεικτικά παραδείγματα κώδικα (device driver / user space) \+  
- Τυπικούς μηχανισμούς συγχρονισμού \+  
- Δομικά στοιχεία ext2.

Μπορείτε να το χρησιμοποιήσετε σαν “ενιαίο έγγραφο μελέτης” για την επερχόμενη εξέταση.

---

# 1. Εισαγωγικά στον Προγραμματισμό Συστήματος / Device Drivers

## 1.1. Τύποι Device Drivers

- **Character Device Drivers (cdev)**: Παρέχουν σειριακή (byte-by-byte) πρόσβαση. Τυπικά υλοποιούν βασικές λειτουργίες open(), read(), write(), ioctl(), close().  
- **Block Device Drivers**: Χειρίζονται δεδομένα κατά “μπλοκ”.  
- **Network Drivers**: Ειδική κατηγορία.

Συχνά, το μάθημα ασχολείται **σχεδόν αποκλειστικά** με **character devices**. Η τυπική δήλωση τους στο Linux γίνεται με:

```c
static const struct file_operations my_fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .read = my_read,
    .write = my_write,
    .ioctl = my_ioctl, /* ή unlocked_ioctl */
    .release = my_close,
    /* κ.λπ. */
};
```

## 1.2. Βασική Ροή Δημιουργίας Character Driver

1. **Καταχώριση Major/Minor** αριθμών (\(alloc_chrdev_region\) κ.λπ.).  
2. **Προσθήκη του cdev** (π.χ. cdev_init, cdev_add).  
3. **Υλοποίηση** μεθόδων `open()`, `read()`, `write()`, `ioctl()`, `release()`.  
4. **Δημιουργία συσκευής** σε user space με `mknod /dev/... c major minor`, ή χρήση udev.

## 1.3. open(), release(), read(), write()

- **open()**: Εκτελείται σε process context, συνήθως δημιουργεί μια per-open state (π.χ. δομή `struct my_state`), την οποία αποθηκεύουμε σε `filp->private_data`.  
- **read()**: Δέχεται (struct file *filp, char __user *buf, size_t len, loff_t *off). Μέσα εκεί κάνουμε `copy_to_user()` για να στείλουμε δεδομένα στον χώρο χρήστη.  
- **write()**: Παρόμοια με read(), αλλά συχνά κάνουμε `copy_from_user()`.  
- **release()**: Τυπικά καθαρίζει τη μνήμη (π.χ. kfree(per-open struct)).

---

# 2. Συγχρονισμός στον Πυρήνα & Χρήση Interrupt Handlers

## 2.1. Διαφορετικά Contexts

- **Process context**: Εκτελείται για λογαριασμό μιας διεργασίας. Μπορεί να κάνει κλήσεις που potentially sleep, π.χ. `mutex_lock()`, `down_interruptible()`, κ.λπ.  
- **Interrupt context**: Εκτελείται σε σκληρή διακοπή (ISR). Δεν υπάρχει “τρέχουσα διεργασία” για να κοιμηθεί.  
  - **Απαγορεύεται** ο ύπνος, άρα δεν επιτρέπονται mutex, semaphores κ.λπ. Μόνο spinlocks/atomic ops.  
  - Συνήθως, στο interrupt handler **καλούμε** `wake_up_interruptible(...)` αν πρέπει να ξεμπλοκάρουμε κάποια διεργασία που περιμένει δεδομένα.

## 2.2. Spinlocks vs Mutex

- **spinlock**: Δεν επιτρέπει sleep. Χρησιμοποιείται σε interrupt context ή όπου χρειάζεται πολύ γρήγορο κλείδωμα μικρής διάρκειας.  
- **mutex**: Επιτρέπεται να αποκλειστούμε (sleep) περιμένοντας. Συχνά σε process context.

## 2.3. Wait Queues

- Χρησιμοποιούνται για να **κοιμούνται** διεργασίες όταν περιμένουν κάποιο γεγονός (π.χ. “διαθέσιμα δεδομένα”).  
- Συσχετίζονται με κλήσεις όπως `wait_event_interruptible(my_wq, condition)` κι έπειτα `wake_up_interruptible(&my_wq)` όταν ολοκληρωθεί το γεγονός (π.χ. έφτασαν δεδομένα).

## 2.4. Παράδειγμα Interrupt Handler + read()

Φανταστείτε έναν driver που λαμβάνει δεδομένα από hardware σε interrupt handler:

```c
static void my_intr_handler(void)
{
    spin_lock(&dev->lock);
    /* Ανάγνωση/αποθήκευση δεδομένων στον κυκλικό buffer */
    spin_unlock(&dev->lock);
    wake_up_interruptible(&dev->read_wq); 
}
```

Και στο `read()`:

```c
static ssize_t my_read(struct file *filp, char __user *usrbuf, size_t count, loff_t *offp)
{
    spin_lock(&dev->lock);
    while ( !data_available ) {
        spin_unlock(&dev->lock);

        if ( wait_event_interruptible(dev->read_wq, data_available) )
            return -ERESTARTSYS;

        spin_lock(&dev->lock);
    }
    /* copy_to_user(...) */
    spin_unlock(&dev->lock);
    return bytes_read;
}
```

---

# 3. Βασικά για Character Drivers που Χρησιμοποιούν Κυκλικό Buffer

Πολύ κλασικό θέμα: Έχετε έναν **κυκλικό απομονωτή (circular buffer)** όπου γράφονται δεδομένα από ISR, κι ένα read() που διαβάζει bytes από τον buffer.

1. **Μεταβλητές**:  
   - `char buffer[BUFFER_SIZE]`;  
   - `int head, tail`; // δεικτοδείκτες για κυκλική δομή.  
2. **ISR γράφει**: Προσθέτει δεδομένα στον buffer, αυξάνει `head`, κάνει `wake_up_interruptible()`.  
3. **read()**: Αν δεν υπάρχουν δεδομένα (`head == tail`), κοιμόμαστε. Αλλιώς αντιγράφουμε στον χρήστη.  
4. **Προστασία**: Αν πρέπει και ο ISR και το read() να προσπελάζουν `head/tail`, συχνά χρησιμοποιούμε **spinlock**.

---

# 4. Ενδεικτικά Σημεία σε Ioctl

Η κλήση συστήματος `ioctl()` επιτρέπει ειδικές εντολές προς τη συσκευή, π.χ.:

```c
static long my_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    switch(cmd) {
        case MY_SET_PARAM: 
            if ( copy_from_user(&local_param, (void __user *)arg, sizeof(local_param)) )
                return -EFAULT;
            break;
        /* ... */
        default: return -EINVAL;
    }
    return 0;
}
```

---

# 5. Εντολές Κελύφους που Πέφτουν Συχνά: Fork, Exec, Pipe, Redirections

## 5.1. Πώς λειτουργεί το `ps | grep init`

1. **pipe()** → δημιουργία pipe.  
2. **fork()** δύο φορές, ένα παιδί εκτελεί `ps`, ένα παιδί εκτελεί `grep init`.  
3. Η έξοδος του `ps` συνδέεται με το write end του pipe, η είσοδος του `grep` με το read end.  
4. Και τα δύο παιδιά εκτελούν `execve()` τις αντίστοιχες εντολές.

## 5.2. `ln` για Hard Links

- **Hard link** → στο ίδιο inode. Δεν επιτρέπεται διασυσκευικά (`EXDEV` error) σε διαφορετικές συσκευές.  

## 5.3. `echo "bar" | cat > foo.txt`

- Εκτελούνται 2 forks:  
  - Ο ένας κώδικας execve("echo","bar") → stdout = pipe write end.  
  - Ο άλλος execve("cat") → stdin = pipe read end, stdout = open("foo.txt").  

---

# 6. Συστήματα Αρχείων: ext2 (και παρόμοια ext3, ext4)

## 6.1. Δομικά Στοιχεία

1. **Superblock**: Περιέχει βασικές πληροφορίες, όπως αριθμός inodes, blocks, μέγεθος block, κ.λπ. Έχει αντίγραφα σε διάφορα block groups (όχι μόνο “μοναδικό”).  
2. **Block Groups**: Το ext2 χωρίζει το filesystem σε block groups.  
   - Κάθε block group έχει:  
     - Inode bitmap (1 block)  
     - Block bitmap (1 block)  
     - Inode table (μπορεί να καταλαμβάνει πολλά blocks)  
     - Data blocks.  
3. **Inodes**: Κάθε αρχείο/κατάλογος αντιστοιχεί σε ένα inode. Δεν περιέχει το όνομα, μόνο μεταδεδομένα + pointers στα data blocks.  
4. **Directory Entries**: Οι φάκελοι είναι “αρχεία” που περιέχουν ζεύγη (filename, inode). Έτσι συνδέεται όνομα αρχείου με inode.

## 6.2. Hard Links, Soft Links

- **Hard link**: Δεύτερο (ή παραπάνω) όνομα που δείχνει στο ίδιο inode. Επομένως, δεν αυξάνει το usage των data blocks, απλώς αυξάνει το link count του inode.  
- **Soft link**: Ειδικό inode που περιέχει path προς ένα άλλο αρχείο.

## 6.3. “Δεν υπάρχει χώρος” (No space left on device)

- Μπορεί να σημαίνει **τέλος blocks** ή **τέλος inodes**. Ένα ext2 μπορεί να έχει blocks ελεύθερα, αλλά να μη διαθέτει inodes (ή το ανάποδο).

## 6.4. Sparse Files

- Όταν κάνουμε `lseek(fd, 4096, SEEK_SET)` και μετά `write()`, δημιουργείται “hole” στο αρχείο. Εφόσον το filesystem το υποστηρίζει, δεν καταναλώνονται blocks για όλο το διάστημα του hole, μέχρι να γραφούν bytes.

---

# 7. Ενδεικτικές Ερωτήσεις/Ασκήσεις με Συστήματα Αρχείων

1. **ext2**: “Πόσα inodes χρειάζεται να προσπελαστούν για το /dir1/dir2/file;” → Συνήθως 4 (root, dir1, dir2, file).  
2. **inode table**: μπορεί να καταλαμβάνει πολλά blocks.  
3. **inode bitmap**: ακριβώς 1 block.  
4. **superblock**: υπάρχουν αντίγραφα.

---

# 8. Παραδείγματα Σύντομων Ζητημάτων

## 8.1. unlink() δεν διαγράφει πάντα περιεχόμενα

- Αν το αρχείο έχει πολλαπλά hard links ή είναι ανοιχτό από άλλη διεργασία, η πραγματική διαγραφή στο δίσκο γίνεται μόνο όταν μηδενιστεί το link count + δεν είναι ανοιχτό από καμία διεργασία.

## 8.2. spin_lock() σε interrupt context

- Δεν μπορούμε να καλέσουμε mutex/ down_interruptible σε interrupt context, διότι θα κοιμηθούμε. Επιτρέπεται μόνο spinlocks και atomic operations.

## 8.3. strace και mmap

- `strace` καταγράφει μόνο system calls. Εάν γράφουμε σε αρχείο μέσω mmap (π.χ. `memcpy(map, data, size)`), αυτό **δεν** εμφανίζεται ως write() σε strace.

## 8.4. Process offset σε file descriptors

- Κάθε process που κάνει open() το ίδιο όνομα αρχείου **έχει ξεχωριστό `struct file`** και ξεχωριστό offset.  
- Εάν γίνει `fork()`, parent/child μοιράζονται το ίδιο open fd (\(=>\) ίδιο offset). Lseek του ενός επηρεάζει και τον άλλο.

---

# 9. Μεθοδολογία Εξεταστικής Προετοιμασίας

Ενδεικτικά, καλό είναι να είστε έτοιμοι να:

1. **Υλοποιήσετε** κώδικα *character driver* σε pseudo-C:  
   - open / read / write / ioctl.  
   - χειρισμός interrupt (spinlock, waitqueues).  
   - χρήση circular buffer.  
2. **Εξηγήσετε** τι γίνεται στη shell εντολή `cmd1 | cmd2 > file 2>&1`. Ποια system calls, πόσες διεργασίες.  
3. **Σχεδιάσετε** πώς “διαβάζουμε μόνο συνεχόμενες μετρήσεις” και πώς επιστρέφουμε EOF αν “σπάσει” η συνέχεια.  
4. **Αναφέρετε** πληροφορίες ext2 (superblock, inodes, bitmaps, block groups, πόσα inodes απαιτούνται για /dir1/dir2/file κ.λπ.).  
5. **Γνωρίζετε** πώς διαφέρει το process context από το interrupt context.

---

# 10. Ενδεικτική Λίστα “Tips & Tricks” για την Τελευταία Στιγμή

1. **Πάντα spinlock** στο interrupt handler + στις shared μεταβλητές (π.χ. head/tail counters).  
2. **copy_to_user / copy_from_user** για μεταφορά δεδομένων ανάμεσα σε user space \↔ kernel.  
3. **wait_event_interruptible()** + `wake_up_interruptible()` για blocking read/write drivers.  
4. **Επιστροφή 0** σε read() ⇒ σημαίνει EOF στην πλευρά χρήστη.  
5. **Non-blocking I/O**: Αν η συσκευή είναι ανοιχτή με O_NONBLOCK, και δεν υπάρχουν δεδομένα, συνήθως επιστρέφουμε -EAGAIN.  
6. **Προσοχή** στην υλοποίηση partial reads (εάν ζητηθούν 200 bytes αλλά έχουμε 64).  
7. **ext2**: “inode table” vs “inode bitmap” vs “block bitmap”.  
8. **unlink()**: διαγράφει μόνο *entry* (directory entry). Αν link count = 0, τότε το inode μπορεί να απελευθερωθεί.

---

## Επίλογος

Το παραπάνω “τεράστιο” έγγραφο ενσωματώνει τα **θεμέλια** που εμφανίζονται συχνά σε εξετάσεις του είδους:

- Πώς γράφουμε έναν απλό character device driver με interrupt handler, circular buffer, blocking I/O.  
- Πώς λειτουργούν οι βασικές κλήσεις συστήματος (open, read, write, ioctl, pipe, fork, exec,  dup2).  
- Τυπική θεωρία ext2: superblock, inode table, bitmaps, block groups, διαφορές σε hard link/soft link, κ.λπ.  
- Θέματα concurrency (spinlock vs. mutex).  
- Τυπικές ερωτήσεις γύρω από shell commands (redirections, pipes) και system calls (strace, mmap, EEXDEV, EAGAIN κ.ο.κ).

Δώστε έμφαση στην πρακτική:  
- Γράψτε ψευδοκώδικα για read/write drivers.  
- Μάθετε τα βασικά struct (π.χ. `struct file`, `struct inode`, `copy_to_user` / `copy_from_user`), system calls (`pipe()`, `fork()`, `execve()`, `dup2()`, `wait()`).  
- Ανακεφαλαιώστε τα δεδομένα ext2 (superblock, inodes, block groups, bitmaps).  
