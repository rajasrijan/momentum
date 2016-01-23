/*
 * Copyright 2009-2012 Srijan Kumar Sharma
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

#ifndef VFS_H
#define VFS_H
#define DBG_OUTPUT printf("\nLine %d, function %s(), file %s",__LINE__,__FUNCTION__,__FILE__);
#include <stdint.h>
#include <vector>
#include <string>
#include <memory>

using namespace std;

enum vtype
{
    VNON = 0,
    VREG = 1,
    VDIR = 2,
    VBLK = 3,
    VCHR = 4,
    VLNK = 5,
    VSOCK = 6,
    VBAD = 7
};

class vnode
{
public:
    uint16_t v_flag; /* vnode flags */
    uint16_t v_count; /* reference count */
    uint16_t v_shlockc; /* # of shared locks */
    uint16_t v_exlockc; /* # of exclusive locks */
    uint32_t v_type; /* vnode type */
    class vfs *v_vfsmountedhere; /* covering vfs */
    //struct vnodeops *v_op; /* vnode operations */
    class vfs *v_vfsp; /* vfs we are in */

    //protected:
    string v_name;
private:
    std::shared_ptr<void> v_data; /* private data */
public:
    vnode(class vfs* vfsp);
    virtual ~vnode();
    int open(void);
    int close(void);
    int rdwr(void);
    int ioctl(void);
    int select(void);
    int getattr(void);
    int setattr(void);
    int access(void);
    virtual int lookup(const char* const path);
    int create(void);
    int remove(void);
    int link(void);
    int rename(string name);
    int mkdir(std::string name, vnode *pDir);
    int rmdir(void);
    int readdir(void);
    int symlink(void);
    int readlink(void);
    int fsync(void);
    int inactive(void);
    int bmap(void);
    int strategy(void);
    virtual int bread(int position, int size, shared_ptr<char> data);
    int bwrite(void);
    int brelse(void);
};

class vfile
{
    vnode* _parent;
public:
    vfile(vnode* parent);
    ~vfile();
    int read(size_t position, shared_ptr<char*> data);
};

class vfs
{
public:
    //class vfs *vfs_next; /* next vfs in list */
    //struct vfsops *vfs_op; /* operations on vfs */
    vnode *vfs_vnodecovered; /* vnode we cover */
    int flag; /* flags */
    int bsize; /* native block size */
    //void* vfs_data; /* private data */
public:
    vfs();
    ~vfs();
    int mount(void);
    int unmount(void);
    int root(void);
    int statfs(void);
    int sync(void);
    int fid(void);
    int vget(void);
};

void init_vfs(void);

extern std::vector<vfs> vfs_list;
extern vnode *rnode; //	Pointer to root vnode.
int mount_root(vnode* vn);
void add_vfs(vfs _vfs);
void add_dev(vnode *devNode);
vnode* open_bdev(string dev_path);

#endif
