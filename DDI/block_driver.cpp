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

#include "block_driver.h"
#include "stdio.h"
#include <vector>
#include <memory>
#include <string>
#include "../kernel/vfs.h"
#include "arch/x86_64/global.h"
#include <kernel/vfs.h>
#include <map>
#include <climits>
#include <string.h>

using namespace std;

class blkdev
{
  private:
	uint8_t major;
	char name[256];

  public:
	blkdev(uint8_t _major, const char *_name) : major(_major)
	{
		strcpy(name, _name);
	}
	~blkdev()
	{
	}
	const char *getName()
	{
		return name;
	}
	int getMajor() const
	{
		return major;
	}
};

std::vector<blockInterface *> blockDeviceList;

std::vector<blkdev> g_vBlkdev;

class blk_vnode : public vnode
{
  private:
	blockInterface *blockDrive;

  public:
	blk_vnode(blockInterface *_blockDrive) : vnode()
	{
		blockDrive = _blockDrive;
		v_type = VBLK;
		v_name = blockDrive->getName().c_str();
		printf("this [%x],flags [%x],flags [%x]\n", this, v_type, VBLK);
	}

	~blk_vnode()
	{
	}

	int lookup(const char *const path, vnode *&foundNode)
	{
		return 1;
	}

	int bread(ssize_t position, size_t size, char *data)
	{
		//printf("read called");
		blockDrive->read(position, size, data);
	}

	int open(uint32_t flags, vfile *&file)
	{
		return -1;
	}

	int ioctl(uint32_t command, void *data, int fflag)
	{
		switch (command)
		{
		case BLKPBSZGET:
			((uint32_t *)data)[0] = blockDrive->getBlockSize();
			break;
		default:
			break;
		}
		return 0;
	}
};

int register_blkdev(blockInterface *blockDrive)
{
	//  Registering block device.
	printf("Registering block device [%s].\n", blockDrive->getName().c_str());
	blockDeviceList.push_back(blockDrive);
	blk_vnode *blkNode = new blk_vnode(blockDrive);

	printf("blkNode [%x],flags [%x]\n", blkNode, blkNode->v_type);
	add_blk_dev(blkNode);
	return 0;
}

int unregister_blkdev(const char *name)
{
}