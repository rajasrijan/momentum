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

#include "pci_driver.h"
#include "../libc/stdlib.h"

std::vector<pci_driver_t *> pci_driver_tables;

int pci_register_driver(pci_driver_t *dev)
{
	pci_driver_tables.push_back(dev);
	printf("Registered [%s]\n", dev->name);

	printf("Supported devices:\n");
	for (int i = 0; i < dev->pci_device_count; i++)
	{
		const pci_device_id &device = dev->deviceTable[i];
		printf("PCI_%x_%x_%x_%x_%x_%x\n", device.VendorID, device.DeviceID,
			   device.SubVendorID, device.SubSystemID, device.Class, device.SubClass);
	}
	return 0;
}

void pci_unregister_driver(pci_driver_t *dev)
{
}

int pci_find_compitable_driver(pci_device_t &device)
{
	int ret = 0;
	pci_device_id dev_id={0};
	device.getDeviceId(&dev_id);
	printf("Trying PCI_%x_%x_%x_%x_%x_%x\n", dev_id.VendorID, dev_id.DeviceID,
		   dev_id.SubVendorID, dev_id.SubSystemID, dev_id.Class, dev_id.SubClass);
	for (pci_driver_t *pci_driver : pci_driver_tables)
	{
		int it = 0;
		for (it = 0; it < pci_driver->pci_device_count; it++)
		{
			if (pci_driver->deviceTable[it].IsMatching(dev_id))
			{
				break;
			}
		}
		if (it != pci_driver->pci_device_count)
		{
			printf("Found driver [%s]\n", pci_driver->name);
			if (!pci_driver->probe(&device, pci_driver->deviceTable[it]))
			{
				device.pDriver=pci_driver;
				printf("Driver registered\n");
			}
		}
	}
	device.bIsProcessed = true;
	return ret;
}
