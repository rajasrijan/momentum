#include <stddef.h>
#include "ext_vnode.h"
#include <kernel/logging.h>

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

ext_vnode::ext_vnode(ext_vfs *vfsp, size_t _inode_id)
    : vnode(vfsp), inode_id(_inode_id), _inode(nullptr)
{
    vfsp->read_inode(inode_id, &_inode);

    switch ((_inode->type_permissions & 0xF000)) {
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
    return -ENOSYS;
}
int ext_vnode::bwrite(ssize_t position, size_t size, char *data, int *bytesWritten)
{
    return -ENOSYS;
}
int ext_vnode::readdir(vector<shared_ptr<vnode>> &vnodes)
{
    int ret = 0;
    if (v_type != VDIR)
        return -ENOTDIR;
    auto size = _inode->size();
    auto blk_sz = ((ext_vfs *)v_vfsp)->blk_sz;

    shared_ptr<char> buffer(new char[blk_sz]);

    size_t remaining = 0;
    while (remaining < size) {
        ret = read_block(remaining / blk_sz, buffer);
        if (ret < 0) {
            log_error("Failed to read from disk\n");
            return -EIO;
        }

        directory *dir = (directory *)buffer.get();
        for (size_t idx = 0; idx < blk_sz; idx += dir->entry_size) {
            dir = (directory *)(buffer.get() + idx);
            string name;
            name.resize(dir->name_sz_lo);
            memcpy(name.c_str(), dir->name, dir->name_sz_lo);

            auto tmp = new ext_vnode((ext_vfs *)v_vfsp, dir->_inode);
            vnodes.emplace_back(tmp);
            tmp->setName(name);
        }
        remaining += blk_sz;
    }

    return ret;
}

int ext_vnode::mkdir(std::string name, std::shared_ptr<vnode> &pDir)
{
    int ret = 0;
    if (v_type != VDIR)
        return -ENOTDIR;
    auto size = _inode->size();
    auto blk_sz = ((ext_vfs *)v_vfsp)->blk_sz;

    //  allocate new directory stuff
    size_t new_inode = 0;
    size_t new_block = 0;
    inode *new_dir_inode = nullptr;
    ret = ((ext_vfs *)v_vfsp)->allocate_inode(new_inode);
    ret = ((ext_vfs *)v_vfsp)->read_inode(new_inode, &new_dir_inode);
    memset(new_dir_inode, 0, ((ext_vfs *)v_vfsp)->superblk.inode_size);

    new_dir_inode->type_permissions = INODE_TYPE_DIR;
    ret = ((ext_vfs *)v_vfsp)->allocate_block(new_block);
    new_dir_inode->direct_block_pointer[0] = new_block;
    new_dir_inode->size(blk_sz);
    new_dir_inode->sector_count = new_dir_inode->size() / ((ext_vfs *)v_vfsp)->sector_sz;
    new_dir_inode->hard_links = 2;

    shared_ptr<char> buffer(new char[blk_sz]);
    directory *new_dir = nullptr;

    size_t remaining = 0;
    while (remaining < size) {

        ret = read_block(remaining / blk_sz, buffer);
        if (ret < 0) {
            log_error("Failed to read from disk\n");
            return -EIO;
        }

        directory *dir = (directory *)buffer.get();
        for (size_t idx = 0; idx < blk_sz; idx += dir->entry_size) {
            dir = (directory *)(buffer.get() + idx);
            new_dir = insert_entry_after(dir, name, new_inode);
            if (new_dir)
                break;
        }
        if (new_dir) {
            ret = write_block(remaining / blk_sz, buffer);
            if (ret < 0) {
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
    ret = ((ext_vfs *)v_vfsp)->write_block(new_dir_inode->direct_block_pointer[0], buffer);
    if (ret < 0) {
        log_error("Failed to write to disk\n");
        return -EIO;
    }
    _inode->hard_links++;
    ret = ((ext_vfs *)v_vfsp)->write_inode(inode_id, _inode);
    ret = ((ext_vfs *)v_vfsp)->write_inode(new_inode, new_dir_inode);
    ret = ((ext_vfs *)v_vfsp)->flush_inode_bitmap();
    ret = ((ext_vfs *)v_vfsp)->flush_block_bitmap();
    ret = ((ext_vfs *)v_vfsp)->write_superblock();
    
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
    int ret = 0;
    if (v_type != VDIR)
        return -ENOTDIR;
    auto size = _inode->size();
    auto blk_sz = ((ext_vfs *)v_vfsp)->blk_sz;

    shared_ptr<char> buffer(new char[blk_sz]);

    size_t remaining = 0;
    while (remaining < size) {

        ret = read_block(remaining / blk_sz, buffer);
        if (ret < 0) {
            log_error("Failed to read from disk\n");
            return -EIO;
        }

        directory *dir = (directory *)buffer.get();
        for (size_t idx = 0; idx < blk_sz; idx += dir->entry_size) {
            dir = (directory *)(buffer.get() + idx);
            string name;
            name.resize(dir->name_sz_lo);
            memcpy(name.c_str(), dir->name, dir->name_sz_lo);
            if (name == path) {
                ext_vnode *tmp = new ext_vnode((ext_vfs *)v_vfsp, dir->_inode);
                tmp->setName(name.c_str());
                node = (vnode *)tmp;
                return 0;
            }
        }
        remaining += blk_sz;
    }

    return -ENOFILE;
}

int ext_vnode::read_block(uint32_t block_index, shared_ptr<char> &buffer)
{
    int ret = 0;
    auto size = _inode->size();
    auto blk_sz = ((ext_vfs *)v_vfsp)->blk_sz;

    if ((block_index * blk_sz) >= size)
        return -EINVAL;
    if (block_index >= 12)
        return -ENOSYS;
    ret = ((ext_vfs *)v_vfsp)->read_block(_inode->direct_block_pointer[block_index], buffer);
    if (ret < 0) {
        log_error("Failed to read from disk\n");
        return -EIO;
    }
    return ret;
}

int ext_vnode::write_block(uint32_t block_index, shared_ptr<char> &buffer)
{
    int ret = 0;
    auto size = _inode->size();
    auto blk_sz = ((ext_vfs *)v_vfsp)->blk_sz;

    if ((block_index * blk_sz) >= size)
        return -EINVAL;
    if (block_index >= 12)
        return -ENOSYS;
    ret = ((ext_vfs *)v_vfsp)->write_block(_inode->direct_block_pointer[block_index], buffer);
    if (ret < 0) {
        log_error("Failed to write to disk\n");
        return -EIO;
    }
    return ret;
}