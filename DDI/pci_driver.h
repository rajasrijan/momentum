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

#ifndef PCI_DRIVER_H
#define	PCI_DRIVER_H

#include <stdint.h>
#include "../arch/x86/pci.h"
#include "pci_ids.h"

#define PCI_ANY_ID ((uint32_t)(~0))
#define PCI_VDEVICE(vendor, device) PCI_VENDOR_ID_##vendor, (device),PCI_ANY_ID, PCI_ANY_ID, 0, 0

typedef int (*pci_probe_t)(pci_device_t *dev, pci_device_id table);
typedef void (*pci_remove_t) (pci_device_t *dev);
typedef int (*pci_suspend_t) (pci_device_t *dev, uint32_t state);
typedef int (*pci_resume_t) (pci_device_t *dev);

typedef struct pci_driver
{
    const char* name;
    pci_device_id table;
    pci_probe_t probe;
    pci_remove_t remove;
    pci_suspend_t suspend;
    pci_resume_t resume;
} pci_driver_t;

extern vector_list_t *pci_driver_tables;

int pci_register_driver(pci_driver_t *dev);
void pci_unregister_driver(pci_driver_t *dev);

#endif	/* PCI_DRIVER_H */
