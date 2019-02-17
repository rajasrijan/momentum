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
#include "ramdrive.h"
#include "stdlib.h"
#include "string.h"
#include "errno.h"
#include <kernel/vfs.h>
#include <utility>

class ramfs_vfs : public vfs
{
  public:
	ramfs_vfs()
	{
	}
	~ramfs_vfs()
	{
	}
	int mount(uint64_t flags, shared_ptr<vnode> blk_dev, shared_ptr<vnode> &fs_root_directory) { return 1; }
	int unmount(void) { return 1; }
	int root(vnode *&rootNode) { return 1; }
	int statfs(shared_ptr<vnode> rootNode, statfs_t &statfs) { return 1; }
	int sync(void) { return 1; }
	int fid(void) { return 1; }
	int vget(void) { return 1; }

  private:
};

class ramfs_vnode : public vnode
{
  public:
	int open(uint32_t flags, class vfile *&file);
	int close(void);
	int rdwr(void);
	int ioctl(uint32_t command, void *data, int fflag);
	int select(void);
	int getattr(void);
	int setattr(void);
	int access(void);
	int lookup(const char *const path, shared_ptr<vnode> &foundNode);
	int create(void);
	int remove(void);
	int link(void);
	int rename(string name);
	int mkdir(std::string name, shared_ptr<vnode> &pDir);
	int rmdir(void);
	int readdir(vector<shared_ptr<vnode>> &vnodes);
	int symlink(void);
	int readlink(void);
	int fsync(void);
	int inactive(void);
	int bmap(void);
	int strategy(void);
	int bread(ssize_t position, size_t size, char *data);
	int bwrite(void);
	int brelse(void);
};

int ramdrive_init(void)
{
	shared_ptr<ramfs_vfs> root_vfs(new ramfs_vfs);
	extern std::vector<shared_ptr<ramfs_vfs>> vfs_list;
	vfs_list.push_back(root_vfs);
	return 0;
}
