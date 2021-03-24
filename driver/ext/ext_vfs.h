/*
 * Copyright 2009-2021 Srijan Kumar Sharma
 *
 * This file is part of Momentum.
 *
 * Momentum is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Momentum is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Momentum.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EXT_VFS_H
#define EXT_VFS_H
#include "ext.h"
#include <kernel/vfs.h>
#include <utility>
#include <kernel/bitmap.h>

class ext_vfs : public vfs
{
    friend class ext_vnode;

  protected:
    superblock superblk;
    vector<block_group_descriptor> block_group_descriptor_tbl;
    vector<pair<uint32_t, inode *>> inode_table;
    vector<bitmap> inode_map;
    vector<bitmap> block_map;

    shared_ptr<vnode> dev;
    uint64_t sector_sz = 0;
    uint64_t blk_sz = 0;

  public:
    ext_vfs(/* args */);
    ~ext_vfs();
    int mount(uint64_t flags, shared_ptr<vnode> blk_dev, shared_ptr<vnode> &fs_root_directory);
    int unmount(void);
    int root(vnode *&rootNode);
    int statfs(shared_ptr<vnode> rootNode, statfs_t &statfs);
    int sync(void);
    int fid(void);
    int vget(void);

  protected:
    int flush_inode_bitmap();
    int flush_block_bitmap();
    int write_superblock();

    int read_block(size_t block_id, shared_ptr<char> &buffer);
    int write_block(size_t block_id, shared_ptr<char> &buffer);

    int read_inode(size_t inode_id, inode **_inode);
    int write_inode(size_t inode_id, inode *_inode);

    int allocate_inode(size_t &inode_id, bool is_dir);
    int free_inode(size_t inode_id);
    int allocate_block(size_t &block_id);
    int free_block(size_t block_id);
};

#endif