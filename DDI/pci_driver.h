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

#ifndef PCI_DRIVER_H
#define PCI_DRIVER_H

#include "../arch/x86_64/pci.h"
#include "pci_ids.h"
#include <errno.h>
#include <stdint.h>

#define PCI_ANY_ID ((uint32_t)(~0))
#define PCI_VDEVICE(vendor, device) PCI_VENDOR_ID_##vendor, (device), PCI_ANY_ID, PCI_ANY_ID, 0, 0

class pci_driver
{
  public:
    const char *name;
    pci_driver(const char *_name) : name(_name)
    {
    }
    virtual ~pci_driver()
    {
    }
    virtual int probe(pci_device_t *, pci_device_id table) = 0;
    virtual int remove()
    {
        return -ENOSYS;
    }
    virtual int suspend()
    {
        return -ENOSYS;
    }
    virtual int resume()
    {
        return -ENOSYS;
    }
    virtual int interrupt()
    {
        return -ENOSYS;
    }
};

struct pci_driver_interface
{
    const pci_device_id *deviceTable;
    const int pci_device_count;
    function<pci_driver *(pci_device_t *)> create_driver_instance;
    function<void(pci_driver *)> destroy_driver_instance;
};

extern std::vector<pci_driver_interface *> pci_driver_tables;

int pci_register_driver(pci_driver_interface *dev);
void pci_unregister_driver(pci_driver_interface *dev);
int pci_find_compitable_driver(pci_device_t &device);

#endif /* PCI_DRIVER_H */
