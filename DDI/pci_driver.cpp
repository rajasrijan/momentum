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

#include "pci_driver.h"
#include "../kernel/lists.h"
#include "../libc/stdlib.h"

vector_list_t *pci_driver_tables;

int pci_register_driver(pci_driver_t *dev)
{
    if (pci_driver_tables != 0)
    {
        if (pci_driver_tables->push(pci_driver_tables, dev) == -1)
            return -1;
    }
    else
    {
        pci_driver_tables = (vector_list_t*) calloc(sizeof (vector_list_t), 1);
        vector_init_fn(pci_driver_tables, sizeof (pci_driver_t *));
        if (pci_driver_tables->push(pci_driver_tables, &dev) == -1)
            return -1;
    }
    printf("\nRegistered \"%s\"", dev->name);
    const vector_list_t* device_list = pci_getDevices();
    pci_device_t* pci_device = (pci_device_t*) device_list->data;
    uint32_t no_devices = device_list->size(device_list);

    for (int i = 0; i < no_devices; i++)
    {
        pci_device_id devID;
        pci_getDeviceId(&pci_device[i], &devID);
        if (devID.BaseClass == dev->table.BaseClass)
        {
            printf("\nMatch found.");
            dev->probe(&pci_device[i], devID);
        }

    }
    return 0;
}

void pci_unregister_driver(pci_driver_t *dev)
{

}