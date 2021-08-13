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

#include <stddef.h>
#include "ext_vfs.h"
#include "ext_vnode.h"
#include <kernel/logging.h>
#include <algorithm>

ext_vfs::ext_vfs(/* args */)
{
}

ext_vfs::~ext_vfs()
{
}

int ext_vfs::mount(uint64_t flags, shared_ptr<vnode> blk_dev, shared_ptr<vnode> &fs_root_directory)
{
    int ret = 0;
    uint32_t blkgrp_count = 0;

    dev = blk_dev;

    superblk = {};

    ret = blk_dev->ioctl(BLKPBSZGET, &sector_sz, 0);
    if (ret < 0)
    {
        log_error("ioctl failed!\n");
        return ret;
    }

    //  one sector long buffer
    shared_ptr<char> buffer = new char[sector_sz];
    uint64_t remaining_size = 0;
    for (size_t i = 0; i < sizeof(superblock); i += sector_sz)
    {
        ret = blk_dev->bread((EXT_SUPERBLOCK_OFFSET + i) / sector_sz, 1, buffer.get(), nullptr);
        if (ret < 0)
        {
            log_error("failed to read!\n");
            return ret;
        }
        remaining_size = sizeof(superblock) - i;
        memcpy((char *)&superblk + i, buffer.get(), min(sector_sz, remaining_size));
    }
    if (superblk.ext_sig != 0xef53)
    {
        log_error("not a valid ext partition!\n");
        return -EINVAL;
    }
    blk_sz = (1024 << superblk.blk_size);

    blkgrp_count = (superblk.blk_count + superblk.blk_per_blkgrp - 1) / superblk.blk_per_blkgrp;

    //  read the block group descriptor table
    block_group_descriptor_tbl.resize(blkgrp_count);
    size_t blk_grp_desc_tbl_idx = superblk.superblock_blk_idx + 1;

    //  it's located right after the superblock
    for (size_t i = 0; i < sizeof(block_group_descriptor) * blkgrp_count; i += sector_sz)
    {
        ret = blk_dev->bread(((blk_grp_desc_tbl_idx * blk_sz) + i) / sector_sz, 1, buffer.get(), nullptr);
        if (ret < 0)
        {
            log_error("failed to read!\n");
            return ret;
        }
        remaining_size = sizeof(block_group_descriptor) * blkgrp_count - i;
        memcpy((char *)block_group_descriptor_tbl.data() + i, buffer.get(), min(sector_sz, remaining_size));
    }

    //  load inode bitmap
    buffer = new char[blk_sz];
    remaining_size = 0;

    //  load inode bitmap
    for (uint32_t blk_idx = 0; blk_idx < blkgrp_count; blk_idx++)
    {
        inode_map.emplace_back(superblk.inode_per_blkgrp);
        for (size_t i = 0; i < ((superblk.inode_per_blkgrp + 7) / 8); i += blk_sz)
        {
            ret = read_block(block_group_descriptor_tbl[blk_idx].inode_bitmap_addr + (i / blk_sz), buffer);
            remaining_size = ((superblk.inode_per_blkgrp + 7) / 8) - i;
            memcpy(inode_map[blk_idx].get_ptr() + i, buffer.get(), min(remaining_size, blk_sz));
        }
    }

    // load block bitmap
    remaining_size = 0;

    //  load block bitmap
    for (uint32_t blk_idx = 0; blk_idx < blkgrp_count; blk_idx++)
    {
        block_map.emplace_back(superblk.blk_per_blkgrp);
        for (size_t i = 0; i < ((superblk.blk_per_blkgrp + 7) / 8); i += blk_sz)
        {
            ret = read_block(block_group_descriptor_tbl[blk_idx].block_bitmap_addr + (i / blk_sz), buffer);
            remaining_size = ((superblk.blk_per_blkgrp + 7) / 8) - i;
            memcpy(block_map[blk_idx].get_ptr() + i, buffer.get(), min(remaining_size, blk_sz));
        }
    }

    fs_root_directory = make_shared<ext_vnode>(this, 2);

    return ret;
}

int ext_vfs::unmount(void)
{
    return -ENOSYS;
}

int ext_vfs::root(vnode *&rootNode)
{
    return -ENOSYS;
}

int ext_vfs::statfs(shared_ptr<vnode> rootNode, statfs_t &statfs)
{
    memcpy(&(statfs.fsid), superblk.fsid, sizeof(uuid_t));
    return 0;
}

int ext_vfs::sync(void)
{
    return -ENOSYS;
}

int ext_vfs::fid(void)
{
    return -ENOSYS;
}

int ext_vfs::vget(void)
{
    return -ENOSYS;
}

int ext_vfs::write_superblock()
{
    shared_ptr<char> buffer = new char[blk_sz];
    memcpy(buffer.get() + EXT_SUPERBLOCK_OFFSET, (char *)&superblk, sizeof(superblock));
    return write_block(superblk.superblock_blk_idx, buffer);
}

int ext_vfs::read_inode(size_t inode_id, inode **_inode)
{
    int ret = 0;

    auto it = find_if(inode_table.begin(), inode_table.end(), [inode_id](const pair<uint32_t, inode *> &v) { return v.first == inode_id; });
    if (it != inode_table.end())
    {
        *_inode = it->second;
        return 0;
    }

    shared_ptr<char> buffer = new char[blk_sz];
    uint32_t blk_grp = (inode_id - 1) / superblk.inode_per_blkgrp;
    uint32_t inode_idx = (inode_id - 1) % superblk.inode_per_blkgrp;

    uint32_t inode_tbl_idx = (inode_idx * superblk.inode_size) / blk_sz;

    ret = read_block(block_group_descriptor_tbl[blk_grp].inode_tbl_addr + inode_tbl_idx, buffer);

    *_inode = (inode *)new char[superblk.inode_size];

    inode_table.emplace_back(inode_id, *_inode);

    uint32_t blk_offset = (inode_idx * superblk.inode_size) % blk_sz;
    memcpy(*_inode, (inode *)(buffer.get() + blk_offset), superblk.inode_size);
    return ret;
}

int ext_vfs::write_inode(size_t inode_id, inode *_inode)
{
    int ret = 0;

    auto it = find_if(inode_table.begin(), inode_table.end(), [inode_id](const pair<uint32_t, inode *> &v) { return v.first == inode_id; });
    //  exists
    if (it != inode_table.end())
    {
        // pointer is different
        if (_inode != it->second)
        {
            delete it->second;
            it->second = _inode;
        }
    }
    else
    {
        //  add if not in cache
        inode_table.emplace_back(inode_id, _inode);
    }

    shared_ptr<char> buffer = new char[blk_sz];
    uint32_t blk_grp = (inode_id - 1) / superblk.inode_per_blkgrp;
    uint32_t inode_idx = (inode_id - 1) % superblk.inode_per_blkgrp;

    uint32_t inode_tbl_idx = (inode_idx * superblk.inode_size) / blk_sz;

    ret = read_block(block_group_descriptor_tbl[blk_grp].inode_tbl_addr + inode_tbl_idx, buffer);

    uint32_t blk_offset = (inode_idx * superblk.inode_size) % blk_sz;
    //  overwrite old inode with new one
    memcpy((inode *)(buffer.get() + blk_offset), _inode, superblk.inode_size);
    ret = write_block(block_group_descriptor_tbl[blk_grp].inode_tbl_addr + inode_tbl_idx, buffer);
    return ret;
}

int ext_vfs::read_block(size_t block_id, shared_ptr<char> &buffer)
{
    return dev->bread((block_id * blk_sz) / sector_sz, blk_sz / sector_sz, buffer.get(), nullptr);
}

int ext_vfs::write_block(size_t block_id, shared_ptr<char> &buffer)
{
    return dev->bwrite((block_id * blk_sz) / sector_sz, blk_sz / sector_sz, buffer.get(), nullptr);
}

int ext_vfs::allocate_inode(size_t &inode_id, bool is_dir)
{
    int ret = 0;
    for (size_t bidx = 0; bidx < block_group_descriptor_tbl.size(); bidx++)
    {
        auto &inode_bmap = inode_map[bidx];
        if ((ret = inode_bmap.alloc_bit(inode_id)) == 0)
        {
            inode_id++;
            superblk.free_inode_count--;
            block_group_descriptor_tbl[bidx].free_inode_in_group--;
            block_group_descriptor_tbl[bidx].dirs_in_group++;
            return 0;
        }
    }
    return -EIO;
}

int ext_vfs::allocate_block(size_t &block_id)
{
    int ret = 0;
    for (size_t bidx = 0; bidx < block_group_descriptor_tbl.size(); bidx++)
    {
        auto &block_bmap = block_map[bidx];
        if ((ret = block_bmap.alloc_bit(block_id)) == 0)
        {
            superblk.free_blk_count--;
            block_group_descriptor_tbl[bidx].free_blocks_in_group--;
            return 0;
        }
    }
    return -EIO;
}
int ext_vfs::flush_inode_bitmap()
{
    int ret = 0;
    shared_ptr<char> buffer = new char[blk_sz];
    uint64_t remaining_size = 0;

    for (uint32_t blk_idx = 0; blk_idx < inode_map.size(); blk_idx++)
    {
        for (size_t i = 0; i < ((superblk.inode_per_blkgrp + 7) / 8); i += blk_sz)
        {
            remaining_size = ((superblk.inode_per_blkgrp + 7) / 8) - i;
            memcpy(buffer.get(), inode_map[blk_idx].get_ptr() + i, min(remaining_size, blk_sz));
            ret = write_block(block_group_descriptor_tbl[blk_idx].inode_bitmap_addr + (i / blk_sz), buffer);
        }
    }
    return ret;
}
int ext_vfs::flush_block_bitmap()
{
    int ret = 0;
    shared_ptr<char> buffer = new char[blk_sz];
    uint64_t remaining_size = 0;

    for (uint32_t blk_idx = 0; blk_idx < block_map.size(); blk_idx++)
    {
        for (size_t i = 0; i < ((superblk.blk_per_blkgrp + 7) / 8); i += blk_sz)
        {
            remaining_size = ((superblk.blk_per_blkgrp + 7) / 8) - i;
            memcpy(buffer.get(), block_map[blk_idx].get_ptr() + i, min(remaining_size, blk_sz));
            ret = write_block(block_group_descriptor_tbl[blk_idx].block_bitmap_addr + (i / blk_sz), buffer);
        }
    }

    //  it's located right after the superblock
    size_t blk_grp_desc_tbl_idx = superblk.superblock_blk_idx + 1;
    for (size_t i = 0; i < sizeof(block_group_descriptor) * block_group_descriptor_tbl.size(); i += sector_sz)
    {
        remaining_size = (sizeof(block_group_descriptor) * block_group_descriptor_tbl.size()) - i;
        memcpy(buffer.get(), (char *)block_group_descriptor_tbl.data() + i, min(sector_sz, remaining_size));
        ret = dev->bwrite(((blk_grp_desc_tbl_idx * blk_sz) + i) / sector_sz, 1, buffer.get(), nullptr);
        if (ret < 0)
        {
            log_error("failed to write!\n");
            return ret;
        }
    }
    return ret;
}