/*
 * Copyright 2009-2020 Srijan Kumar Sharma
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

#include <kernel/vfs.h>
#include "sata3.0.h"

class sata_blk_vnode : public vnode
{
    HBA_MEM volatile *abar;
    HBA_PORT volatile *port;
    size_t portId;
    HBA_CMD_HEADER volatile *command_list;
    HBA_FIS volatile *fis_list;
    mtx_t port_lock;

  public:
    mtx_t cmd_notify;

    sata_blk_vnode(HBA_MEM volatile *_abar, size_t _portId, const string &_name);
    ~sata_blk_vnode();
    int bread(ssize_t position, size_t size, char *data, int *bytesRead);
    int readdir(vector<shared_ptr<vnode>> &vnodes);
    int open(uint64_t flags);
    int ioctl(uint32_t command, void *data, int fflag);
    int getCommandSlot();
    int identify(ata_identity &ident);
    int disableFISReceive();
    int enableFISReceive();
    int disableCommand();
    int enableCommand();
};
