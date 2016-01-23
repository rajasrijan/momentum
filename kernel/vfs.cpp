#include "vector.h"

#define VFS_C
#include "vfs.h"
#include "vfsops.h"
#include "stdio.h"
#include "stdlib.h"
#include "lists.h"
#include <errno.h>
#include <memory>
#include <vector>
#include <map>
#include <DDI/filesystem.h>

using namespace std;

std::vector<vfs> vfs_list; //	List of all vfs in system.
std::map<string, vnode*> dev_list; //	List of all dev in system.
vnode *rnode; //	Pointer to root vnode.

void init_vfs(void)
{
    printf("\nCreating vfs.");
    //vfs vfs_first;
    //	root is a dir

    //vfs_list.push_back(vfs_first);
    //rnode = vfs_first.vfs_vnodecovered;
    //rnode->mkdir("dev", 0);
}

int mount_root(vnode* vn)
{
    if (vn == 0)
    {
        return EFAULT;
    }
    rnode = vn;
    return 0;
}

void add_vfs(vfs _vfs)
{
    vfs_list.push_back(_vfs);
}

void add_dev(vnode *devNode)
{
    static unsigned int dev_no = 0;
    string full_dev_path = "/dev/hda";
    full_dev_path[7] = (char) (full_dev_path[7] +(char) dev_no++);
    printf("\nAdding dev [%s],devNode [%x], v_type [%x]", full_dev_path.c_str(), devNode, devNode->v_type);
    dev_list[full_dev_path] = devNode;
}

vfs::vfs(void)
{
    printf("\nInitialising vfs");
    //vfs_vnodecovered = new vnode;
    //vfs_vnodecovered->v_type = VDIR;
}

vfs::~vfs(void)
{
    printf("\nde-Initialisint vfs");
}

int vfs::mount(void)
{
    return 0;
}

int vfs::unmount(void)
{
    return 0;
}

int vfs::root(void)
{
    return 0;
}

int vfs::statfs(void)
{
    return 0;
}

int vfs::sync(void)
{
    return 0;
}

int vfs::fid(void)
{
    return 0;
}

int vfs::vget(void)
{
    return 0;
}

vnode::vnode(class vfs* vfsp) : v_count(0), v_vfsp(vfsp)
{
    //assert(!v_vfsp)
    vector<vnode> *directory_list = new vector<vnode>;
    v_data = directory_list;
}

vnode::~vnode()
{

}

int vnode::open()
{

}

int vnode::lookup(const char* const path)
{

}

int vnode::mkdir(std::string name, vnode *pDir)
{
    //    vector<vnode>* directory_list = (vector<vnode>*)v_data.get();
    //    vnode newDir;
    //    newDir.v_type = VDIR;
    //    newDir.rename(name);
    //    directory_list->push_back(newDir);
    return 0;
}

int vnode::bread(int position, int size, shared_ptr<char> data)
{
    printf("This is wrong\n");
    return 1;
}

int vnode::rename(string name)
{
    v_name = name;
}

vnode* open_bdev(string dev_path)
{
    vnode* device = dev_list[dev_path];
    //printf("device %x,flag %x\n", device, device->v_type);
    if ((device != 0) && (device->v_type == VBLK))
    {
        //DBG_OUTPUT
        return device;
    }
    //DBG_OUTPUT
    return NULL;
}


//  vfile implementation

vfile::vfile(vnode* parent) : _parent(parent)
{
    //_parent->v_count++;
}

vfile::~vfile()
{
    _parent = 0;
}

int vfile::read(size_t position, shared_ptr<char*> data)
{
    //_parent->bread();
}