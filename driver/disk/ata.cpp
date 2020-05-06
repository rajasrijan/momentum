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
#include "ata.h"
#include <arch/x86_64/pci.h>
#include <arch/x86_64/timer.h>
#include <DDI/driver.h>
#include <DDI/pci_driver.h>
#include <DDI/block_driver.h>
#include <errno.h>

#define ATA_HDDEVSEL_PORT(x) (x + 6)
#define ATA_COMMAND_PORT(x) (x + 7)

enum ATA_STATUS
{
	ERR = 1,
	DRQ = 8,
	SRV = 16,
	DF = 32,
	RDY = 64,
	BSY = 128
};

enum ATA_CONTROL
{
	nIEN = 2,
	SRST = 4,
	HOB = 0x80
};

enum ATA_COMMAND
{
	READ_PIO = 0x20,
	READ_PIO_EXT = 0x24,
	READ_DMA = 0xC8,
	READ_DMA_EXT = 0x25,
	WRITE_PIO = 0x30,
	WRITE_PIO_EXT = 0x34,
	WRITE_DMA = 0xCA,
	WRITE_DMA_EXT = 0x35,
	CACHE_FLUSH = 0xE7,
	CACHE_FLUSH_EXT = 0xEA,
	PACKET = 0xA0,
	IDENTIFY_PACKET = 0xA1,
	IDENTIFY = 0xEC
};

#pragma pack(1)
struct ata_identity
{
	uint16_t DEVICETYPE;  //0
	char pad1[118];		  //2
	uint32_t MAX_LBA;	 //120
	char pad2[76];		  //124
	uint32_t MAX_LBA_EXT; //200
};

class ata_blk_vnode : public vnode
{
  private:
	uint16_t data_port;
	bool IsMaster;

  public:
	ata_blk_vnode(uint16_t dataPort, bool isMaster, const string &name) : vnode(nullptr), data_port(dataPort), IsMaster(isMaster)
	{
		setName(name.c_str());
		v_type = VBLK;
		printf("creating interface for [%s]\n", getName().c_str());
	}
	~ata_blk_vnode()
	{
	}
	int bread(ssize_t position, size_t size, char *data, int *bytesRead);
	int write(size_t offset, size_t count, void *data)
	{
		printf("%s not implemented\n", __FUNCTION__);
		__asm__("cli;hlt");
		return -ENOSYS;
	}
	int readdir(vector<shared_ptr<vnode>> &vnodes)
	{
		printf("Not implemented");
		asm("cli;hlt;");
		return 0;
	}
	int ioctl(uint32_t command, void *data, int fflag);
	int open(uint32_t flags) { return -ENOSYS; }
};

uint16_t control_register = 0x3F6;
static uint8_t disk_no = 0;

int ataReadSectors(uint16_t data_port, bool IsMaster, size_t offset, size_t count, void *data);
int ataSoftReset();
int ata_probe(pci_device_t *dev, pci_device_id table);
void ata_remove(pci_device_t *dev);
int ata_suspend(pci_device_t *dev, uint32_t state);
int ata_resume(pci_device_t *dev);
int ataIdentify(ata_identity &ident, uint16_t data_port = 0x1F0, bool IsMaster = true);

int ata_blk_vnode::bread(ssize_t position, size_t size, char *data, int *bytesRead)
{
	if (bytesRead)
		*bytesRead = (size * 512);
	return ataReadSectors(data_port, IsMaster, position, size, data);
}

int ata_blk_vnode::ioctl(uint32_t command, void *data, int fflag)
{
	switch (command)
	{
	case BLKGETSIZE64:
	{
		ata_identity ident = {};
		ataIdentify(ident, data_port, IsMaster);
		*((uint64_t *)data) = ident.MAX_LBA;
	}
	break;
	default:
		return EINVAL;
	}
	return 0;
}

pci_device_id supportedDevices[] = {
	{(uint16_t)PCI_ANY_ID, (uint16_t)PCI_ANY_ID, (uint16_t)PCI_ANY_ID, (uint16_t)PCI_ANY_ID, PCI_BASE_CLASS_STORAGE, PCI_CLASS_STORAGE_IDE, (uint8_t)PCI_ANY_ID},
	{(uint16_t)PCI_ANY_ID, (uint16_t)PCI_ANY_ID, (uint16_t)PCI_ANY_ID, (uint16_t)PCI_ANY_ID, PCI_BASE_CLASS_STORAGE, PCI_CLASS_STORAGE_ATA, (uint8_t)PCI_ANY_ID},
};

pci_driver_t ata_pci_driver_interface = {"IDE PCI Driver",
										 supportedDevices,
										 sizeof(supportedDevices) / sizeof(supportedDevices[0]),
										 ata_probe,
										 ata_remove,
										 ata_suspend,
										 ata_resume};

void ata_init()
{
	printf("ata_init\n");
	pci_register_driver(&ata_pci_driver_interface);
}

int ata_probe(pci_device_t *dev, pci_device_id table)
{
	int ret = 0;
	if (table.SubClass != PCI_CLASS_STORAGE_IDE)
	{
		printf("Storage device not supported\n");
		return 1;
	}
	printf("Device probed\n");
	pci_device_id devID;
	dev->getDeviceId(&devID);
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
		//printf("BAR[%d]=0x%x\n", j, BAR[j]);
	}
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
				ret = 0; // non fatal.
			}
		}
	}
	return ret;
}

void ata_remove(pci_device_t *dev)
{
}

int ata_suspend(pci_device_t *dev, uint32_t state)
{
	return 0;
}

int ata_resume(pci_device_t *dev)
{
	return 0;
}

int ataReadSectors(uint16_t data_port, bool IsMaster, size_t offset, size_t count, void *data)
{
	while (count > 0)
	{
		auto size = min(count, 0xfful);
		offset &= 0x0FFFFFFF;
		uint32_t status = 0;
		uint16_t porta = data_port + 6; //Where bit 24 to 27 are sent.
		uint16_t portb = data_port + 2; //Where no of sectors is sent.
		uint16_t portc = data_port + 3; //Where nit 0 to 7 are sent.
		uint16_t portd = data_port + 4; //Where nit 8 to 15 are sent.
		uint16_t porte = data_port + 5; //Where nit 16 to 23 are sent.
		uint16_t command_port = data_port + 7;

		uint8_t bits_0_to_7 = (uint8_t)(offset & 0xFF);
		uint8_t bits_8_to_15 = (uint8_t)((offset >> 8) & 0xFF);
		uint8_t bits_16_to_23 = (uint8_t)((offset >> 16) & 0xFF);
		uint8_t bits_24_to_27 = (uint8_t)((offset >> 24) | 0xE0 | (IsMaster ? 0 : 16));

		//  Send no of sectors to get.
		outb(portb, (uint8_t)size);
		//  Send bit 0 to 7.
		outb(portc, bits_0_to_7);
		//  Send bit 8 to 15.
		outb(portd, bits_8_to_15);
		//  Send bit 0 to 7.
		outb(porte, bits_16_to_23);
		//  Send bit 24 to 27.
		outb(porta, bits_24_to_27);
		//  read with retry
		outb(command_port, READ_PIO);

		//while (((status = inb(command_port)) & BSY) != BSY);
		for (size_t c = 0; c < size; c++)
		{
			while (true)
			{
				status = inb(command_port);
				assert(status & (ERR | DF));
				if (status & DRQ)
					break;
			}
			//printf("\nout of loop.YAY");
			uint16_t *dst = ((uint16_t *)data);

			for (int i = 0; i < 256; i++)
			{
				uint16_t tmp = inw(data_port);
				dst[(c * 256) + i] = tmp;
			}
		}
		count -= size;
		offset += size;
		data = (void *)&((char *)data)[((512) * size)];
	}
	return 0;
}

int ataSoftReset()
{
	outb(control_register, SRST);
	outb(control_register, 0);
	return 0;
}

int ataIdentify(ata_identity &ident, uint16_t data_port, bool IsMaster)
{
	int ret = 0;
	// (I) Select Drive:
	outb(ATA_HDDEVSEL_PORT(data_port), 0xA0 | (IsMaster ? 0 : 16)); // Select Drive.
	//  read with retry
	outb(ATA_COMMAND_PORT(data_port), IDENTIFY);
	while (true)
	{
		int status = inb(ATA_COMMAND_PORT(data_port));
		if ((status == 0) || (status & (ERR | DF)) != 0)
		{
			return 1;
		}
		if (status & DRQ)
			break;
	}
	uint16_t dst[256] = {0};
	for (int i = 0; i < 256; i++)
	{
		dst[i] = inw(data_port);
	}
	ident = *((ata_identity *)dst);
	return ret;
}
MODULT_INIT(ata_init)