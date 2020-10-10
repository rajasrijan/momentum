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
/*
 * File:   ata.h
 * Author: srijan
 *
 * Created on 1 April, 2014, 5:56 PM
 */

#ifndef ATA_H
#define ATA_H

#include <stdint.h>
#include <kernel/vfs.h>

#ifdef __cplusplus
extern "C"
{
#endif

    void ata_init(void);

#ifdef __cplusplus
}
#endif

int ataIdentify(ata_identity &ident, uint16_t data_port = 0x1F0, bool IsMaster = true);
class ata_blk_vnode : public vnode
{
  private:
    uint16_t data_port;
    bool IsMaster;

  public:
    ata_blk_vnode(uint16_t dataPort, bool isMaster, const string &name);
    ~ata_blk_vnode();
    int bread(ssize_t position, size_t size, char *data, int *bytesRead);
    int bwrite(ssize_t position, size_t size, char *data, int *bytesWritten);
    int write(size_t offset, size_t count, void *data);
    int readdir(vector<shared_ptr<vnode>> &vnodes);
    int ioctl(uint32_t command, void *data, int fflag);
    int open(uint64_t flags)
    {
        return -ENOSYS;
    }
};
#endif /* ATA_H */
