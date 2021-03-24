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

enum ATA_STATUS {
    ERR = 1,
    DRQ = 8,
    SRV = 16,
    DF = 32,
    RDY = 64,
    BSY = 128
};

enum ATA_CONTROL {
    nIEN = 2,
    SRST = 4,
    HOB = 0x80
};

enum ATA_COMMAND {
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

#pragma pack(push, 1)
struct ata_identity {
    uint16_t DEVICETYPE;  // 0
    char pad1[118];       // 2
    uint32_t MAX_LBA;     // 120
    char pad2[76];        // 124
    uint32_t MAX_LBA_EXT; // 200
};
#pragma pack(pop)

int ide_probe(pci_device_t *dev);