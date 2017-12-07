/*
 * Copyright 2009-2017 Srijan Kumar Sharma
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
#include <ddi/partition_table.h>

using namespace std;

std::vector<pair<vfs *, string>> vfs_list; //	List of all vfs in system.
std::map<string, vnode *> dev_list;		   //	List of all dev in system.
std::vector<vfile *> openFiles;
mtx_t openFileMutex = 0;
mtx_t vfsMtx = 0;
vnode *rnode = nullptr;		 //	Pointer to root vnode.
vnode *root_devfs = nullptr; //	Pointer to root devfs vnode.

class root_vnode : public vnode
{
	vector<vnode *> children;

  public:
	root_vnode(const char *name, root_vnode *root)
	{
		if (name)
		{
			v_name = name;
		}
		v_root = root;
	}
	~root_vnode()
	{
	}

	int lookup(char const *fileName, vnode *&pVnode)
	{
		pVnode = nullptr;
		for (vnode *vn : children)
		{
			if (vn->v_name.compare(fileName))
			{
				pVnode = vn;
				break;
			}
		}
		if (!pVnode)
		{
			return ENOFILE;
		}
		return 0;
	}

	int mkdir(std::string name, vnode *&pDir)
	{
		children.push_back(new root_vnode(name.c_str(), this));
		pDir = children.back();
		pDir->v_type = VDIR;
		return 0;
	}

	int mknod(vnode *pNode)
	{
		children.push_back(pNode);
		pNode->v_root = this;
		return 0;
	}
};

class partition_vnode : public vnode
{
	vnode *v_parent;
	size_t v_start_blk, v_size_blk;

  public:
	partition_vnode(const char *name, vnode *parent_vnode, size_t start_blk, size_t size_blk) : v_parent(parent_vnode)
	{
		if (name)
		{
			v_name = name;
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
};

int mount_root(void);
int auto_mount_partition(vnode *blk_dev);
int create_partition_dev(vnode *blk_dev);
int get_vnode_from_abspath(const char *pathname, vnode **pathNode);

int mount_root(void)
{
	if (!rnode)
	{
		rnode = new root_vnode(nullptr, nullptr);
		rnode->v_type = VDIR;
	}
	if (!root_devfs)
	{
		rnode->mkdir("dev", root_devfs);
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

int auto_mount_partition(vnode *blk_dev)
{
	int ret = 0;
	vfs *fs_vfs = nullptr;
	vnode *fs_root_vnode = nullptr;
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
	if (!ret && fs_vfs && fs_root_vnode)
	{
		statfs_t statfs = {0};
		fs_vfs->statfs(fs_root_vnode, statfs);
		printf("Partition UUID [%x]\n", statfs.fsid);
	}
	return ret;
}

int create_blockdevice_subview(vnode *blk_dev, const char *output_name, size_t start_blk, size_t size_blk, vnode **out_blk_dev)
{
	int ret = 0;
	if (!out_blk_dev)
	{
		return EINVAL;
	}
	*out_blk_dev = new partition_vnode(output_name, blk_dev, start_blk, size_blk);
	//	create in same directory
	vnode *parent_directory_vnode = blk_dev->v_root;
	ret = parent_directory_vnode->mknod(*out_blk_dev);
	return ret;
}

int create_partition_dev(vnode *blk_dev)
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
		printf("Partition [%d], status [%x]...%s\n", i, status, (status == 0x80) ? "valid" : "invalid");
		if (status == 0x80)
		{
			char partition_name[256] = {0};
			no_of_partitions++;
			sprintf(partition_name, "%sp%d", blk_dev->v_name.c_str(), i + 1);
			vnode *partition_node = nullptr;
			create_blockdevice_subview(blk_dev, partition_name, pTable.partition[i].startLBA, pTable.partition[i].count, &partition_node);
			if(!partition_node)
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

int add_blk_dev(vnode *blk_dev)
{
	// const char *device_name = blk_dev->v_name.c_str();
	// char full_dev_path[256] = "/dev/";
	// strcat(full_dev_path, device_name);
	// printf("Adding dev [%s], blk_dev [%x], v_type [%x]\n", full_dev_path, blk_dev, blk_dev->v_type);
	// string sPath(full_dev_path);
	// {
	// 	sync lock(vfsMtx);
	// 	dev_list[sPath] = blk_dev;
	// }
	create_partition_dev(blk_dev);
	return 0;
}

vfs::vfs(void)
{
	printf("Initializing VFS\n");
}

vfs::~vfs(void)
{
	printf("\nde-Initialisint vfs");
}

vnode::vnode() : v_count(0)
{
}

vnode::~vnode()
{
}
int vnode::open(unsigned int, vfile *&) { return ENOSYS; }
int vnode::ioctl(unsigned int, void *, int) { return ENOSYS; }
int vnode::lookup(char const *, vnode *&) { return ENOSYS; }
int vnode::bread(ssize_t, size_t, char *) { return ENOSYS; }
int vnode::mkdir(std::string name, vnode *&pDir)
{
	return ENOSYS;
}

int vnode::mknod(vnode *pNode)
{
	return ENOSYS;
}

int vnode::rename(string name)
{
	v_name = name;
}
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
int vnode::readdir(void) { return ENOSYS; }
int vnode::symlink(void) { return ENOSYS; }
int vnode::readlink(void) { return ENOSYS; }
int vnode::fsync(void) { return ENOSYS; }
int vnode::inactive(void) { return ENOSYS; }
int vnode::bmap(void) { return ENOSYS; }
int vnode::strategy(void) { return ENOSYS; }
int vnode::bwrite(void) { return ENOSYS; }
int vnode::brelse(void) { return ENOSYS; }

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
	vnode *node = 0;
	vfile *file = 0;
	assert(name[0] != '/');
	if (rnode->lookup(&(name.c_str()[1]), node) != 0)
	{
		return -1;
	}
	for (size_t i = name.find_first_of("/", 1); i != -1; i = name.find_first_of("/", i))
	{
		if (node->lookup(&(name.c_str()[i]), node) != 0)
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

int get_vnode_from_abspath(const char *pathname, vnode **pathNode)
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
	vnode *current_vnode = rnode;
	while (pathIt && pathIt[0] && current_vnode)
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
		vnode *subnode = nullptr;
		current_vnode->lookup(path, subnode);
		current_vnode = subnode;
	}
	if (!current_vnode)
	{
		return ENOFILE;
	}
	*pathNode = current_vnode;
	return 0;
}

int mknod(const char *pathname, vnode *dev)
{
	int ret = 0;
	char fileName[256] = {0};
	char folderName[256] = {0};
	vnode *folderNode = nullptr;
	if (const char *fn = strrchar(pathname, '/'))
	{
		strcpy(fileName, fn + 1);
		copy(pathname, fn, folderName);
	}
	else
	{
		return ENOFILE;
	}
	ret = get_vnode_from_abspath(folderName, &folderNode);
	if (ret || !folderNode)
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
