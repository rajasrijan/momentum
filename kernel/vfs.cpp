/*
 * Copyright 2009-2018 Srijan Kumar Sharma
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
#include "vector.h"

#define VFS_C
#include "vfs.h"
#include "vfsops.h"
#include "stdio.h"
#include "stdlib.h"
#include "lists.h"
#include <errno.h>
#include "string.h"
#include <memory>
#include <vector>
#include <map>
#include <utility>
#include <driver/ramdrive.h>
#include <DDI/block_driver.h>
#include <DDI/partition_table.h>
#include <arch/x86_64/arch_hal.h>

using namespace std;

std::vector<pair<vfs *, string>> vfs_list; //	List of all vfs in system.
std::map<string, vnode *> dev_list;		   //	List of all dev in system.
std::vector<vfile *> openFiles;
mtx_t openFileMutex = 0;
mtx_t vfsMtx = 0;
shared_ptr<vnode> rnode;	  //	Pointer to root vnode.
shared_ptr<vnode> root_devfs; //	Pointer to root devfs vnode.
shared_ptr<vnode> root_mnt;   //	Pointer to root mount vnode.

int64_t handle_value = 1;
std::map<int64_t, shared_ptr<vnode>> handles;

class root_vnode : public vnode
{
	vector<shared_ptr<vnode>> children;

  public:
	root_vnode(const char *name, root_vnode *root) : vnode(nullptr)
	{
		if (name)
		{
			setName(name);
		}
		v_root = root;
	}
	~root_vnode()
	{
	}

	int lookup(char const *fileName, shared_ptr<vnode> &pVnode)
	{
		pVnode = nullptr;
		for (const auto vn : children)
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

	int mkdir(std::string name, shared_ptr<vnode> &pDir)
	{
		pDir = make_shared<root_vnode>(name.c_str(), this);
		pDir->v_type = VDIR;
		children.push_back(pDir);
		return 0;
	}

	int mknod(shared_ptr<vnode> pNode)
	{
		children.push_back(pNode);
		pNode->v_root = this;
		return 0;
	}

	int readdir(vector<shared_ptr<vnode>> &vnodes)
	{
		for (auto node : children)
		{
			vnodes.push_back(node);
		}
		return 0;
	}
};

class partition_vnode : public vnode
{
	shared_ptr<vnode> v_parent;
	size_t v_start_blk, v_size_blk;

  public:
	partition_vnode(const char *name, shared_ptr<vnode> parent_vnode, size_t start_blk, size_t size_blk) : vnode(nullptr), v_parent(parent_vnode)
	{
		if (name)
		{
			setName(name);
		}
		v_start_blk = start_blk;
		v_size_blk = size_blk;
	}
	~partition_vnode()
	{
	}
	int bread(ssize_t position, size_t size, char *data)
	{
		if (size >= v_size_blk)
		{
			return EINVAL;
		}
		v_parent->bread(v_start_blk + position, size, data);
	}
	int readdir(vector<shared_ptr<vnode>> &vnodes)
	{
		return ENOTDIR;
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
		rnode = new root_vnode(nullptr, nullptr);
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

int auto_mount_partition(shared_ptr<vnode> blk_dev)
{
	int ret = 0;
	vfs *fs_vfs = nullptr;
	shared_ptr<vnode> fs_root_vnode = nullptr;
	sync lock(vfsMtx);
	for (pair<vfs *, string> &fs : vfs_list)
	{
		printf("Trying to mount as [%s]\n", fs.second.c_str());
		fs_vfs = fs.first;
		fs_root_vnode = nullptr;
		ret = fs_vfs->mount(0, blk_dev, fs_root_vnode);
		if (!ret)
		{
			printf("Successfully mounted\n");
			break;
		}
	}

	//	FS mounted.
	if (!ret && fs_vfs && fs_root_vnode != nullptr)
	{
		statfs_t statfs = {0};
		shared_ptr<vnode> mnt_node = nullptr, mnt_pt = nullptr;
		static unsigned drive_id = 0;
		string mount_point_name = "drive_" + to_string(drive_id++);
		fs_vfs->vfs_vnodecovered = fs_root_vnode;
		fs_vfs->statfs(fs_root_vnode, statfs);
		printf("Partition UUID [%x]\n", statfs.fsid);
		get_vnode_from_abspath("/mnt", mnt_node);
		if (mnt_node == nullptr)
		{
			return ENOFILE;
		}
		mnt_node->mkdir(mount_point_name.c_str(), mnt_pt);
		mnt_pt->v_vfsmountedhere = fs_vfs;
	}
	return ret;
}

int create_blockdevice_subview(shared_ptr<vnode> blk_dev, const char *output_name, size_t start_blk, size_t size_blk, shared_ptr<vnode> &out_blk_dev)
{
	int ret = 0;
	out_blk_dev = make_shared<partition_vnode>(output_name, blk_dev, start_blk, size_blk);
	//	create in same directory
	vnode *parent_directory_vnode = blk_dev->v_root;
	ret = parent_directory_vnode->mknod(out_blk_dev);
	return ret;
}

int create_partition_dev(shared_ptr<vnode> blk_dev)
{
	int ret = 0;
	//  Searching for partitions.
	printf("Searching for partitions.\n");
	PartitionTable pTable;
	int no_of_partitions = 0;
	ret = blk_dev->bread(0, 1, (char *)&pTable);
	printf("Boot signature [%x]\n", (uint32_t)pTable.boot_signature);
	for (int i = 0; i < 4; i++)
	{
		uint32_t status = pTable.partition[i].status;
		if (status == 0x80)
		{
			printf("Partition [%d]\n", i);
			char partition_name[256] = {0};
			no_of_partitions++;
			sprintf(partition_name, "%sp%d", blk_dev->getName().c_str(), i + 1);
			shared_ptr<vnode> partition_node = nullptr;
			create_blockdevice_subview(blk_dev, partition_name, pTable.partition[i].startLBA, pTable.partition[i].count, partition_node);
			if (partition_node == nullptr)
			{
				printf("failed to create partition vnode\n");
				continue;
			}
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
	create_partition_dev(blk_dev);
	return 0;
}

vfs::vfs(void) : vfs_vnodecovered(nullptr)
{
	printf("Initializing VFS\n");
}

vfs::~vfs(void)
{
	printf("\nde-Initialisint vfs");
}

vnode::vnode(class vfs *vfsp) : v_vfsp(vfsp), v_count(0), v_vfsmountedhere(nullptr)
{
}

vnode::~vnode()
{
	printf("vnode deleted\n");
}
const string &vnode::getName() const { return v_name; }
int vnode::open(unsigned int, vfile *&) { return ENOSYS; }
int vnode::ioctl(unsigned int, void *, int) { return ENOSYS; }
int vnode::dolookup(const char *const path, shared_ptr<vnode> &foundNode)
{
	if (v_vfsmountedhere && v_vfsmountedhere->vfs_vnodecovered != nullptr)
		v_vfsmountedhere->vfs_vnodecovered->dolookup(path, foundNode);
	else
		return lookup(path, foundNode);
}
int vnode::doreaddir(vector<shared_ptr<vnode>> &vnodes)
{
	if (v_vfsmountedhere && v_vfsmountedhere->vfs_vnodecovered != nullptr)
		v_vfsmountedhere->vfs_vnodecovered->doreaddir(vnodes);
	else
		return readdir(vnodes);
}
int vnode::lookup(char const *, shared_ptr<vnode> &) { return ENOSYS; }
int vnode::bread(ssize_t, size_t, char *) { return ENOSYS; }
int vnode::mkdir(std::string name, shared_ptr<vnode> &pDir) { return ENOSYS; }
int vnode::mknod(shared_ptr<vnode> pNode) { return ENOSYS; }
int vnode::rename(string name) { v_name = name; }
int vnode::close(void) { return ENOSYS; }
int vnode::rdwr(void) { return ENOSYS; }
int vnode::select(void) { return ENOSYS; }
int vnode::getattr(void) { return ENOSYS; }
int vnode::setattr(void) { return ENOSYS; }
int vnode::access(void) { return ENOSYS; }
int vnode::create(void) { return ENOSYS; }
int vnode::remove(void) { return ENOSYS; }
int vnode::link(void) { return ENOSYS; }
int vnode::rmdir(void) { return ENOSYS; }
int vnode::symlink(void) { return ENOSYS; }
int vnode::readlink(void) { return ENOSYS; }
int vnode::fsync(void) { return ENOSYS; }
int vnode::inactive(void) { return ENOSYS; }
int vnode::bmap(void) { return ENOSYS; }
int vnode::strategy(void) { return ENOSYS; }
int vnode::bwrite(void) { return ENOSYS; }
int vnode::brelse(void) { return ENOSYS; }

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
	//printf("device %x,flag %x\n", device, device->v_type);
	if ((device != 0) && (device->v_type == VBLK))
	{
		return device;
	}
	return NULL;
}

//  vfile implementation

vfile::vfile(vnode *parent) : _parent(parent), posP(0), posG(0)
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
	if (errCode = _parent->bread(position, sz, data))
		return errCode;
	return errCode;
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
}

int mount(string mountPoint, string mountSource)
{
	return 0;
}

uint32_t open(string name)
{
	uint32_t fd = 0;
	shared_ptr<vnode> node = nullptr;
	vfile *file = 0;
	assert(name[0] != '/');
	if (rnode->dolookup(&(name.c_str()[1]), node) != 0)
	{
		return -1;
	}
	for (size_t i = name.find_first_of("/", 1); i != -1; i = name.find_first_of("/", i))
	{
		if (node->dolookup(&(name.c_str()[i]), node) != 0)
		{
			return -1;
		}
	}
	if (node == 0)
		return -1;
	if (node->open(0, file))
		return -1;
	{
		sync fileSyncronisation(openFileMutex);
		openFiles.push_back(file);
		fd = (uint32_t)openFiles.size() - 1;
	}
	return fd;
}

uint32_t read(uint32_t fd, char *dst, size_t size)
{
	vfile *file = 0;
	{
		sync openFileLock(openFileMutex);
		if (fd < 0 || fd >= openFiles.size())
			return EBADF;
		file = openFiles[fd];
	}
	return file->read(dst, size);
}

uint32_t fseek(uint32_t fd, long int offset, int origin)
{
	vfile *file = 0;
	{
		sync openFileLock(openFileMutex);
		if (fd < 0 || fd >= openFiles.size())
			return EBADF;
		file = openFiles[fd];
	}
	return file->seekg(offset, origin);
}

void register_filesystem(vfs *fs, string fsName)
{
	sync lock(vfsMtx);
	vfs_list.push_back(make_pair<vfs *, string>(fs, fsName));
}

void unregister_filesystem(vfs *fs)
{
	sync lock(vfsMtx);
	//vfs_list.remove(fs);
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
	ret = folderNode->mknod(dev);
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

shared_ptr<vnode> current_directory;

shared_ptr<vnode> getCurrentDirectory()
{
	if (current_directory == nullptr)
	{
		current_directory = rnode;
	}
	return current_directory;
}

void setCurrentDirectory(shared_ptr<vnode> dir)
{
	current_directory = dir;
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
	} while (end_offset != string::npos);
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

int chdir(const char *path)
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
	setCurrentDirectory(cdir);
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
	for (const auto node : dir_contents)
	{
		dir.push_back(node->getName());
	}
	return 0;
}