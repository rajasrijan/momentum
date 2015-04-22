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
#include "paging.h"
#include "../../kernel/lists.h"

vector_list_t pci_devices;

static void print_pci_info(PCI_COMMON_CONFIG *config)
{
    const char* info_str[] = {"Unknown",
        "SCSI Bus Controller",
        "IDE Controller",
        "Floppy Disk Controller",
        "IPI Bus Controller",
        "RAID Controller",
        "ATA Controller (Single DMA)",
        "ATA Controller (Chained DMA)",
        "Serial ATA (Vendor Specific Interface)",
        "Serial ATA (AHCI 1.0)",
        "Serial Attached SCSI (SAS)",
        "Other Mass Storage Controller"};

    uint32_t info_id[] = {0,
        0x00010000,
        0x00010100,
        0x00010200,
        0x00010300,
        0x00010400,
        0x00010520,
        0x00010530,
        0x00010600,
        0x00010601,
        0x00010700,
        0x00018000};
    uint32_t id = 0;
    id = (uint32_t)((config->BaseClass << 16) | (config->SubClass << 8) | (config->ProgIf));
    printf("%x\\",id);
    unsigned int index=0;
    for(uint32_t i=0;i<(sizeof(info_id)/sizeof(uint32_t));i++)
    {
        if(id==info_id[i])
        {
            index=i;
            break;
        }
    }
    printf(info_str[index]);
}

void init_pci_devices()
{
    pci_device_t device;
    vector_init_fn(&pci_devices, sizeof (pci_device_t));
    printf("\nSearching for PCI devices....");
    for (device.address = 0; device.address < 0x00FFFF00; device.address += 0x100)
    {
        uint32_t vendor = readPciRegister(&device, 0)&0xffff;
        if (vendor != 0xffff)
        {
            PCI_COMMON_CONFIG config;
            for (uint32_t index = 0; index<sizeof (PCI_COMMON_CONFIG); index += 4)
                ((uint32_t*) & config)[(index / 4)] = readPciRegister(&device, index);
            printf("\nPCI:%x\\", device.address);
            print_pci_info(&config);
            pci_devices.push(&pci_devices, &device);
        }
    }
    printf("\nSearch completed.");
}

uint32_t readPciRegister(const pci_device_t *device, uint32_t offset)
{
    uint32_t address = device->address;

    /* create configuration address as per Figure 1 */
    address |= ((offset & 0xfc) | ((uint32_t) 0x80000000));

    /* write out the address */
    outl(0xCF8, address);
    /* read in the data */
    /* (offset & 2) * 8) = 0 will choose the fisrt word of the 32 bits register */
    return ((uint32_t) ((inl(0xCFC) >> ((offset & 2) * 8))));
}

void writePciRegister(const pci_device_t *device, uint32_t offset, uint32_t value)
{
    uint32_t address = device->address;

    /* create configuration address as per Figure 1 */
    address |= ((offset & 0xfc) | ((uint32_t) 0x80000000));

    /* write out the address */
    outl(0xCF8, address);
    /* write the data */
    outl(0xCFC, value);
}

const vector_list_t* getPciDevices(void)
{
    return &pci_devices;
}

void getDeviceId(const pci_device_t *dev, pci_device_id *devID)
{
    uint32_t tmp = readPciRegister(dev, 0);
    devID->VendorID = (uint16_t) (tmp & 0xFFFF);
    devID->DeviceID = (uint16_t) ((tmp >> 16) & 0xFFFF);
    tmp = readPciRegister(dev, 0x08);
    devID->BaseClass = (uint8_t) ((tmp >> 24)&0xFF);
    devID->SubClass = (uint8_t) ((tmp >> 16)&0xFF);
    devID->ProgIf = (uint8_t) ((tmp >> 8)&0xFF);
}

uint32_t pci_resource_start(pci_device_t *dev, uint32_t bar)
{
    uint32_t addr = readPciRegister(dev, 0x10 + (bar * 4));
    writePciRegister(dev, 0x10 + (bar * 4), 0xFFFFFFFF);
    uint32_t size = (readPciRegister(dev, 0x10 + (bar * 4))&0xFFFFFFF0);
    writePciRegister(dev, 0x10 + (bar * 4), addr);
    size = ((~size) + 1);
    printf("\nMapping %x size %x", addr, size);
    force_map(addr, addr, (size + 4096 - 1) / 4096);
    return (addr & 0xFFFFFFF0);
}
