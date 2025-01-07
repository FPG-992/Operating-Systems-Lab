#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif


static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x67207f47, "bh_uptodate_or_lock" },
	{ 0xe8793f4f, "inode_init_owner" },
	{ 0xcb829628, "generic_fillattr" },
	{ 0x76c22ea4, "setattr_prepare" },
	{ 0x29b28905, "iget_locked" },
	{ 0x1dadd75c, "generic_block_bmap" },
	{ 0x9177b5d1, "d_instantiate" },
	{ 0xfc1a68a7, "clear_inode" },
	{ 0x162734fa, "discard_new_inode" },
	{ 0x84431843, "folio_unlock" },
	{ 0xcf04ab36, "generic_file_splice_read" },
	{ 0x40401550, "new_inode" },
	{ 0x21677e28, "init_special_inode" },
	{ 0xc573d276, "unregister_filesystem" },
	{ 0x48d88a2c, "__SCT__preempt_schedule" },
	{ 0xb15f3a65, "mark_buffer_dirty" },
	{ 0xff45eb04, "__block_write_begin" },
	{ 0x664537f9, "inode_needs_sync" },
	{ 0x28599c7f, "d_make_root" },
	{ 0xe6b35c4e, "sb_set_blocksize" },
	{ 0x68abcf24, "block_truncate_page" },
	{ 0x66929574, "read_cache_folio" },
	{ 0x5db19166, "sync_filesystem" },
	{ 0x7eb6a73f, "d_splice_alias" },
	{ 0x9f3921e8, "generic_error_remove_page" },
	{ 0xe625dd9d, "current_time" },
	{ 0x69acdf38, "memcpy" },
	{ 0xd9d8cac5, "ihold" },
	{ 0x56fac0af, "inode_dio_wait" },
	{ 0x37a0cba, "kfree" },
	{ 0xa374d7db, "generic_file_fsync" },
	{ 0xb8bd3dd7, "iput" },
	{ 0xd9486c38, "iter_file_splice_write" },
	{ 0xcb2b82ac, "generic_file_write_iter" },
	{ 0xdb060676, "register_filesystem" },
	{ 0x9546e219, "sync_inode_metadata" },
	{ 0xba8fbd64, "_raw_spin_lock" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x20cbb30a, "__percpu_counter_init" },
	{ 0x65487097, "__x86_indirect_thunk_rax" },
	{ 0xb2da7b45, "kill_block_super" },
	{ 0x92997ed8, "_printk" },
	{ 0xcc894f44, "block_invalidate_folio" },
	{ 0x4cc831f2, "generic_write_end" },
	{ 0xd0da656b, "__stack_chk_fail" },
	{ 0x26d9d845, "truncate_inode_pages_final" },
	{ 0x97a77bf6, "block_write_end" },
	{ 0x864cafb4, "generic_file_mmap" },
	{ 0xc355b602, "sync_mapping_buffers" },
	{ 0x621a006f, "__filemap_get_folio" },
	{ 0x90963cc2, "kmem_cache_alloc" },
	{ 0xc5a9d96, "invalidate_inode_buffers" },
	{ 0x7cd8d75e, "page_offset_base" },
	{ 0xefb089be, "inode_init_once" },
	{ 0xc04a30df, "__brelse" },
	{ 0xdce9985a, "inode_nohighmem" },
	{ 0xf5540edc, "generic_file_read_iter" },
	{ 0xda6360d6, "inode_maybe_inc_iversion" },
	{ 0x5d101f87, "simple_symlink_inode_operations" },
	{ 0x1a14f772, "thp_get_unmapped_area" },
	{ 0x939b6099, "kmem_cache_create_usercopy" },
	{ 0x5240ee7, "percpu_counter_batch" },
	{ 0x572971cd, "truncate_pagecache" },
	{ 0x6996c0dd, "sync_dirty_buffer" },
	{ 0x2637d480, "insert_inode_locked" },
	{ 0x5dbada8a, "page_symlink" },
	{ 0xc1bf7685, "setattr_copy" },
	{ 0x10363961, "kmem_cache_free" },
	{ 0x7b59084d, "__bh_read" },
	{ 0xd3f457f8, "mpage_writepages" },
	{ 0xf1e046cc, "panic" },
	{ 0x449ad0a7, "memcmp" },
	{ 0xbb2d7c96, "__bread_gfp" },
	{ 0xb830e212, "block_dirty_folio" },
	{ 0xbef2e7ad, "mpage_read_folio" },
	{ 0xfb578fc5, "memset" },
	{ 0x5357ba71, "buffer_migrate_folio" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x423b2643, "truncate_setsize" },
	{ 0x953e1b9e, "ktime_get_real_seconds" },
	{ 0x97651e6c, "vmemmap_base" },
	{ 0x4629334c, "__preempt_count" },
	{ 0x62db788, "__getblk_gfp" },
	{ 0x885eff4e, "generic_read_dir" },
	{ 0xdf521442, "_find_next_zero_bit" },
	{ 0xa25c0536, "page_symlink_inode_operations" },
	{ 0xdc604953, "d_instantiate_new" },
	{ 0x85df9b6c, "strsep" },
	{ 0xa254b58f, "mount_bdev" },
	{ 0x661345d8, "mpage_readahead" },
	{ 0xe138fb8c, "percpu_counter_add_batch" },
	{ 0xbf8bb359, "inode_set_ctime_current" },
	{ 0xf07460b2, "inc_nlink" },
	{ 0x44e9a829, "match_token" },
	{ 0x52b41c7f, "__folio_put" },
	{ 0x7941abec, "__percpu_down_read" },
	{ 0x453dcd4d, "__folio_lock" },
	{ 0xc12c92fb, "is_bad_inode" },
	{ 0xe5c43f8e, "seq_puts" },
	{ 0x668535e, "rcuwait_wake_up" },
	{ 0x8ed7d7b4, "block_write_begin" },
	{ 0x348120aa, "block_is_partially_uptodate" },
	{ 0x4c1b64b8, "kmalloc_trace" },
	{ 0xefd1d3a0, "filemap_write_and_wait_range" },
	{ 0x60a13e90, "rcu_barrier" },
	{ 0x754d539c, "strlen" },
	{ 0xac537ac2, "percpu_counter_destroy" },
	{ 0xc7a8a61f, "__mark_inode_dirty" },
	{ 0xca8beb29, "generic_file_llseek" },
	{ 0xb5b54b34, "_raw_spin_unlock" },
	{ 0xbb3305af, "make_bad_inode" },
	{ 0x2ba10500, "sb_min_blocksize" },
	{ 0xb3be43d1, "drop_nlink" },
	{ 0xeb233a45, "__kmalloc" },
	{ 0xe2c17b5d, "__SCT__might_resched" },
	{ 0x767648eb, "kmalloc_caches" },
	{ 0xc851ccc5, "iget_failed" },
	{ 0x3f37a918, "kmem_cache_destroy" },
	{ 0x9b9daf0, "module_layout" },
};

MODULE_INFO(depends, "");

