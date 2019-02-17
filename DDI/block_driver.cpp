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
#include <errno.h>

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

int register_blkdev(blockInterface *blockDrive)
{
	return ENOSYS;
}

int unregister_blkdev(const char *name)
{
	return ENOSYS;
}