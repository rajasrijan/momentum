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

 #include "pci.h"
#include "string.h"
#include "../../kernel/lists.h"

vector_list_t pci_devices;

void init_pci_devices()
{
    vector_init_fn(&pci_devices, sizeof (PCI_COMMON_CONFIG));
    printf("\n");
    uint16_t tmp[32];
    for (uint16_t l = 0; l < 256; l++)
        for (uint16_t k = 0; k < 32; k++)
            for (uint16_t j = 0; j < 1; j++)
            {
                for (uint16_t i = 0; i < 64; i += 2)
                    tmp[i / 2] = pciConfigReadWord(l, k, j, i);
                if (tmp[0] != 0xFFFF)
                    pci_devices.push(&pci_devices, (void*) tmp);
            }
    printf("\n");
    /*
     * two different loops for debugging purpose.
     * checking if vector list works.
     */
    for (uint32_t i = 0; i < pci_devices.size(&pci_devices); i++)
    {
        PCI_COMMON_CONFIG* dev = (PCI_COMMON_CONFIG*) (pci_devices.at(&pci_devices, i));
        printf("\nVendor :%x  ,device :%x  ,Classcode :%x  ,Subclass :%x ", dev->VendorID, dev->DeviceID, dev->BaseClass, dev->SubClass);
    }
}

unsigned short pciConfigReadWord(unsigned short bus, unsigned short slot,
                                 unsigned short func, unsigned short offset)
{
    unsigned long address;
    unsigned long lbus = (unsigned long) bus;
    unsigned long lslot = (unsigned long) slot;
    unsigned long lfunc = (unsigned long) func;
    unsigned short tmp = 0;

    /* create configuration address as per Figure 1 */
    address = (unsigned long) ((lbus << 16) | (lslot << 11) |
            (lfunc << 8) | (offset & 0xfc) | ((uint32_t) 0x80000000));

    /* write out the address */
    outl(0xCF8, address);
    /* read in the data */
    /* (offset & 2) * 8) = 0 will choose the fisrt word of the 32 bits register */
    tmp = (unsigned short) ((inl(0xCFC) >> ((offset & 2) * 8)) & 0xffff);
    return (tmp);
}
