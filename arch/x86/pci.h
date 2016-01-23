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

#ifndef PCI_H
#define PCI_H

#include "global.h"

struct _PCI_HEADER_TYPE_0
{
    uint32_t BaseAddresses[6];
    uint32_t CIS;
    uint16_t SubVendorID;
    uint16_t SubSystemID;
    uint32_t ROMBaseAddress;
    uint32_t Reserved2[2];

    uint8_t InterruptLine;
    uint8_t InterruptPin;
    uint8_t MinimumGrant;
    uint8_t MaximumLatency;
};

struct _PCI_HEADER_TYPE_1
{
    uint32_t BaseAddresses[2];
    uint8_t PrimaryBusNumber;
    uint8_t SecondaryBusNumber;
    uint8_t SubordinateBusNumber;
    uint8_t SecondaryLatencyTimer;
    uint8_t IOBase;
    uint8_t IOLimit;
    uint16_t SecondaryStatus;
    uint16_t MemoryBase;
    uint16_t MemoryLimit;
    uint16_t PrefetchableMemoryBase;
    uint16_t PrefetchableMemoryLimit;
    uint32_t PrefetchableMemoryBaseUpper32;
    uint32_t PrefetchableMemoryLimitUpper32;
    uint16_t IOBaseUpper;
    uint16_t IOLimitUpper;
    uint32_t Reserved2;
    uint32_t ExpansionROMBase;
    uint8_t InterruptLine;
    uint8_t InterruptPin;
    uint16_t BridgeControl;
};

struct _PCI_HEADER_TYPE_2
{
    uint32_t BaseAddress;
    uint8_t CapabilitiesPtr;
    uint8_t Reserved2;
    uint16_t SecondaryStatus;
    uint8_t PrimaryBusNumber;
    uint8_t CardbusBusNumber;
    uint8_t SubordinateBusNumber;
    uint8_t CardbusLatencyTimer;
    uint32_t MemoryBase0;
    uint32_t MemoryLimit0;
    uint32_t MemoryBase1;
    uint32_t MemoryLimit1;
    uint16_t IOBase0_LO;
    uint16_t IOBase0_HI;
    uint16_t IOLimit0_LO;
    uint16_t IOLimit0_HI;
    uint16_t IOBase1_LO;
    uint16_t IOBase1_HI;
    uint16_t IOLimit1_LO;
    uint16_t IOLimit1_HI;
    uint8_t InterruptLine;
    uint8_t InterruptPin;
    uint16_t BridgeControl;
    uint16_t SubVendorID;
    uint16_t SubSystemID;
    uint32_t LegacyBaseAddress;
    uint8_t Reserved3[56];
    uint32_t SystemControl;
    uint8_t MultiMediaControl;
    uint8_t GeneralStatus;
    uint8_t Reserved4[2];
    uint8_t GPIO0Control;
    uint8_t GPIO1Control;
    uint8_t GPIO2Control;
    uint8_t GPIO3Control;
    uint32_t IRQMuxRouting;
    uint8_t RetryStatus;
    uint8_t CardControl;
    uint8_t DeviceControl;
    uint8_t Diagnostic;
};

typedef struct _PCI_COMMON_CONFIG
{
    uint16_t VendorID;
    uint16_t DeviceID;
    uint16_t Command;
    uint16_t Status;
    uint8_t RevisionID;
    uint8_t ProgIf;
    uint8_t SubClass;
    uint8_t BaseClass;
    uint8_t CacheLineSize;
    uint8_t LatencyTimer;
    uint8_t HeaderType;
    uint8_t BIST;

    union
    {
        struct _PCI_HEADER_TYPE_0 type0;
        struct _PCI_HEADER_TYPE_1 type1;
        struct _PCI_HEADER_TYPE_2 type2;
    };
    uint8_t DeviceSpecific[108];

} PCI_COMMON_CONFIG;

typedef union
{

    struct
    {
        uint8_t resv1;
        uint8_t func : 3;
        uint8_t slot : 5;
        uint8_t bus;
        uint8_t resv0 : 7;
        uint8_t enable : 1;
    } __attribute__((packed));
    uint32_t address;
} pci_device_t;

typedef struct
{
    uint16_t VendorID;
    uint16_t DeviceID;
    uint8_t ProgIf;
    uint8_t SubClass;
    uint8_t BaseClass;
} __attribute__((packed)) pci_device_id;

uint32_t pci_resource_start(pci_device_t *dev, uint32_t bar);
uint32_t pci_resource_end(pci_device_t *dev, uint32_t bar);
uint32_t pci_resource_flags(pci_device_t *dev, uint32_t bar);
uint32_t pci_readRegister(const pci_device_t *device, uint32_t offset);
void pci_writeRegister(const pci_device_t *device, uint32_t offset, uint32_t value);
const vector_list_t* pci_getDevices(void);
void pci_init_devices(void);
void pci_getDeviceId(const pci_device_t *dev, pci_device_id *devID);
#endif /* PCI_H */
