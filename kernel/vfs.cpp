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

#include "vfs.h"
#include <kernel/vnode.h>
#include <DDI/partition_table.h>
#include <arch/x86_64/arch_hal.h>
#include <kernel/multitask.h>
#include <kernel/sys_info.h>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <fcntl.h>
#include <sys/stat.h>
#include <kernel/logging.h>
#include <stdatomic.h>

using namespace std;
vector<pair<fileSystem, string>> fs_list; //	List of all fs in system.
vector<shared_ptr<vfs>> vfs_list;         //	List of all vfs in system.
mtx_t vfsMtx = 0;
shared_ptr<vnode> rnode;      //	Pointer to root vnode.
shared_ptr<vnode> root_devfs; //	Pointer to root devfs vnode.
shared_ptr<vnode> root_mnt;   //	Pointer to root mount vnode.
int64_t handle_value = 1;
map<int64_t, shared_ptr<vnode>> handles;
#ifdef _arch_efi_
vector<shared_ptr<vnode>> path_history;
#endif

uuid_t root_partition_uuid = {};

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
            return -ENOFILE;
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
    int bread(size_t position, size_t size, char *data, size_t *bytesRead)
    {
        return -ENOSYS;
    }

    int bwrite(size_t position, size_t size, const char *data, size_t *bytesWritten)
    {
        return -ENOSYS;
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
    partition_vnode(const char *name, shared_ptr<vnode> parent_vnode, size_t start_blk, size_t size_blk)
        : vnode(nullptr), v_parent(parent_vnode), v_start_blk(0), v_size_blk(0)
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
    int bread(size_t position, size_t size, char *data, size_t *bytesRead)
    {
        int errorcode = 0;
        if (size >= v_size_blk)
        {
            return EINVAL;
        }
        errorcode = v_parent->bread(v_start_blk + position, size, data, bytesRead);
        return errorcode;
    }
    int bwrite(size_t position, size_t size, const char *data, size_t *bytesRead)
    {
        int errorcode = 0;
        if (size >= v_size_blk)
        {
            return EINVAL;
        }
        errorcode = v_parent->bwrite(v_start_blk + position, size, data, bytesRead);
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
    int ioctl(unsigned int cmd, void *data, int flags)
    {
        return v_parent->ioctl(cmd, data, flags);
    }
    bool isPartitionable()
    {
        return false;
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

void init_path_history(shared_ptr<vnode> &rnode)
{
#ifdef _arch_efi_
    path_history.clear();
    path_history.push_back(rnode);
#else
    multitask::getInstance()->getKernelProcess()->path_history.clear();
    multitask::getInstance()->getKernelProcess()->path_history.push_back(rnode);
#endif
}

int init_vfs(void)
{
    printf("Creating vfs.\n");
    if (mount_root())
    {
        log_error("Failed to mount root vfs\n");
    }
    init_path_history(rnode);
    return 0;
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
            if (child->isMountPoint())
            {
                result = dst->dolookup(name.c_str(), dst_child);
                if (result < 0)
                {
                    log_info("Mount point not found. Creating mount point.\n");
                    result = dst->mkdir(name.c_str(), dst_child);
                    if (result < 0)
                    {
                        log_error("Failed to clone root file system. File not found. [%s]\n", name.c_str());
                        return result;
                    }
                }
                dst_child->v_vfsmountedhere = child->v_vfsmountedhere;
            }
            else
            {
                result = dst->dolookup(name.c_str(), dst_child);
                if (result < 0)
                {
                    log_error("Failed to clone root file system. File not found. [%s]\n", name.c_str());
                    return result;
                }
                result = clone_path(dst_child, child);
                if (result < 0)
                {
                    return result;
                }
            }
        }
        else if (child->isBlockDevice() || child->isCharacterDevice())
        {
            dst->mknod(dst, child);
            src->rmnod(src, child);
        }
    }
    return 0;
}

shared_ptr<vnode> getCurrentDirectory()
{
#if KERNEL_ENABLE_MULTITASKING == 1
    return multitask::getInstance()->getCurrentProcess()->path_history.back();
#else
    return path_history.back();
#endif
}

string getCurrentPath()
{
    string completePath = "";
#if KERNEL_ENABLE_MULTITASKING == 1
    auto &path_history = multitask::getInstance()->getCurrentProcess()->path_history;
#endif
    for (size_t i = 0; i < path_history.size(); i++)
    {
        completePath += path_history[i]->getName() + "/";
    }
    return completePath;
}

void setRootDirectory(shared_ptr<vnode> rnode)
{
#if KERNEL_ENABLE_MULTITASKING == 1
    auto &path_history = multitask::getInstance()->getCurrentProcess()->path_history;
#endif
    path_history[0] = rnode;
}

int auto_mount_partition(shared_ptr<vnode> blk_dev)
{
    int ret = 0;
    vfs *fs_vfs = nullptr;
    shared_ptr<vnode> fs_root_vnode = nullptr;
    sync lock(vfsMtx);
    for (const pair<fileSystem, string> &fs : fs_list)
    {
        log_info("Trying to mount as [%s]\n", fs.second.c_str());

        if (!fs.first.new_vfs)
        {
            log_error("Invalid function pointer!\n");
            break;
        }
        fs_vfs = fs.first.new_vfs();

        fs_root_vnode = nullptr;
        ret = fs_vfs->mount(0, blk_dev, fs_root_vnode);

        if (!ret)
        {
            vfs_list.push_back(fs_vfs);
            log_info("Successfully mounted\n");
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
        ret = fs_vfs->statfs(fs_root_vnode, statfs);
        if (ret < 0)
        {
            log_error("Failed to stat fs\n");
            goto error_exit;
        }
        printf("Partition UUID [%s]\n", to_string(statfs.fsid).c_str());
        if (statfs.fsid == root_partition_uuid)
        {
            // root is mounted at '/'
            ret = clone_path(fs_root_vnode, rnode);
            if (ret < 0)
            {
                log_error("Failed to mount root file system.\n");
                asm("cli;hlt");
            }
            rnode = fs_root_vnode;
            setRootDirectory(rnode);
        }
        else
        {
            shared_ptr<vnode> mnt_node = nullptr, mnt_pt = nullptr;
            static unsigned drive_id = 0;
            string mount_point_name = "drive_" + to_string(drive_id++);
            get_vnode_from_abspath("/mnt", mnt_node);
            if (mnt_node == nullptr)
            {
                return -ENOFILE;
            }
            ret = mnt_node->dolookup(mount_point_name.c_str(), mnt_pt);
            if (ret < 0)
            {
                ret = mnt_node->mkdir(mount_point_name.c_str(), mnt_pt);
                if (ret < 0)
                {
                    log_error("failed to create mount moint directory [%s]. error [%d]", mount_point_name.c_str(), ret);
                    goto error_exit;
                }
            }
            mnt_pt->v_vfsmountedhere = fs_vfs;
        }
        fs_root_vnode->v_vfsp = fs_vfs;
    }
error_exit:
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
    if (blk_dev->isPartitionable())
    {
        //  Searching for partitions.
        printf("Searching for partitions.\n");
        PartitionTable pTable;
        int no_of_partitions = 0;
        uint64_t disk_size = 0;
        ret = blk_dev->ioctl(BLKGETSIZE64, &disk_size, 0);
        if (ret)
        {
            log_error("Failed to BLKGETSIZE64. error(%d)\n", ret);
            return ret;
        }
        ret = blk_dev->bread(0, 1, (char *)&pTable, nullptr);
        if (ret)
        {
            log_error("Failed to read from disk. error(%d)\n", ret);
            return ret;
        }
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
                    log_error("failed to create partition vnode\n");
                    continue;
                }
                mknod("/dev/", partition_node);
            }
        }
        if (no_of_partitions != 0)
        {
            return ret;
        }
        printf("No partitions detected. Will treat whole disk as partition\n");
    }
    ret = auto_mount_partition(blk_dev);
    if (ret)
    {
        log_error("No matching file system found\n");
    }
    return ret;
}

int add_blk_dev(shared_ptr<vnode> blk_dev)
{
    return create_partition_dev(blk_dev);
}
vfs::vfs(void) : vfs_vnodecovered(nullptr), flag(0)
{
}
vfs::~vfs(void)
{
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
    size_t position = posG;
    int errCode = 0;
    size_t bytesRead = 0;
    errCode = _parent->bread(position, sz, data, &bytesRead);
    if (errCode)
        return -1;
    posG += bytesRead;
    return bytesRead;
}

int vfile::write(const char *data, size_t sz)
{
    size_t position = posG;
    int errCode = 0;
    size_t bytesWritten = 0;
    errCode = _parent->bwrite(position, sz, data, &bytesWritten);
    if (errCode)
        return -1;
    posG += bytesWritten;
    return bytesWritten;
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

int open(const string &name, int oflag, vfile **file)
{
    int errorCode = 0;
    shared_ptr<vnode> node = nullptr;
    errorCode = lookup(name.c_str(), node);

    //  if file doesn't exit and not creating it, then return error
    if (errorCode < 0 && !(oflag & O_CREAT))
    {
        return -ENOFILE;
    }

    if ((oflag & O_DIRECTORY) && !node->isDirectory())
    {
        return -ENOTDIR;
    }

    // doesn't exist and create flag set, then create
    if (errorCode < 0 && (oflag & O_CREAT))
    {
        errorCode = create(name.c_str(), node);
        if (errorCode < 0)
        {
            log_error("Failed to create file. error [%d]\n", errorCode);
            return errorCode;
        }
    }
    else // try to open it
    {
        errorCode = node->doopen(node, oflag, file);
        if (errorCode || !file)
            return -1;
    }
    return errorCode;
}

int open(const string &name, int oflag)
{
    int ret = 0;
    vfile *file = nullptr;
    ret = open(name, oflag, &file);
    if (ret < 0)
    {
        return ret;
    }
    auto process = multitask::getInstance()->getCurrentProcess();
    ret = process->insert_opened_file(file);
    if (ret < 0)
    {
        return ret;
    }
    return ret;
}

int read(int fd, char *dst, size_t size)
{
    int ret = 0;
    vfile *file = 0;
    auto process = multitask::getInstance()->getCurrentProcess();
    ret = process->get_opened_file(fd, &file);
    if (ret < 0)
    {
        return ret;
    }
    return file->read(dst, size);
}

int write(int fd, const char *src, size_t size)
{
    int ret = 0;
    vfile *file = 0;
    auto process = multitask::getInstance()->getCurrentProcess();
    ret = process->get_opened_file(fd, &file);
    if (ret < 0)
    {
        return ret;
    }
    return file->write(src, size);
}

int fseek(int fd, long int offset, int origin)
{
    int ret = 0;
    vfile *file = 0;
    auto process = multitask::getInstance()->getCurrentProcess();
    ret = process->get_opened_file(fd, &file);
    if (ret < 0)
    {
        return ret;
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
/**
 * @brief Create a node at pathname. Insert 'dev' vnode at location. If 'dev' is of type 'VBLK' then add_blk_dev() is
 * called.
 *
 * @param pathname Path to vnode location
 * @param dev vnode to be inserted
 * @return int returns 0 on success. negative error code.
 */
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

int openat(int dirfd, const char *pathname, int flags, mode_t mode)
{
    int64_t descriptor = -1;
    if (dirfd == FDCWD)
    {
        shared_ptr<vnode> cdir;
        if (lookup(pathname, cdir))
        {
            log_error("no such directory!");
            return -ENOTDIR;
        }
        //	handles
        descriptor = atomic_fetch_add(&handle_value, (int64_t)1);
        handles[descriptor] = cdir;
    }
    return descriptor;
}

int lookup(const char *path, shared_ptr<vnode> &node)
{
    if (!path)
        return -EINVAL;
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
        if (path_component == ".")
            continue;
        else if (cdir->dolookup(path_component.c_str(), cdir) != 0)
        {
            log_error("Invalid path\n");
            return -ENOFILE;
        }
    }
    node = cdir;
    return 0;
}

int create(const string &path, shared_ptr<vnode> &node)
{
    int ret = 0;
    if (path.empty())
        return -EINVAL;
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
    const string file_name = path_components.back();
    path_components.pop_back();
    for (auto &path_component : path_components)
    {
        if (path_component == ".")
            continue;
        else if (cdir->dolookup(path_component.c_str(), cdir) != 0)
        {
            log_error("Invalid path\n");
            return -ENOFILE;
        }
    }
    if (!cdir->isDirectory())
    {
        log_error("Invalid path\n");
        return -ENOFILE;
    }
    ret = cdir->docreate(file_name, node);
    if (ret < 0)
    {
        return ret;
    }

    return ret;
}

int chdir(const char *path)
{
    int errCode = 0;
    shared_ptr<vnode> cdir;
#if KERNEL_ENABLE_MULTITASKING == 1
    auto &path_history = multitask::getInstance()->getCurrentProcess()->path_history;
#endif
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
                log_error("Invalid path\n");
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

int stat(const char *path, struct stat *buf)
{
    int ret = 0;
    shared_ptr<vnode> node;
    ret = lookup(path, node);
    if (ret)
        return ret;
    ret = stat(node, buf);
    return ret;
}
int stat(shared_ptr<vnode> &file_node, struct stat *buf)
{
    int ret = 0;
    //  not implemented
    buf->st_dev = 0;
    //  user id not implemented
    buf->st_uid = 0;
    //  group id not implemented
    buf->st_gid = 0;
    //  not implemented
    buf->st_rdev = 0;
    file_node->getSize(buf->st_size);
    //  vnode type
    switch (file_node->v_type)
    {

    case VREG:
        buf->st_mode = S_IFREG;
        break;
    case VDIR:
        buf->st_mode = S_IFDIR;
        break;
    case VBLK:
        buf->st_mode = S_IFBLK;
        break;
    case VCHR:
        buf->st_mode = S_IFCHR;
        break;
    case VLNK:
        buf->st_mode = S_IFLNK;
        break;
    case VSOCK:
        buf->st_mode = S_IFSOCK;
        break;
    default:
        buf->st_mode = 0;
        break;
    };

    // not implemented
    buf->st_nlink = 0;
    buf->st_ino = 0;
    buf->st_atime = 0; // last access time
    buf->st_mtime = 0; // last modification time
    buf->st_ctime = 0; // last sttus change type
    buf->st_blocks = 4096;
    return ret;
}

int vfile::readdir(struct dirent *buf, size_t *buf_size)
{
    int ret = 0;
    vector<shared_ptr<vnode>> vnode_list;
    ret = _parent->doreaddir(vnode_list);

    return ret;
}

int readdir(int fd, struct dirent *buf, size_t *buf_size)
{
    int ret = 0;
    vfile *file = nullptr;
    auto process = multitask::getInstance()->getCurrentProcess();
    ret = process->get_opened_file(fd, &file);
    ret = readdir(file, buf, buf_size);
    return ret;
}
int readdir(vfile *file, struct dirent *buf, size_t *buf_size)
{
    int ret = 0;
    if (!file->_parent->isDirectory())
    {
        ret = -ENOTDIR;
        goto error_exit;
    }
    ret = file->readdir(buf, buf_size);
error_exit:
    return ret;
}

int find(const char *path, const char *pattern, int depth, vector<shared_ptr<class vnode>> &nodes)
{
    int ret = 0;
    shared_ptr<vnode> root_mnt;
    vector<shared_ptr<class vnode>> stack, stack_nxt_lvl;
    nodes.clear();
    ret = lookup(path, root_mnt);
    if (ret)
    {
        log_error("Failed to get vnode for [%s]! error [%d]\n", path, ret);
    }
    stack.push_back(root_mnt);
    while ((stack.size() > 0) && (depth > 0))
    {
        shared_ptr<vnode> cur_mnt = stack.back();
        stack.pop_back();

        vector<shared_ptr<class vnode>> cur_file_list;
        cur_mnt->doreaddir(cur_file_list);
        for (auto node : cur_file_list)
        {
            if (node->isDirectory())
            {
                stack_nxt_lvl.push_back(node);
            }
            if (node->getName() == pattern)
            {
                nodes.push_back(node);
            }
        }
        if (stack.empty())
        {
            stack.insert(stack.end(), stack_nxt_lvl.begin(), stack_nxt_lvl.end());
            stack_nxt_lvl.clear();
            depth--;
        }
    }
    return ret;
}
