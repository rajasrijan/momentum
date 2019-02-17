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

#ifndef BLOCK_DRIVER_H
#define BLOCK_DRIVER_H

#include "../kernel/vfs.h"
#include <string>
#include <vector>

class blockInterface
{
  protected:
    std::string name;
    size_t blockSize;

  public:
    blockInterface(const string &_name, size_t blkSz) : name(_name), blockSize(blkSz)
    {
    }
    virtual ~blockInterface() {}
    virtual int read(size_t offset, size_t count, void *data) = 0;
    virtual int write(size_t offset, size_t count, void *data) = 0;

    std::string getName()
    {
        return name;
    }

    size_t getBlockSize()
    {
        return blockSize;
    }
};

extern std::vector<blockInterface *> blockDeviceList;

int register_blkdev(blockInterface *blockDrive);
int unregister_blkdev(const char *name);

#endif /* BLOCK_DRIVER_H */
