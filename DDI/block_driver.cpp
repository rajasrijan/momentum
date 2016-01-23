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

#include "block_driver.h"
#include "stdio.h"
#include <vector>
#include <memory>
#include <string>
#include "partition_table.h"
#include "../kernel/vfs.h"
#include "filesystem.h"
#include "arch/x86/global.h"
#include <kernel/vfs.h>
#include <map>

using namespace std;

std::vector<blockInterface*> blockDeviceList;

class blk_vnode : public vnode
{
private:
    blockInterface* blockDrive;
public:

    blk_vnode(blockInterface* _blockDrive) : vnode(0)
    {
        blockDrive = _blockDrive;
        v_type = VBLK;
        v_name = _blockDrive->getName().c_str();
        printf("\nthis [%x],flags [%x],flags [%x]", this, v_type, VBLK);
        //DBG_OUTPUT
    }

    ~blk_vnode()
    {
    }

    int bread(int position, int size, shared_ptr<char> data)
    {
        //printf("\nread called");
        blockDrive->read(position, size, data.get());
    }
};

int register_blkdev(blockInterface* blockDrive)
{
    //  Registering block device.
    printf("\nRegistering block device [%s].", blockDrive->getName().c_str());
    blockDeviceList.push_back(blockDrive);
    blk_vnode* blkNode = new blk_vnode(blockDrive);

    printf("\nblkNode [%x],flags [%x]", blkNode, blkNode->v_type);
    add_dev(blkNode);
    //  Searching for partitions.
    printf("\nSearching for partitions.");
    PartitionTable pTable;
    blockDrive->read(0, 1, &pTable);
    printf("\nBoot signature [%x]", (uint32_t) pTable.boot_signature);
    for (int i = 0; i < 4; i++)
    {
        uint32_t status = pTable.partition[i].status;
        //printf("\nPartition [%d], status [%x]...%s", i, status, (status == 0x80) ? "valid" : "invalid");
    }
    return 0;
}

int unregister_blkdev(const char *name)
{

}