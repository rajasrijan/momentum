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

#include <stdint.h>
#include <vector>
#include <DDI/driver.h>
#include <DDI/pci_driver.h>
#include <kernel/vfs.h>
#include "ide.h"
#include "ata.h"
#include <string>

using namespace std;

static uint8_t disk_no = 0;

int ide_probe(pci_device_t *dev)
{
    int ret = 0;
    uint32_t standard_bars[] = {0x1F0, 0x3F6, 0x170, 0x376};
    uint32_t BAR[6] = {0};
    for (uint32_t j = 0; j < 6; j++)
    {
        BAR[j] = pci_resource_start(dev, j);
        if (j < 4)
        {
            //  Replace BAR[j] with standard bars if it is 0x0 or 0x1
            BAR[j] = ((BAR[j] == 0) || (BAR[j] == 1)) ? standard_bars[j] : BAR[j];
        }
        // printf("BAR[%d]=0x%x\n", j, BAR[j]);
    }
    // start setup DMA
    // end setup DMA
    ret = -ENODEV;
    for (int bus = 0; bus < 2; bus++)
    {
        for (int master = 0; master <= 1; master++)
        {
            ata_identity ident = {};
            if (ataIdentify(ident, (uint16_t)BAR[bus * 2], !master))
            {
                continue;
            }
            
            printf("IDE drive at [%s] [%s]\n", !bus ? "primary" : "secondry", !master ? "master" : "slave");
            string disk_name = "hda";
            disk_name.back() += disk_no++;
            auto ataBlkInterface = new ata_blk_vnode(BAR[bus * 2], !master, disk_name);
            ret = mknod(("/dev/" + disk_name).c_str(), ataBlkInterface);
            if (ret)
            {
                printf("Failed to register block device\n");
                // ret = 0; // non fatal.
            }
        }
    }
    return ret;
}