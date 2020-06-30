/*
 * Copyright 2009-2020 Srijan Kumar Sharma
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
#define VFS_C
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <DDI/partition_table.h>
#include <kernel/multitask.h>
#include <kernel/sys_info.h>
#include <arch/x86_64/arch_hal.h>
#include "vfs.h"

using namespace std;
vector<pair<fileSystem, string>> fs_list; //	List of all fs in system.
vector<shared_ptr<vfs>> vfs_list;         //	List of all vfs in system.
map<string, vnode *> dev_list;            //	List of all dev in system.
vector<vfile *> openFiles;
mtx_t openFileMutex = 0;
mtx_t vfsMtx = 0;
shared_ptr<vnode> rnode;      //	Pointer to root vnode.
shared_ptr<vnode> root_devfs; //	Pointer to root devfs vnode.
shared_ptr<vnode> root_mnt;   //	Pointer to root mount vnode.
int64_t handle_value = 1;
map<int64_t, shared_ptr<vnode>> handles;
class root_vnode : public vnode
{
    vector<shared_ptr<vnode>> children;

  public:
    root_vnode(const char *name /*, root_vnode *root*/) : vnode(nullptr), children()
    {
        if (name)
        {
            setName(name);
        }
        // v_root = root;
    }
    ~root_vnode()
    {
    }
    int lookup(char const *fileName, shared_ptr<vnode> &pVnode)
    {
        pVnode = nullptr;
        for (const auto &vn : children)
        {
            if (vn->getName().compare(fileName) == 0)
            {
                pVnode = vn;
                break;
            }
        }
        if (pVnode == nullptr)
        {
            return ENOFILE;
        }
        return 0;
    }
    int mkdir(string name, shared_ptr<vnode> &pDir)
    {
        pDir = make_shared<root_vnode>(name.c_str());
        pDir->v_type = VDIR;
        children.push_back(pDir);
        return 0;
    }
    int bread(ssize_t position, size_t size, char *data, int *bytesRead)
    {
        return ENOSYS;
    }
    int readdir(vector<shared_ptr<vnode>> &vnodes)
    {
        for (auto node : children)
        {
            vnodes.push_back(node);
        }
        return 0;
    }
    int open(uint64_t flags)
    {
        return -ENOSYS;
    }
};
class partition_vnode : public vnode
{
    shared_ptr<vnode> v_parent;
    size_t v_start_blk, v_size_blk;

  public:
    partition_vnode(const char *name, shared_ptr<vnode> parent_vnode, size_t start_blk, size_t size_blk) : vnode(nullptr), v_parent(parent_vnode), v_start_blk(0), v_size_blk(0)
    {
        if (name)
        {
            setName(name);
        }
        v_start_blk = start_blk;
        v_size_blk = size_blk;
        v_type = VBLK;
    }
    ~partition_vnode()
    {
    }
    int bread(ssize_t position, size_t size, char *data, int *bytesRead)
    {
        int errorcode = 0;
        if (size >= v_size_blk)
        {
            return EINVAL;
        }
        errorcode = v_parent->bread(v_start_blk + position, size, data, bytesRead);
        return errorcode;
    }
    int readdir(vector<shared_ptr<vnode>> &vnodes)
    {
        return ENOTDIR;
    }
    int open(uint64_t flags)
    {
        return -ENOSYS;
    }
};
int mount_root(void);
int auto_mount_partition(vnode *blk_dev);
int create_partition_dev(vnode *blk_dev);
int get_vnode_from_abspath(const char *pathname, shared_ptr<vnode> &pathNode);
int mount_root(void)
{
    if (rnode == nullptr)
    {
        rnode = new root_vnode(nullptr);
        rnode->v_type = VDIR;
    }
    if (root_devfs == nullptr)
    {
        rnode->mkdir("dev", root_devfs);
        rnode->mkdir("mnt", root_mnt);
    }
    return 0;
}

void init_vfs(void)
{
    printf("Creating vfs.\n");
    if (mount_root())
    {
        printf("Failed to mount root vfs\n");
    }
    multitask::getInstance()->getKernelProcess()->path_history.push_back(rnode);
}
int mount_root(vnode *vn)
{
    if (vn == 0)
    {
        return EFAULT;
    }
    rnode = vn;
    return 0;
}

int clone_path(shared_ptr<vnode> &dst, shared_ptr<vnode> &src)
{
    int result = 0;
    vector<shared_ptr<vnode>> children;
    src->doreaddir(children);
    for (auto &child : children)
    {
        string name = child->getName();
        if (child->isDirectory())
        {
            shared_ptr<vnode> dst_child;
            result = dst->dolookup(name.c_str(), dst_child);
            if (result)
            {
                return result;
            }
            result = clone_path(dst_child, child);
            if (result)
            {
                return result;
            }
        }
        else if (child->isBlockDevice())
        {
            dst->mknod(dst, child);
            src->rmnod(src, child);
        }
    }
    return 0;
}

int auto_mount_partition(shared_ptr<vnode> blk_dev)
{
    int ret = 0;
    vfs *fs_vfs = nullptr;
    shared_ptr<vnode> fs_root_vnode = nullptr;
    sync lock(vfsMtx);
    for (const pair<fileSystem, string> &fs : fs_list)
    {
        printf("Trying to mount as [%s]\n", fs.second.c_str());
        fs_vfs = fs.first.new_vfs();
        fs_root_vnode = nullptr;
        ret = fs_vfs->mount(0, blk_dev, fs_root_vnode);
        if (!ret)
        {
            vfs_list.push_back(fs_vfs);
            printf("Successfully mounted\n");
            break;
        }
        fs.first.delete_vfs(fs_vfs);
        fs_vfs = nullptr;
    }
    //	FS mounted.
    if (!ret && fs_vfs && fs_root_vnode != nullptr)
    {
        statfs_t statfs = {};
        fs_vfs->vfs_vnodecovered = fs_root_vnode;
        fs_vfs->statfs(fs_root_vnode, statfs);
        printf("Partition UUID [%x]\n", statfs.fsid);
        if (statfs.fsid == sys_info.root_drive_uuid)
        {
            // root is mounted at '/'
            clone_path(fs_root_vnode, rnode);
            rnode = fs_root_vnode;
            multitask::getInstance()->getKernelProcess()->path_history[0] = rnode;
        }
        else
        {
            shared_ptr<vnode> mnt_node = nullptr, mnt_pt = nullptr;
            static unsigned drive_id = 0;
            string mount_point_name = "drive_" + to_string(drive_id++);
            get_vnode_from_abspath("/mnt", mnt_node);
            if (mnt_node == nullptr)
            {
                return ENOFILE;
            }
            mnt_node->mkdir(mount_point_name.c_str(), mnt_pt);
            mnt_pt->v_vfsmountedhere = fs_vfs;
        }
        fs_root_vnode->v_vfsp = fs_vfs;
    }
    return ret;
}
int create_blockdevice_subview(shared_ptr<vnode> blk_dev, const char *output_name, size_t start_blk, size_t size_blk, shared_ptr<vnode> &out_blk_dev)
{
    int ret = 0;
    out_blk_dev = make_shared<partition_vnode>(output_name, blk_dev, start_blk, size_blk);
    return ret;
}
int create_partition_dev(shared_ptr<vnode> blk_dev)
{
    int ret = 0;
    //  Searching for partitions.
    printf("Searching for partitions.\n");
    PartitionTable pTable;
    int no_of_partitions = 0;
    uint64_t disk_size = 0;
    ret = blk_dev->ioctl(BLKGETSIZE64, &disk_size, 0);
    if (ret)
    {
        printf("Failed to BLKGETSIZE64. error(%d)\n", ret);
        return ret;
    }
    ret = blk_dev->bread(0, 1, (char *)&pTable, nullptr);
    printf("Boot signature [%x]\n", (uint32_t)pTable.boot_signature);
    for (int i = 0; i < 4; i++)
    {
        const auto &p = pTable.partition[i];
        if (p.count > 0 && p.startLBA > 1 && p.startLBA + p.count <= disk_size)
        {
            printf("Partition [%d]\n", i);
            char partition_name[256] = {0};
            no_of_partitions++;
            snprintf(partition_name, 256, "%sp%d", blk_dev->getName().c_str(), i + 1);
            shared_ptr<vnode> partition_node = nullptr;
            create_blockdevice_subview(blk_dev, partition_name, pTable.partition[i].startLBA, pTable.partition[i].count, partition_node);
            if (partition_node == nullptr)
            {
                printf("failed to create partition vnode\n");
                continue;
            }
            mknod("/dev/", partition_node);
            if (auto_mount_partition(partition_node))
            {
                printf("No matching file system found\n");
                continue;
            }
        }
    }
    if (no_of_partitions != 0)
    {
        return ret;
    }
    printf("No partitions detected. Will treat whole disk as partition\n");
    if (auto_mount_partition(blk_dev))
    {
        printf("No matching file system found\n");
    }
    return ret;
}

int add_blk_dev(shared_ptr<vnode> blk_dev)
{
    return create_partition_dev(blk_dev);
}
vfs::vfs(void) : vfs_vnodecovered(nullptr), flag(0)
{
    printf("Initializing VFS\n");
}
vfs::~vfs(void)
{
    printf("de-Initialisint vfs\n");
}
vnode::vnode(class vfs *vfsp) : v_shlockc(0), v_exlockc(0), v_type(0), v_vfsmountedhere(nullptr), v_vfsp(vfsp), v_flag(0), v_name(), v_count(0), special_nodes(), dnode_cache(), ref_nodes()
{
}
vnode::~vnode()
{
    printf("vnode deleted\n");
}
const string &vnode::getName() const
{
    return v_name;
}
int vnode::ioctl(unsigned int, void *, int)
{
    return -ENOSYS;
}
bool vnode::namecmp(const string &name) const
{
    return !strcmp(name.c_str(), getName().c_str());
}
int vnode::dolookup(const char *const path, shared_ptr<vnode> &foundNode)
{
    char sub_path[256] = {0};
    // copy till '/' or eos.
    for (size_t i = 0; path[i] != 0 && path[i] != '/'; i++)
    {
        sub_path[i] = path[i];
    }
    auto found_itr = find_if(dnode_cache.begin(), dnode_cache.end(), [sub_path](const shared_ptr<vnode> &node) { return node->namecmp(sub_path); });
    if (found_itr != dnode_cache.end())
    {
        foundNode = *found_itr;
        return 0;
    }
    int ret = 0;
    if (v_vfsmountedhere && v_vfsmountedhere->vfs_vnodecovered != nullptr)
    {
        ret = v_vfsmountedhere->vfs_vnodecovered->dolookup(path, foundNode);
    }
    else
    {
        ret = lookup(path, foundNode);
    }
    if (!ret)
    {
        dnode_cache.push_back(foundNode);
    }
    return ret;
}

int vnode::doreaddir(vector<shared_ptr<vnode>> &vnodes)
{
    int ret = 0;
    if (runOnce && !dnode_cache.empty())
    {
        vnodes = dnode_cache;
        goto exit;
    }
    if (v_vfsmountedhere && v_vfsmountedhere->vfs_vnodecovered != nullptr)
        ret = v_vfsmountedhere->vfs_vnodecovered->doreaddir(vnodes);
    else
        ret = readdir(vnodes);
    if (ret)
    {
        return ret;
    }
    //	add to cache
    for (auto &var : vnodes)
    {
        if (dnode_cache.end() == find_if(dnode_cache.begin(), dnode_cache.end(), [&var](const shared_ptr<vnode> &node) { return node->namecmp(var->getName()); }))
        {
            dnode_cache.push_back(var);
        }
    }
    runOnce = true;
exit:
    for (auto &var : special_nodes)
    {
        vnodes.push_back(var);
    }
    return ret;
}
int vnode::doopen(shared_ptr<vnode> &node, uint32_t flags, vfile **file)
{
    if (node != this || !file)
        return -EINVAL;
    if (v_type != VREG)
    {
        return ENOFILE;
    }
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
int vnode::create(void)
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
int vnode::bwrite(void)
{
    return -ENOSYS;
}
int vnode::brelse(void)
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
    if (result == special_nodes.end())
    {
        return ENOENT;
    }
    special_nodes.erase(result);
    pNode->removeRef(current_node);
    return 0;
}

void vnode::setName(const char *name)
{
    if (strchar(name, '/') != nullptr)
    {
        printf("Illegal character in filename\nHALT...");
        asm("cli;hlt;");
    }
    v_name = name;
}

vnode *open_bdev(string dev_path)
{
    vnode *device = dev_list[dev_path];
    // printf("device %x,flag %x\n", device, device->v_type);
    if ((device != 0) && (device->v_type == VBLK))
    {
        return device;
    }
    return NULL;
}

//  vfile implementation
vfile::vfile(shared_ptr<vnode> parent) : _parent(parent), posP(0), posG(0), fileIOLock()
{
    //_parent->v_count++;
}

vfile::~vfile()
{
    _parent = 0;
}

int vfile::read(char *data, size_t sz)
{
    ssize_t position = posG;
    int errCode = 0;
    int bytesRead = 0;
    errCode = _parent->bread(position, sz, data, &bytesRead);
    if (errCode)
        return -1;
    posG += bytesRead;
    return bytesRead;
}

int vfile::seekg(long int offset, int origin)
{
    switch (origin)
    {
    case SEEK_SET:
        posG = offset;
        break;
    case SEEK_CUR:
        posG += offset;
        break;
    case SEEK_END:
        posG = -1;
    default:
        break;
    }
    return 0;
}

int mount(string mountPoint, string mountSource)
{
    return 0;
}

int open(const string &name)
{
    int errorCode = 0;
    int fd = 0;
    shared_ptr<vnode> node = nullptr;
    vfile *file = nullptr;
    errorCode = lookup(name.c_str(), node);
    if (errorCode || node == 0)
        return -1;
    errorCode = node->doopen(node, 0, &file);
    if (errorCode || !file)
        return -1;
    {
        sync fileSyncronisation(openFileMutex);
        openFiles.push_back(file);
        fd = (uint32_t)openFiles.size() - 1;
    }
    return fd;
}

int read(int fd, char *dst, size_t size)
{
    vfile *file = 0;
    {
        sync openFileLock(openFileMutex);
        if (fd < 0 || (size_t)fd >= openFiles.size())
            return EBADF;
        file = openFiles[fd];
    }
    return file->read(dst, size);
}

int fseek(int fd, long int offset, int origin)
{
    vfile *file = 0;
    {
        sync openFileLock(openFileMutex);
        if (fd < 0 || (size_t)fd >= openFiles.size())
            return EBADF;
        file = openFiles[fd];
    }
    return file->seekg(offset, origin);
}

void register_filesystem(fileSystem fs, string fsName)
{
    sync lock(vfsMtx);
    fs_list.push_back(make_pair<fileSystem, string>(fs, fsName));
}

void unregister_filesystem(fileSystem fs)
{
    sync lock(vfsMtx);
    // vfs_list.remove(fs);
}

int get_vnode_from_abspath(const char *pathname, shared_ptr<vnode> &pathNode)
{
    char path[256] = {0};
    const char *pathIt = pathname;
    if (!pathname)
    {
        return EINVAL;
    }
    if (pathname[0] != '/')
    {
        return ENOFILE;
    }
    pathIt++;
    shared_ptr<vnode> current_vnode = rnode;
    while (pathIt && pathIt[0] && current_vnode != nullptr)
    {
        const char *pathItEnd = strchar(pathIt, '/');
        if (pathItEnd)
        {
            copy(pathIt, pathItEnd, path);
        }
        else
        {
            strcpy(path, pathIt);
        }
        pathIt = pathItEnd;
        shared_ptr<vnode> subnode = nullptr;
        current_vnode->dolookup(path, subnode);
        current_vnode = subnode;
    }
    if (current_vnode == nullptr)
    {
        return ENOFILE;
    }
    pathNode = current_vnode;
    return 0;
}
int mknod(const char *pathname, shared_ptr<vnode> dev)
{
    int ret = 0;
    char fileName[256] = {0};
    char folderName[256] = {0};
    shared_ptr<vnode> folderNode;
    if (const char *fn = strrchar(pathname, '/'))
    {
        strcpy(fileName, fn + 1);
        copy(pathname, fn, folderName);
    }
    else
    {
        return ENOFILE;
    }
    ret = get_vnode_from_abspath(folderName, folderNode);
    if (ret || folderNode == nullptr)
    {
        return ENOFILE;
    }
    ret = folderNode->mknod(folderNode, dev);
    if (ret)
    {
        return ret;
    }
    if (dev->v_type == VBLK)
    {
        add_blk_dev(dev);
    }
    return 0;
}

shared_ptr<vnode> getCurrentDirectory()
{
    return multitask::getInstance()->getCurrentProcess()->path_history.back();
}
string getCurrentPath()
{
    string completePath = "";
    auto &path_history = multitask::getInstance()->getCurrentProcess()->path_history;
    for (size_t i = 0; i < path_history.size(); i++)
    {
        completePath += path_history[i]->getName() + "/";
    }
    return completePath;
}

vector<string> split_path(const string &path)
{
    vector<string> ret;
    if (path.empty())
        return ret;
    size_t start_offset = 0;
    size_t end_offset = 0;
    do
    {
        end_offset = path.find("/", start_offset);
        size_t length = (end_offset == string::npos) ? path.size() : end_offset - start_offset;
        // ignore empty
        if (length == 0)
        {
            start_offset = end_offset + 1;
            continue;
        }
        ret.push_back(path.substr(start_offset, length));
        start_offset = end_offset + 1;
    } while (end_offset != string::npos && start_offset < path.size());
    return ret;
}
int openat(int dirfd, const string &pathname, int flags, mode_t mode)
{
    int descriptor = -1;
    if (dirfd == FDCWD)
    {
        shared_ptr<vnode> cdir;
        if (pathname[0] == '/')
        {
            cdir = rnode;
        }
        else
        {
            cdir = getCurrentDirectory();
        }
        auto path_components = split_path(pathname);
        for (auto &path_component : path_components)
        {
            if (cdir->dolookup(path_component.c_str(), cdir) != 0)
            {
                printf("Invalid path\n");
                return -1;
            }
        }
        //	handles
        descriptor = atomic_increment(&handle_value);
        handles[descriptor] = cdir;
    }
    return descriptor;
}
int lookup(const char *path, shared_ptr<vnode> &node)
{
    shared_ptr<vnode> cdir;
    if (path[0] == '/')
    {
        cdir = rnode;
    }
    else
    {
        cdir = getCurrentDirectory();
    }
    auto path_components = split_path(path);
    for (auto &path_component : path_components)
    {
        if (cdir->dolookup(path_component.c_str(), cdir) != 0)
        {
            printf("Invalid path\n");
            return -1;
        }
    }
    node = cdir;
    return 0;
}

int chdir(const char *path)
{
    int errCode = 0;
    shared_ptr<vnode> cdir;
    auto &path_history = multitask::getInstance()->getCurrentProcess()->path_history;
    vector<shared_ptr<vnode>> backup_path = path_history;
    if (path[0] == '/')
    {
        cdir = rnode;
        path_history.clear();
        path_history.push_back(rnode);
    }
    else
    {
        cdir = path_history.back();
    }
    auto path_components = split_path(path);
    for (auto &path_component : path_components)
    {
        if (path_component == ".")
        {
            continue;
        }
        else if (path_component == "..")
        {
            path_history.pop_back();
            cdir = path_history.back();
        }
        else
        {
            if (cdir->dolookup(path_component.c_str(), cdir) != 0)
            {
                printf("Invalid path\n");
                return -1;
            }
            path_history.push_back(cdir);
        }
    }
    if (errCode)
        path_history = backup_path;
    return 0;
}

void close(int fd)
{
    handles.erase(fd);
}
int getdents(int fd, vector<string> &dir)
{
    auto find_result = handles.find(fd);
    if (find_result == handles.end())
        return EBADF;
    shared_ptr<vnode> dir_node = find_result->second;
    if (dir_node->v_type != VDIR)
        return ENOTDIR;
    vector<shared_ptr<vnode>> dir_contents;
    dir_node->doreaddir(dir_contents);
    for (const auto &node : dir_contents)
    {
        dir.push_back(node->getName());
    }
    return 0;
}