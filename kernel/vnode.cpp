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

#include <errno.h>
#include <string>
#include <memory>
#include <map>
#include <kernel/logging.h>
#include <kernel/vfs.h>
#include <kernel/vnode.h>

using namespace std;

map<string, vnode *> dev_list; //	List of all dev in system.

vnode::vnode(class vfs *vfsp)
    : v_shlockc(0), v_exlockc(0), v_type(0), v_vfsmountedhere(nullptr), v_vfsp(vfsp), v_flag(0), v_name(), v_count(0), special_nodes(), dnode_cache(), ref_nodes()
{
}

vnode::~vnode()
{
    log_info("vnode deleted\n");
}

const string &vnode::getName() const
{
    return v_name;
}

int vnode::ioctl(unsigned int, void *, int)
{
    return -ENOSYS;
}

int vnode::dolookup(const char *const path, shared_ptr<vnode> &foundNode)
{
    char sub_path[256] = {0};
    // copy till '/' or eos.
    for (size_t i = 0; path[i] != 0 && path[i] != '/'; i++) {
        sub_path[i] = path[i];
    }
    //  look in dnode cache
    auto found_itr = find_if(dnode_cache.begin(), dnode_cache.end(), [sub_path](const shared_ptr<vnode> &node) {
        return !strcmp(node->v_name.c_str(), sub_path);
    });
    if (found_itr != dnode_cache.end()) {
        foundNode = *found_itr;
        return 0;
    }
    //  look in special files
    found_itr = find_if(special_nodes.begin(), special_nodes.end(), [sub_path](const shared_ptr<vnode> &node) {
        return !strcmp(node->v_name.c_str(), sub_path);
    });
    if (found_itr != special_nodes.end()) {
        foundNode = *found_itr;
        return 0;
    }
    int ret = 0;
    if (v_vfsmountedhere && v_vfsmountedhere->vfs_vnodecovered != nullptr) {
        ret = v_vfsmountedhere->vfs_vnodecovered->dolookup(path, foundNode);
    } else {
        ret = lookup(path, foundNode);
    }
    if (!ret) {
        dnode_cache.push_back(foundNode);
    }
    return ret;
}

int vnode::docreate(const string &path, shared_ptr<vnode> &node)
{
    int ret = 0;
    ret = create(path, node);
    return ret;
}

int vnode::doreaddir(vector<shared_ptr<vnode>> &vnodes)
{
    int ret = 0;
    if (runOnce && !dnode_cache.empty()) {
        vnodes = dnode_cache;
        goto exit;
    }
    if (v_vfsmountedhere && v_vfsmountedhere->vfs_vnodecovered != nullptr)
        ret = v_vfsmountedhere->vfs_vnodecovered->doreaddir(vnodes);
    else
        ret = readdir(vnodes);
    if (ret) {
        return ret;
    }
    //	add to cache
    for (auto &var : vnodes) {
        if (dnode_cache.end() == find_if(dnode_cache.begin(), dnode_cache.end(), [&var](const shared_ptr<vnode> &node) {
                return (node->v_name == var->getName());
            })) {
            dnode_cache.push_back(var);
        }
    }
    runOnce = true;
exit:
    for (auto &var : special_nodes) {
        vnodes.push_back(var);
    }
    return ret;
}
int vnode::doopen(shared_ptr<vnode> &node, uint32_t flags, vfile **file)
{
    if (node != this || !file)
        return -EINVAL;
    *file = new vfile(node);
    return 0;
}
int vnode::lookup(char const *, shared_ptr<vnode> &)
{
    return -ENOSYS;
}
int vnode::mkdir(string name, shared_ptr<vnode> &pDir)
{
    return -ENOSYS;
}
int vnode::rename(string name)
{
    return -ENOSYS;
}
int vnode::close(void)
{
    return -ENOSYS;
}
int vnode::rdwr(void)
{
    return -ENOSYS;
}
int vnode::select(void)
{
    return -ENOSYS;
}
int vnode::getattr(void)
{
    return -ENOSYS;
}
int vnode::setattr(void)
{
    return -ENOSYS;
}
int vnode::access(void)
{
    return -ENOSYS;
}
int vnode::create(const string &path, shared_ptr<vnode> &created_node)
{
    return -ENOSYS;
}
int vnode::remove(void)
{
    return -ENOSYS;
}
int vnode::link(void)
{
    return -ENOSYS;
}
int vnode::rmdir(void)
{
    return -ENOSYS;
}
int vnode::symlink(void)
{
    return -ENOSYS;
}
int vnode::readlink(void)
{
    return -ENOSYS;
}
int vnode::fsync(void)
{
    return -ENOSYS;
}
int vnode::inactive(void)
{
    return -ENOSYS;
}
int vnode::bmap(void)
{
    return -ENOSYS;
}
int vnode::strategy(void)
{
    return -ENOSYS;
}
int vnode::brelse(void)
{
    return -ENOSYS;
}
int vnode::bread(ssize_t position, size_t size, char *data, int *bytesRead)
{
    return -ENOSYS;
}
int vnode::bwrite(ssize_t position, size_t size, const char *data, int *bytesWritten)
{
    return -ENOSYS;
}
int vnode::readdir(std::vector<std::shared_ptr<vnode>> &vnodes)
{
    return -ENOSYS;
}
int vnode::open(uint64_t flags)
{
    return -ENOSYS;
}

int vnode::mknod(shared_ptr<vnode> &current_node, shared_ptr<vnode> &pNode)
{
    special_nodes.push_back(pNode);
    pNode->addRef(current_node);
    return 0;
}

int vnode::rmnod(shared_ptr<vnode> &current_node, shared_ptr<vnode> &pNode)
{
    auto result = find(special_nodes.begin(), special_nodes.end(), pNode);
    if (result == special_nodes.end()) {
        return ENOENT;
    }
    special_nodes.erase(result);
    pNode->removeRef(current_node);
    return 0;
}

void vnode::setName(const char *name)
{
    if (strchar(name, '/') != nullptr) {
        log_error("Illegal character in filename\nHALT...");
        asm("cli;hlt;");
    }
    v_name = name;
}

int vnode::getSize(size_t &sz)
{
    return -ENOSYS;
}

vnode *open_bdev(string dev_path)
{
    vnode *device = dev_list[dev_path];
    // printf("device %x,flag %x\n", device, device->v_type);
    if ((device != 0) && (device->v_type == VBLK)) {
        return device;
    }
    return NULL;
}