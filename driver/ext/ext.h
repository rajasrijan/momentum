#ifndef EXT_H
#define EXT_H

#include <stdint.h>
#include <time.h>

#define EXT_SUPERBLOCK_OFFSET 1024
#define EXT_SUPERBLOCK_SIZE 1024

#pragma pack(push, 1)
struct superblock {
    uint32_t inode_count;
    uint32_t blk_count;
    uint32_t resv_blk_count;
    uint32_t free_blk_count;
    uint32_t free_inode_count;
    uint32_t superblock_blk_idx;
    uint32_t blk_size;
    uint32_t frag_size;
    uint32_t blk_per_blkgrp;
    uint32_t frag_per_blkgrp;
    uint32_t inode_per_blkgrp;
    uint32_t mount_time;
    uint32_t write_time;
    uint16_t mount_count;
    uint16_t mount_allowed;
    uint16_t ext_sig;
    uint16_t fs_state;
    uint16_t err_hand;
    uint16_t ver_minor;
    uint32_t last_chk;
    uint32_t int_chk;
    uint32_t os_id;
    uint32_t ver_major;
    uint16_t uid_rev_blk;
    uint16_t gid_rev_blk;
    uint32_t first_inode;
    uint16_t inode_size;
    uint16_t parent_block_group;
    uint32_t optional_features;
    uint32_t required_features;
    uint32_t needed_features;
    uint8_t fsid[16];
    uint8_t vol_name[16];
    uint8_t last_mnt_path[64];
    uint32_t compression_algo;
    uint8_t file_prealloc_blk_cnt;
    uint8_t dir_prealloc_blk_cnt;
    uint16_t unused;
    uint8_t journal_id[16];
    uint32_t journal_inode;
    uint32_t journal_device;
    uint32_t orphan_inode_list;
    uint8_t resv[788];
};

struct block_group_descriptor {
    uint32_t block_bitmap_addr;
    uint32_t inode_bitmap_addr;
    uint32_t inode_tbl_addr;
    uint16_t free_blocks_in_group;
    uint16_t free_inode_in_group;
    uint16_t dirs_in_group;
    uint8_t unused[14];
};

#define INODE_TYPE_FIFO 0x1000
#define INODE_TYPE_CDEV 0x2000
#define INODE_TYPE_DIR 0x4000
#define INODE_TYPE_BLK 0x6000
#define INODE_TYPE_FILE 0x8000
#define INODE_TYPE_SLNK 0xA000
#define INODE_TYPE_SOCK 0xC000

struct inode {
    uint16_t type_permissions;
    uint16_t uid;
    uint32_t size_lo;
    uint32_t atime;
    uint32_t ctime;
    uint32_t mtime;
    uint32_t dtime;
    uint16_t gid;
    uint16_t hard_links;
    uint32_t sector_count;
    uint32_t flags;
    uint32_t os_specific_1;
    uint32_t direct_block_pointer[12];
    uint32_t single_block_pointer;
    uint32_t double_block_pointer;
    uint32_t triple_block_pointer;
    uint32_t generation_number;
    uint32_t file_acl;
    uint32_t size_hi_dir_acl;
    uint32_t block_addr;
    uint8_t os_specific_2[12];
    size_t size()
    {
        return (((uint64_t)size_lo) | (((uint64_t)size_hi_dir_acl) << 32));
    }
    void size(uint64_t sz)
    {
        size_lo = (sz & 0xFFFFFFFF);
        size_hi_dir_acl = ((sz >> 32) & 0xFFFFFFFF);
    }
};

struct directory {
    uint32_t _inode;
    uint16_t entry_size;
    uint8_t name_sz_lo;
    uint8_t type_name_sz_hi;
    char name[0];
};

#pragma pack(pop)

#endif //EXT_H
