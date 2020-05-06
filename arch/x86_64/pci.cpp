/*
 * Copyright 2009-2019 Srijan Kumar Sharma
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
#include "acpi.h"
#include <map>
#include <vector>
#include <DDI/pci_driver.h>

using namespace std;

//	WARNING: pointer values may change after resize.
vector<pci_device_t> pci_devices;
map<uint32_t, vector<pci_device_t *>> irq_pci_lookup;

static void pci_print_info(PCI_COMMON_CONFIG *config)
{
	const char *info_str[] = {"Unknown",
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
							  "Other Mass Storage Controller",
							  "Ethernet Controller",
							  "VGA Controller",
							  "Host Bridge",
							  "ISA Bridge",
							  "Other Bridge"};

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
						  0x00018000,
						  0x00020000,
						  0x00030000,
						  0x00060000,
						  0x00060100,
						  0x00068000};
	uint32_t id = 0;
	id = (uint32_t)((config->BaseClass << 16) | (config->SubClass << 8) | (config->ProgIf));
	printf("%x\\", id);
	unsigned int index = 0;
	for (uint32_t i = 0; i < (sizeof(info_id) / sizeof(uint32_t)); i++)
	{
		if (id == info_id[i])
		{
			index = i;
			break;
		}
	}
	printf(info_str[index]);
}

void pci_interrupt_handler(retStack_t *ret, general_registers_t *regs)
{
	auto device_bkt_it = irq_pci_lookup.find(ret->interruptNumber);
	if (device_bkt_it == irq_pci_lookup.end())
	{
		printf("Critical failure, IRQ should be in list\n");
		asm("cli;hlt");
	}
	for (auto &pci_dev : device_bkt_it->second)
	{
		auto status = pci_read_status(pci_dev);
		if (status & PCI_STATUS_INTERRUPT)
		{
			if (pci_dev->pDriver && pci_dev->pDriver->interrupt != nullptr)
			{
				pci_dev->pDriver->interrupt(pci_dev);
			}
		}
	}
}

void pci_init_devices()
{
	pci_device_t device = {};
	const auto &pci_irq_routing = get_pci_routing_table();

	printf("Searching for PCI devices....\n");
	for (device.address = 0; device.address < 0x00FFFF00; device.address += 0x100)
	{
		uint32_t vendor = pci_readRegister(&device, 0) & 0xffff;
		if (vendor != 0xffff)
		{
			PCI_COMMON_CONFIG config = {};
			for (uint32_t index = 0; index < sizeof(PCI_COMMON_CONFIG); index += 4)
			{
				((uint32_t *)&config)[(index / 4)] = pci_readRegister(&device, index);
			}
			printf("PCI:%x\\", device.address);
			pci_print_info(&config);
			printf("\n");
			device.has_irq = false;
			if (config.type0.InterruptPin != 0)
			{
				//	find IRQ from routing table
				for (const auto &pci_routing : pci_irq_routing)
				{
					//	match filter. ACPI INTA starts at 0 but PCI config space INTA starts at 1.
					if ((((pci_routing.address >> 16) & 0xFF) == device.slot) && (config.type0.InterruptPin == (pci_routing.pin + 1)))
					{
						device.ioapic_pin = pci_routing.irq;
						device.irq_no = IRQ(pci_routing.irq);
						device.has_irq = true;
					}
				}
				if (!device.has_irq)
				{
					printf("Route not found\n");
					asm("cli;hlt");
				}
			}
			device.bIsProcessed = false;
			pci_devices.push_back(device);
		}
	}
	for (auto &pci_device : pci_devices)
	{
		if (irq_pci_lookup.find(pci_device.irq_no) == irq_pci_lookup.end())
			register_interrupt_handler(pci_device.irq_no, pci_interrupt_handler);
		irq_pci_lookup[pci_device.irq_no].push_back(&pci_device);
	}

	printf("Search completed\n");
}

uint32_t pci_readRegister(const pci_device_t *device, uint32_t offset)
{
	if (offset & 0x03)
	{
		printf("Offset has to be 4 byte aligned\n");
		asm("cli;hlt");
	}
	uint32_t address = device->address;

	/* create configuration address */
	address |= ((offset & 0xfc) | ((uint32_t)0x80000000));

	/* write out the address */
	outl(0xCF8, address);
	/* read in the data */
	return inl(0xCFC);
}

void pci_writeRegister(const pci_device_t *device, uint32_t offset, uint32_t value)
{
	uint32_t address = device->address;

	/* create configuration address */
	address |= ((offset & 0xfc) | ((uint32_t)0x80000000));

	/* write out the address */
	outl(0xCF8, address);
	/* write the data */
	outl(0xCFC, value);
}

std::vector<pci_device_t> &pci_getDevices(void)
{
	return pci_devices;
}

void pci_device_t::getDeviceId(pci_device_id *devID) const
{
	uint32_t tmp = pci_readRegister(this, 0);
	devID->VendorID = (uint16_t)(tmp & 0xFFFF);
	devID->DeviceID = (uint16_t)((tmp >> 16) & 0xFFFF);
	tmp = pci_readRegister(this, 0x08);
	devID->Class = (uint8_t)((tmp >> 24) & 0xFF);
	devID->SubClass = (uint8_t)((tmp >> 16) & 0xFF);
	tmp = pci_readRegister(this, 0x2C);
	devID->SubVendorID = (uint16_t)((tmp)&0xFFFF);
	devID->SubSystemID = (uint16_t)((tmp >> 16) & 0xFFFF);
}

uint32_t pci_resource_start(pci_device_t *dev, uint32_t bar)
{
	uint32_t addr = pci_readRegister(dev, 0x10 + (bar * 4));
	if (addr != 0 && (addr % 2) == 0)
	{
		pci_writeRegister(dev, 0x10 + (bar * 4), 0xFFFFFFFF);
		uint32_t size = (pci_readRegister(dev, 0x10 + (bar * 4)) & 0xFFFFFFF0);
		pci_writeRegister(dev, 0x10 + (bar * 4), addr);
		size = ((~size) + 1);
		printf("Mapping %x size %x\n", addr, size);
		for (size_t paddr = (addr / PageManager::PAGESIZE) * PageManager::PAGESIZE; paddr < addr + size; paddr += PageManager::PAGESIZE)
			PageManager::getInstance()->IdentityMap2MBPages(paddr);
		return addr & 0xFFFFFFF0;
	}
	return addr;
}

bool pci_device_id::IsMatching(const pci_device_id &in) const
{
	return (VendorID == in.VendorID || VendorID == 0xFFFF) && (DeviceID == in.DeviceID || DeviceID == 0xFFFF) && (SubVendorID == in.SubVendorID || SubVendorID == 0xFFFF) && (SubSystemID == in.SubSystemID || SubSystemID == 0xFFFF) && (Class == in.Class || Class == 0xFF) && (SubClass == in.SubClass || SubClass == 0xFF) && (ProgIf == in.ProgIf || ProgIf == 0xFF);
}

uint16_t pci_read_status(const pci_device_t *device)
{
	return (uint16_t)(pci_readRegister(device, 4) >> 16);
}