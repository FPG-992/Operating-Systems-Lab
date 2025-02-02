## 1. Superblock Registration/Unregistration (super.c)

### init_ext2_fs()

This function initializes the ext2-lite filesystem by creating an inode cache and then registering the filesystem with the kernel. The registration makes the filesystem known to the kernel (it will then appear in /proc/filesystems).

```c
static int __init init_ext2_fs(void)
{
    int err = init_inodecache();
    if (err)
        return err;

    /* Register ext2-lite filesystem in the kernel.
     * This call adds ext2-lite to the list of supported filesystems.
     * If an error occurs during registration, we must call destroy_inodecache()
     * to free the inode cache that we allocated.
     */
    err = register_filesystem(&ext2_fs_type);
    if (err) {
        destroy_inodecache();
    }

    return err;
}
```

**Explanation:**

- **init_inodecache():** Allocates an inode cache used later for inode allocation.
- **register_filesystem(&ext2_fs_type):** Registers our ext2-lite filesystem (defined by ext2_fs_type) with the kernel.  
- **Error handling:** If registration fails, we destroy the inode cache to avoid memory leaks.

---

### exit_ext2_fs()

This function unregisters the filesystem from the kernel and destroys the inode cache. This is called when the module is removed.

```c
static void __exit exit_ext2_fs(void)
{
    /* Unregister ext2-lite filesystem from the kernel.
     * This removes the filesystem from the list in /proc/filesystems.
     */
    unregister_filesystem(&ext2_fs_type);

    /* Destroy the inode cache allocated in init_ext2_fs() */
    destroy_inodecache();
}
```

**Explanation:**

- **unregister_filesystem(&ext2_fs_type):** Removes our filesystem from the kernel’s list.  
- **destroy_inodecache():** Frees the resources allocated for the inode cache.

---

## 2. Directory Entry Lookup (ext2_find_entry in dir.c)

This function scans the directory pages to find an entry matching a given name (provided as a qstr). It returns a pointer to the directory entry if found or an error pointer otherwise.

```c
ext2_dirent *ext2_find_entry(struct inode *dir, const struct qstr *child,
                             struct folio **foliop)
{
    const char *name = child->name;
    int namelen = child->len;
    unsigned reclen = EXT2_DIR_REC_LEN(namelen);
    unsigned long npages = dir_pages(dir);  // number of pages in the directory
    unsigned long i;
    ext2_dirent *de;
    char *kaddr;

    if (npages == 0)
        return ERR_PTR(-ENOENT);

    /* Scan each page of the directory */
    for (i = 0; i < npages; i++) {
        /* Get a pointer to the i-th page of the directory. 
         * ext2_get_folio() returns a kernel virtual address for the page,
         * and sets the folio pointer for later release.
         */
        kaddr = ext2_get_folio(dir, i, 0, foliop);
        if (IS_ERR(kaddr))
            return ERR_CAST(kaddr);

        /* Determine the last valid byte in this page so we don't scan past the directory's size */
        unsigned last_byte = ext2_last_byte(dir, i);

        /* Iterate over directory entries in the page */
        for (unsigned offset = 0; offset < last_byte;) {
            de = (ext2_dirent *)(kaddr + offset);
            
            /* If the directory entry is valid (nonzero inode) and the name matches,
             * we have found the desired entry.
             */
            if (de->inode && ext2_match(namelen, name, de))
                return de;  // Found the matching directory entry

            /* Move to the next directory entry using the rec_len field */
            offset += le16_to_cpu(de->rec_len);
        }

        /* Release the kernel mapping for the current folio */
        folio_release_kmap(*foliop, kaddr);
    }
    return ERR_PTR(-ENOENT); // If not found, return error pointer for "entry not found"
}
```

**Explanation:**

- The function iterates over all pages (npages) of the directory.
- For each page, it maps the page into kernel space with `ext2_get_folio()` and determines the last valid byte using `ext2_last_byte()`.
- It then iterates over directory entries in that page (using the rec_len field to jump from one entry to the next).
- If an entry with a matching name is found (checked via ext2_match()), it is returned.
- After scanning each page, the page mapping is released.
- If no matching entry is found in all pages, it returns an error pointer (-ENOENT).

---

## 3. Retrieving an Inode from Disk (ext2_get_inode in inode.c)

This function finds and returns a pointer to an ext2 inode from disk. It calculates which block group the inode belongs to, finds the inode table block in that group, reads the block, and then returns a pointer to the inode within that block.

```c
static struct ext2_inode *ext2_get_inode(struct super_block *sb, ino_t ino,
                                         struct buffer_head **p)
{
    struct buffer_head *bh;
    unsigned long block_group;
    unsigned long block;
    unsigned long offset;
    struct ext2_group_desc *gdp;
    unsigned long inodes_pg = EXT2_INODES_PER_GROUP(sb);
    int inode_sz = EXT2_INODE_SIZE(sb);
    unsigned long blocksize = sb->s_blocksize;

    *p = NULL;
    /* Validate the inode number */
    if ((ino != EXT2_ROOT_INO && ino < EXT2_FIRST_INO(sb)) ||
        ino > le32_to_cpu(EXT2_SB(sb)->s_es->s_inodes_count))
        goto einval;

    /* Calculate the block group that contains the inode */
    block_group = (ino - 1) / inodes_pg;

    /* Get the group descriptor for the block group; bh will point to the block containing the descriptor */
    gdp = ext2_get_group_desc(sb, block_group, &bh);
    if (!gdp)
        goto eio;

    /* Calculate the offset index of the inode within its group */
    unsigned long index = (ino - 1) % inodes_pg;

    /* The inode table starts at block number stored in bg_inode_table; convert it from little endian */
    block = le32_to_cpu(gdp->bg_inode_table);

    /* Calculate which block in the inode table contains the inode */
    unsigned long inode_table_block = block + (index * inode_sz) / blocksize;

    /* Calculate the byte offset within that block for the inode */
    offset = (index * inode_sz) % blocksize;

    /* Read the block from disk that contains the inode */
    bh = sb_bread(sb, inode_table_block);
    if (!bh)
        goto eio;

    /* Save the buffer head pointer for later release */
    *p = bh;

    /* Return a pointer to the inode inside the block */
    return (struct ext2_inode *)(bh->b_data + offset);

einval:
    ext2_error(sb, __func__, "bad inode number: %lu", (unsigned long)ino);
    return ERR_PTR(-EINVAL);
eio:
    ext2_error(sb, __func__, "unable to read inode block - inode=%lu, block=%lu",
               (unsigned long)ino, block);
    return ERR_PTR(-EIO);
}
```

**Explanation:**

- **Validation:** Checks if the inode number is valid based on the first valid inode and the total inode count.
- **Block Group Calculation:** In ext2, inodes are divided into groups. The group is computed by `(ino - 1) / inodes_pg`.
- **Group Descriptor:** Retrieves the group descriptor for the group using `ext2_get_group_desc()`.  
- **Index Within Group:** The index inside the group is `(ino - 1) % inodes_pg`.
- **Inode Table Location:** The inode table starts at a block number given by `gdp->bg_inode_table` (converted from little-endian).  
- **Block and Offset Calculation:**  
  - The block within the inode table that contains the inode is computed using the inode size and block size.
  - The byte offset within that block is computed as `(index * inode_sz) % blocksize`.
- **Reading the Inode:**  
  - The block containing the inode is read from disk using `sb_bread()`.
  - A pointer to the inode is returned as an offset into the block’s data.

---

## 4. Block Allocation within a Block Group (ext2_allocate_in_bg in balloc.c)

This function is responsible for finding and allocating free blocks from a block group. It uses a bitmap that represents the usage of blocks in the group.

```c
static int ext2_allocate_in_bg(struct super_block *sb, int group,
                               struct buffer_head *bitmap_bh, unsigned long *count)
{
    // Calculate the first and last block numbers in the group.
    ext2_fsblk_t group_first_block = ext2_group_first_block_no(sb, group);
    ext2_fsblk_t group_last_block = ext2_group_last_block_no(sb, group);
    // Total number of blocks in this group.
    ext2_grpblk_t nblocks = group_last_block - group_first_block + 1;
    ext2_grpblk_t first_free_bit;
    unsigned long num = 0;
    unsigned long i = 0;
    unsigned long desired = *count;

    /* Find the first free bit in the block group's bitmap.
     * The bitmap is stored in bitmap_bh->b_data and nblocks indicates
     * how many bits (blocks) we want to check.
     */
    first_free_bit = find_next_zero_bit_le(bitmap_bh->b_data, nblocks, 0);

    /* If no free block is found, return -ENOSPC (No space left) */
    if (first_free_bit >= nblocks)
        return -ENOSPC;

    /* Attempt to allocate up to 'desired' blocks, starting at first_free_bit */
    for (i = first_free_bit; i < nblocks && num < desired; i++) {
        /* ext2_set_bit_atomic() sets the bit atomically.
         * It returns true if the bit was successfully set (i.e., block allocated),
         * and false if the bit was already set.
         */
        if (ext2_set_bit_atomic(sb_bgl_lock(EXT2_SB(sb), group), i, bitmap_bh->b_data)) {
            num++;  // Increment count of allocated blocks
        } else {
            break; // Stop if we cannot allocate further contiguous blocks
        }
    }

    /* If we allocated at least one block, update the count and mark the bitmap as dirty */
    if (num > 0) {
        *count = num;  // Return how many blocks were allocated

        /* Mark the bitmap buffer as dirty so that the changes will be written back to disk */
        mark_buffer_dirty(bitmap_bh);

        /* If the filesystem is mounted synchronously, force a write of the dirty buffer */
        if (sb->s_flags & SB_SYNCHRONOUS)
            sync_dirty_buffer(bitmap_bh);

        return first_free_bit;  // Return the group offset of the first allocated block
    }

    return -ENOSPC;  // No contiguous free blocks found
}
```

**Explanation:**

- **Bitmap Search:**  
  Uses `find_next_zero_bit_le()` to find the first zero bit in the bitmap, which indicates a free block.
- **Allocation Loop:**  
  Iterates from the first free bit and attempts to set (allocate) each bit atomically with `ext2_set_bit_atomic()`, counting how many blocks were allocated (up to the desired number).
- **Buffer Update:**  
  Marks the bitmap as dirty and synchronizes if necessary.
- **Return:**  
  Returns the offset of the first allocated block (relative to the group) or -ENOSPC if allocation fails.

---

## General Preparation for the Exam

When answering exam questions on this topic, keep these points in mind:

1. **Understanding Data Structures:**  
   - Know the role of the superblock, group descriptors, inodes, and bitmaps in ext2.  
   - Understand how the inode table is divided into groups and how minor numbers help locate the right sensor (or inode) in Lunix:TNG.

2. **Synchronization Mechanisms:**  
   - Explain why spinlocks are used in interrupt contexts or for protecting shared data in the driver.  
   - Understand the use of semaphores (down_interruptible/up) in per-open state protection where sleeping is allowed.

3. **Filesystem Operations:**  
   - Know how the filesystem is registered/unregistered with the kernel (register_filesystem/unregister_filesystem).  
   - Understand how ext2_get_inode() calculates the block group and offset to find an inode.

4. **Block Allocation:**  
   - Explain the process of scanning the block bitmap (using find_next_zero_bit_le) and setting bits atomically to allocate blocks.

5. **Directory Operations:**  
   - Know how directory entries are stored and iterated over (using rec_len to jump to the next entry) and how ext2_find_entry() scans the pages.

6. **Coding Style and Debugging:**  
   - Write clear inline comments and use printk() for debugging.  
   - Be prepared to explain each line of code, especially in areas marked with “/* ? */” in exam questions.

---
