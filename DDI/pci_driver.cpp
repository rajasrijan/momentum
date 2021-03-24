/*
 * Copyright 2009-2021 Srijan Kumar Sharma
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
#include <stdlib.h>
#include <stdio.h>
#include "pci_driver.h"

std::vector<pci_driver_interface *> pci_driver_tables;

int pci_register_driver(pci_driver_interface *dev)
{
    pci_driver_tables.push_back(dev);
    printf("Registered PCI driver\n");

    printf("Supported devices:\n");
    for (int i = 0; i < dev->pci_device_count; i++) {
        const pci_device_id &device = dev->deviceTable[i];
        printf("PCI_%x_%x_%x_%x_%x_%x\n", device.VendorID, device.DeviceID, device.SubVendorID, device.SubSystemID, device.Class, device.SubClass);
    }
    return 0;
}

void pci_unregister_driver(pci_driver_interface *dev)
{
}

int pci_find_compitable_driver(pci_device_t &device)
{
    int ret = 0;
    pci_device_id dev_id = {0, 0, 0, 0, 0, 0, 0};
    device.getDeviceId(&dev_id);
    printf("Trying PCI_%x_%x_%x_%x_%x_%x\n", dev_id.VendorID, dev_id.DeviceID, dev_id.SubVendorID, dev_id.SubSystemID, dev_id.Class, dev_id.SubClass);
    for (pci_driver_interface *pci_driver : pci_driver_tables) {
        int it = 0;
        for (it = 0; it < pci_driver->pci_device_count; it++) {
            if (pci_driver->deviceTable[it].IsMatching(dev_id)) {
                break;
            }
        }
        if (it != pci_driver->pci_device_count) {
            printf("Found driver\n");
            device.pDriver = pci_driver->create_driver_instance(&device);
            if (device.pDriver->probe(&device, dev_id))
                device.pDriver = nullptr;
            else
                printf("Driver registered\n");
        }
    }
    device.bIsProcessed = true;
    return ret;
}
