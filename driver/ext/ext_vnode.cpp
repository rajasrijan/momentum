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

#include <kernel/logging.h>
#include <stddef.h>
#include "ext_vnode.h"
#include <algorithm>

directory *make_entry(directory *ent, const std::string &name, uint32_t _inode_id)
{
    size_t ent_sz = ((sizeof(directory) + name.size() + 3) / 4) * 4;
    ent->_inode = _inode_id;
    ent->entry_size = ent_sz;
    ent->name_sz_lo = name.size();
    ent->type_name_sz_hi = 2;
    memcpy(ent->name, name.c_str(), name.size());
    return ent;
}

directory *make_entry(const std::string &name, uint32_t _inode_id)
{
    size_t ent_sz = ((sizeof(directory) + name.size() + 3) / 4) * 4;
    directory *ent = (directory *)(new char[ent_sz]);
    return make_entry(ent, name, _inode_id);
}
size_t directory_struct_size(const directory *ent)
{
    return ((sizeof(directory) + ent->name_sz_lo + 3) / 4) * 4;
}

directory *next_directory_entry(directory *ent)
{
    return (directory *)((char *)ent + ent->entry_size);
}

directory *insert_entry_after(directory *ent, const std::string &name, uint32_t _inode_id)
{
    size_t new_ent_sz = ((sizeof(directory) + name.size() + 3) / 4) * 4;
    if (ent->entry_size < new_ent_sz + directory_struct_size(ent))
        return nullptr;
    auto new_sz = ent->entry_size - directory_struct_size(ent);
    ent->entry_size = directory_struct_size(ent);
    ent = next_directory_entry(ent);
    ent = make_entry(ent, name, _inode_id);
    ent->entry_size = new_sz;
    return ent;
}

ext_vnode::ext_vnode(ext_vfs *vfsp, size_t _inode_id) : vnode(vfsp), inode_id(_inode_id), _inode(nullptr)
{
    vfsp->read_inode(inode_id, &_inode);

    switch ((_inode->type_permissions & 0xF000))
    {
    case INODE_TYPE_FIFO:
        v_type = VNON;
        break;
    case INODE_TYPE_CDEV:
        v_type = VCHR;
        break;
    case INODE_TYPE_DIR:
        v_type = VDIR;
        break;
    case INODE_TYPE_BLK:
        v_type = VBLK;
        break;
    case INODE_TYPE_FILE:
        v_type = VREG;
        break;
    case INODE_TYPE_SLNK:
        v_type = VLNK;
        break;
    case INODE_TYPE_SOCK:
        v_type = VSOCK;
        break;
    default:
        log_error("Unknown inode type\n");
        asm("cli;hlt");
        break;
    }
}

ext_vnode::~ext_vnode()
{
}

int ext_vnode::bread(ssize_t position, size_t size, char *data, int *bytesRead)
{
    int ret = 0;
    ext_vfs *v_ext_vfs = (ext_vfs *)v_vfsp;
    size_t read_count = 0;
    shared_ptr<char> buffer = new char[v_ext_vfs->blk_sz];
    size_t blkIdx = 0;
    while (get_block_id_from_position(position, blkIdx) == 0)
    {
        v_ext_vfs->read_block(blkIdx, buffer);
        auto blk_offset = position % v_ext_vfs->blk_sz;
        auto cpy_sz = min((uint64_t)size - read_count, v_ext_vfs->blk_sz - blk_offset);
        memcpy(&data[read_count], &(buffer.get()[blk_offset]), cpy_sz);
        read_count += cpy_sz;
        position += cpy_sz;
        if (read_count >= size)
        {
            break;
        }
    }
    if (bytesRead)
        *bytesRead = read_count;

    return ret;
}

int ext_vnode::bwrite(ssize_t position, size_t size, const char *data, int *bytesWritten)
{
    return -ENOSYS;
}

int ext_vnode::readdir(vector<shared_ptr<vnode>> &vnodes)
{
    int ret = 0;
    if (v_type != VDIR)
        return -ENOTDIR;

    auto end = directory_it_end();
    for (auto current = directory_it_begin(); current != end; ++current)
    {
        directory &dir = *current;
        string name;
        name.resize(dir.name_sz_lo);
        memcpy(name.c_str(), dir.name, dir.name_sz_lo);

        auto tmp = new ext_vnode((ext_vfs *)v_vfsp, dir._inode);
        vnodes.emplace_back(tmp);
        tmp->setName(name);
    }
    return ret;
}

int ext_vnode::mkdir(std::string name, std::shared_ptr<vnode> &pDir)
{
    int ret = 0;
    if (v_type != VDIR)
        return -ENOTDIR;
    auto size = _inode->size();
    ext_vfs *v_ext_vfs = (ext_vfs *)v_vfsp;
    auto blk_sz = v_ext_vfs->blk_sz;

    //  allocate new directory stuff
    size_t new_inode = 0;
    size_t new_block = 0;
    inode *new_dir_inode = nullptr;
    ret = v_ext_vfs->allocate_inode(new_inode, true);
    ret = v_ext_vfs->read_inode(new_inode, &new_dir_inode);
    memset(new_dir_inode, 0, v_ext_vfs->superblk.inode_size);

    new_dir_inode->type_permissions = INODE_TYPE_DIR;
    ret = v_ext_vfs->allocate_block(new_block);
    new_dir_inode->direct_block_pointer[0] = new_block;
    new_dir_inode->size(blk_sz);
    new_dir_inode->sector_count = new_dir_inode->size() / v_ext_vfs->sector_sz;
    new_dir_inode->hard_links = 2;

    shared_ptr<char> buffer(new char[blk_sz]);
    directory *new_dir = nullptr;

    size_t remaining = 0;
    while (remaining < size)
    {

        ret = read_block(remaining / blk_sz, buffer);
        if (ret < 0)
        {
            log_error("Failed to read from disk\n");
            return -EIO;
        }

        directory *dir = (directory *)buffer.get();
        for (size_t idx = 0; idx < blk_sz; idx += dir->entry_size)
        {
            dir = (directory *)(buffer.get() + idx);
            new_dir = insert_entry_after(dir, name, new_inode);
            if (new_dir)
                break;
        }
        if (new_dir)
        {
            ret = write_block(remaining / blk_sz, buffer);
            if (ret < 0)
            {
                log_error("Failed to write to disk\n");
                return -EIO;
            }
            break;
        }
        remaining += blk_sz;
    }
    if (!new_dir)
        return -ENOFILE;
    memset(buffer.get(), 0, blk_sz);
    auto tmp_dir = make_entry((directory *)buffer.get(), ".", new_inode);
    tmp_dir->entry_size = blk_sz;
    tmp_dir = insert_entry_after(tmp_dir, "..", inode_id);
    ret = v_ext_vfs->write_block(new_dir_inode->direct_block_pointer[0], buffer);
    if (ret < 0)
    {
        log_error("Failed to write to disk\n");
        return -EIO;
    }
    _inode->hard_links++;
    ret = v_ext_vfs->write_inode(inode_id, _inode);
    ret = v_ext_vfs->write_inode(new_inode, new_dir_inode);
    ret = v_ext_vfs->flush_inode_bitmap();
    ret = v_ext_vfs->flush_block_bitmap();
    ret = v_ext_vfs->write_superblock();

    pDir = make_shared<ext_vnode>((ext_vfs *)v_vfsp, new_inode);
    ((ext_vnode *)pDir.get())->setName(name.c_str());
    return ret;
}

int ext_vnode::open(uint64_t flags)
{
    if (v_type == VDIR)
        return -EISDIR;
    return -ENOSYS;
}

int ext_vnode::lookup(char const *path, shared_ptr<vnode> &node)
{
    if (v_type != VDIR)
        return -ENOTDIR;
    auto end = directory_it_end();
    for (auto current = directory_it_begin(); current != end; ++current)
    {
        directory &dir = *current;
        string name;
        name.resize(dir.name_sz_lo);
        memcpy(name.c_str(), dir.name, dir.name_sz_lo);
        if (name == path)
        {
            ext_vnode *tmp = new ext_vnode((ext_vfs *)v_vfsp, dir._inode);
            tmp->setName(name.c_str());
            node = (vnode *)tmp;
            return 0;
        }
    }
    return -ENOFILE;
}

int ext_vnode::read_block(uint32_t block_index, shared_ptr<char> &buffer)
{
    int ret = 0;
    auto size = _inode->size();
    ext_vfs *v_ext_vfs = (ext_vfs *)v_vfsp;
    auto blk_sz = v_ext_vfs->blk_sz;

    if ((block_index * blk_sz) >= size)
        return -EINVAL;
    if (block_index >= 12)
        return -ENOSYS;
    ret = v_ext_vfs->read_block(_inode->direct_block_pointer[block_index], buffer);
    if (ret < 0)
    {
        log_error("Failed to read from disk\n");
        return -EIO;
    }
    return ret;
}

int ext_vnode::write_block(uint32_t block_index, shared_ptr<char> &buffer)
{
    int ret = 0;
    auto size = _inode->size();
    ext_vfs *v_ext_vfs = (ext_vfs *)v_vfsp;
    auto blk_sz = v_ext_vfs->blk_sz;

    if ((block_index * blk_sz) >= size)
        return -EINVAL;
    if (block_index >= 12)
        return -ENOSYS;
    ret = v_ext_vfs->write_block(_inode->direct_block_pointer[block_index], buffer);
    if (ret < 0)
    {
        log_error("Failed to write to disk\n");
        return -EIO;
    }
    return ret;
}

int ext_vnode::create(const string &path, shared_ptr<vnode> &created_node)
{
    int ret = -ENOSYS;
    if (v_type != VDIR)
    {
        return -ENOTDIR;
    }
    ext_vfs *v_ext_vfs = (ext_vfs *)v_vfsp;
    size_t new_inode_id = 0;
    inode *new_inode = nullptr;
    ret = v_ext_vfs->allocate_inode(new_inode_id, false);
    ret = v_ext_vfs->read_inode(new_inode_id, &new_inode);
    memset(new_inode, 0, v_ext_vfs->superblk.inode_size);
    directory *new_dir_ent = make_entry(path, new_inode_id);

    auto current = directory_it_begin();
    auto end = directory_it_end();

    //  find an empty directory entry
    current = find_if(current, end, [new_dir_ent](const directory &dir) { return !dir.is_allocated() && (dir.entry_size == new_dir_ent->entry_size); });
    if (current != end)
        goto error_exit;
error_exit:
    free(new_dir_ent);
    return ret;
}

ext_vnode::ext_dir_iterator ext_vnode::directory_it_begin()
{
    return ext_vnode::ext_dir_iterator(this, 0u);
}

ext_vnode::ext_dir_iterator ext_vnode::directory_it_end()
{
    return ext_vnode::ext_dir_iterator(this, _inode->size());
}

ext_vnode::ext_dir_iterator::ext_dir_iterator(ext_vnode *_parent_vnode, size_t _offset) : parent_vnode(_parent_vnode), offset(_offset)
{
    ext_vfs *v_ext_vfs = (ext_vfs *)parent_vnode->v_vfsp;
    if (offset < parent_vnode->_inode->size())
    {
        buffer = new char[v_ext_vfs->blk_sz];
        parent_vnode->read_block(offset / v_ext_vfs->blk_sz, buffer);
    }
}

ext_vnode::ext_dir_iterator::~ext_dir_iterator()
{
}

void ext_vnode::ext_dir_iterator::operator++()
{
    ext_vfs *v_ext_vfs = (ext_vfs *)parent_vnode->v_vfsp;
    const auto &dir = this->operator*();
    auto new_offset = offset + dir.entry_size;
    //  if offset crossed over to next block then read next block
    if ((new_offset / v_ext_vfs->blk_sz) != (offset / v_ext_vfs->blk_sz))
    {
        parent_vnode->read_block(offset / v_ext_vfs->blk_sz, buffer);
    }
    offset = new_offset;
}

bool ext_vnode::ext_dir_iterator::operator!=(const ext_dir_iterator &rhs)
{
    return offset != rhs.offset;
}

void ext_vnode::ext_dir_iterator::operator=(const ext_dir_iterator &rhs)
{
    parent_vnode = rhs.parent_vnode;
    offset = rhs.offset;
    buffer = rhs.buffer;
}

directory &ext_vnode::ext_dir_iterator::operator*()
{
    ext_vfs *v_ext_vfs = (ext_vfs *)parent_vnode->v_vfsp;
    auto i = offset % v_ext_vfs->blk_sz;
    return *((directory *)&(buffer.get()[i]));
}

int ext_vnode::get_block_id_from_position(size_t pos, size_t &blk_id)
{
    int ret = 0;
    ext_vfs *v_ext_vfs = (ext_vfs *)v_vfsp;
    const auto direct_block_count = sizeof(_inode->direct_block_pointer) / sizeof(_inode->direct_block_pointer[0]);
    const auto single_block_count = v_ext_vfs->blk_sz / sizeof(_inode->direct_block_pointer[0]);
    auto blk_idx = pos / v_ext_vfs->blk_sz;
    if (blk_idx < direct_block_count)
    {
        blk_id = _inode->direct_block_pointer[blk_idx];
        return 0;
    }
    else if ((blk_idx - direct_block_count) < single_block_count)
    {
        std::shared_ptr<char> buffer = new char[v_ext_vfs->blk_sz];
        ret = v_ext_vfs->read_block(_inode->single_block_pointer, buffer);
        if (ret != 0)
        {
            return ret;
        }
        blk_id = ((uint32_t *)buffer.get())[(blk_idx - direct_block_count)];
        return 0;
    }
    return -EOF;
}

bool directory::is_allocated() const
{
    return (type_name_sz_hi & 2) == 2;
}
