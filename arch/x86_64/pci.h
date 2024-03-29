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

#ifndef PCI_H
#define PCI_H

#define PCI_STATUS_INTERRUPT (0x08)

struct _PCI_HEADER_TYPE_0
{
    uint32_t BaseAddresses[6];
    uint32_t CIS;
    uint16_t SubVendorID;
    uint16_t SubSystemID;
    uint32_t ROMBaseAddress;
    uint32_t Reserved2[2];

    uint8_t InterruptLine;
    uint8_t InterruptPin; //  1 = INTA
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

struct PCI_COMMON_CONFIG
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

    union {
        struct _PCI_HEADER_TYPE_0 type0;
        struct _PCI_HEADER_TYPE_1 type1;
        struct _PCI_HEADER_TYPE_2 type2;
    };
    uint8_t DeviceSpecific[108];
};

struct pci_device_id
{
    uint16_t VendorID;
    uint16_t DeviceID;
    uint16_t SubVendorID;
    uint16_t SubSystemID;
    uint8_t Class;
    uint8_t SubClass;
    uint8_t ProgIf;
    bool IsMatching(const pci_device_id &in) const;
} __attribute__((packed));

struct pci_device_t
{
    union {
        struct
        {
            uint8_t resv1;
            uint8_t func:3;
            uint8_t slot:5;
            uint8_t bus;
            uint8_t resv0:7;
            uint8_t enable:1;
        } __attribute__((packed));
        uint32_t address;
    };
    bool bIsProcessed;
    uint32_t ioapic_pin;
    uint32_t irq_no;
    bool has_irq;
    class pci_driver *pDriver;
    void getDeviceId(pci_device_id *devID) const;
};

struct pci_routing
{
    uint64_t address; // it's more of a address filter
    uint32_t pin;     //  0 = INTA
    uint32_t irq;
    uint32_t flags;
};

uint32_t pci_resource_start(pci_device_t *dev, uint32_t bar);
uint32_t pci_resource_end(pci_device_t *dev, uint32_t bar);
uint32_t pci_resource_flags(pci_device_t *dev, uint32_t bar);
uint32_t pci_readRegister(const pci_device_t *device, uint32_t offset);
void pci_writeRegister(const pci_device_t *device, uint32_t offset, uint32_t value);
std::vector<pci_device_t> &pci_getDevices(void);
void pci_init_devices(void);
uint16_t pci_read_status(const pci_device_t *device);
#endif /* PCI_H */
