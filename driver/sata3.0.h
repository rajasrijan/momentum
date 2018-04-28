/*
 * Copyright 2009-2017 Srijan Kumar Sharma
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
/*
SATA basic
----------

There are at least two SATA standards maintained respectively by T13 and SATA-IO. The SATA-IO focuses on serial ATA and T13 encompasses traditional parallel ATA specifications as well.

To a software developer, the biggest difference between SATA and parallel ATA is that SATA uses FIS (Frame Information Structure) packet to transport data between host and device, though at hardware level they differ much. An FIS can be viewed as a data set of traditional task files, or an encapsulation of ATA commands. SATA uses the same command set as parallel ATA.

1) FIS types

Following code defines different kinds of FIS specified in Serial ATA Revision 3.0.
 */
#include <stdint.h>
typedef enum
{
    FIS_TYPE_REG_H2D = 0x27, // Register FIS - host to device
    FIS_TYPE_REG_D2H = 0x34, // Register FIS - device to host
    FIS_TYPE_DMA_ACT = 0x39, // DMA activate FIS - device to host
    FIS_TYPE_DMA_SETUP = 0x41, // DMA setup FIS - bidirectional
    FIS_TYPE_DATA = 0x46, // Data FIS - bidirectional
    FIS_TYPE_BIST = 0x58, // BIST activate FIS - bidirectional
    FIS_TYPE_PIO_SETUP = 0x5F, // PIO setup FIS - device to host
    FIS_TYPE_DEV_BITS = 0xA1, // Set device bits FIS - device to host
} FIS_TYPE;

/*
2) Register FIS � Host to Device

A host to device register FIS is used by the host to send command or control to a device. As illustrated in the following data structure, it contains the IDE registers such as command, LBA, device, feature, count and control. An ATA command is constructed in this structure and issued to the device. All reserved fields in an FIS should be cleared to zero.
 */
typedef struct tagFIS_REG_H2D
{
    // DWORD 0
    uint8_t fis_type; // FIS_TYPE_REG_H2D

    uint8_t pmport : 4; // Port multiplier
    uint8_t rsv0 : 3; // Reserved
    uint8_t c : 1; // 1: Command, 0: Control

    uint8_t command; // Command register
    uint8_t featurel; // Feature register, 7:0

    // DWORD 1
    uint8_t lba0; // LBA low register, 7:0
    uint8_t lba1; // LBA mid register, 15:8
    uint8_t lba2; // LBA high register, 23:16
    uint8_t device; // Device register

    // DWORD 2
    uint8_t lba3; // LBA register, 31:24
    uint8_t lba4; // LBA register, 39:32
    uint8_t lba5; // LBA register, 47:40
    uint8_t featureh; // Feature register, 15:8

    // DWORD 3
    uint8_t countl; // Count register, 7:0
    uint8_t counth; // Count register, 15:8
    uint8_t icc; // Isochronous command completion
    uint8_t control; // Control register

    // DWORD 4
    uint8_t rsv1[4]; // Reserved
} FIS_REG_H2D;

/*
3) Register FIS � Device to Host

A device to host register FIS is used by the device to notify the host that some ATA register has changed. It contains the updated task files such as status, error and other registers.
 */
typedef struct tagFIS_REG_D2H
{
    // DWORD 0
    uint8_t fis_type; // FIS_TYPE_REG_D2H

    uint8_t pmport : 4; // Port multiplier
    uint8_t rsv0 : 2; // Reserved
    uint8_t i : 1; // Interrupt bit
    uint8_t rsv1 : 1; // Reserved

    uint8_t status; // Status register
    uint8_t error; // Error register

    // DWORD 1
    uint8_t lba0; // LBA low register, 7:0
    uint8_t lba1; // LBA mid register, 15:8
    uint8_t lba2; // LBA high register, 23:16
    uint8_t device; // Device register

    // DWORD 2
    uint8_t lba3; // LBA register, 31:24
    uint8_t lba4; // LBA register, 39:32
    uint8_t lba5; // LBA register, 47:40
    uint8_t rsv2; // Reserved

    // DWORD 3
    uint8_t countl; // Count register, 7:0
    uint8_t counth; // Count register, 15:8
    uint8_t rsv3[2]; // Reserved

    // DWORD 4
    uint8_t rsv4[4]; // Reserved
} FIS_REG_D2H;

/*
4) Data FIS � Bidirectional

This FIS is used by the host or device to send data payload. The data size can be varied.
 */
typedef struct tagFIS_DATA
{
    // DWORD 0
    uint8_t fis_type; // FIS_TYPE_DATA

    uint8_t pmport : 4; // Port multiplier
    uint8_t rsv0 : 4; // Reserved

    uint8_t rsv1[2]; // Reserved

    // DWORD 1 ~ N
    uint32_t data[1]; // Payload
} FIS_DATA;

/*
5) PIO Setup � Device to Host

This FIS is used by the device to tell the host that it�s about to send or ready to receive a PIO data payload.
 */
typedef struct tagFIS_PIO_SETUP
{
    // DWORD 0
    uint8_t fis_type; // FIS_TYPE_PIO_SETUP

    uint8_t pmport : 4; // Port multiplier
    uint8_t rsv0 : 1; // Reserved
    uint8_t d : 1; // Data transfer direction, 1 - device to host
    uint8_t i : 1; // Interrupt bit
    uint8_t rsv1 : 1;

    uint8_t status; // Status register
    uint8_t error; // Error register

    // DWORD 1
    uint8_t lba0; // LBA low register, 7:0
    uint8_t lba1; // LBA mid register, 15:8
    uint8_t lba2; // LBA high register, 23:16
    uint8_t device; // Device register

    // DWORD 2
    uint8_t lba3; // LBA register, 31:24
    uint8_t lba4; // LBA register, 39:32
    uint8_t lba5; // LBA register, 47:40
    uint8_t rsv2; // Reserved

    // DWORD 3
    uint8_t countl; // Count register, 7:0
    uint8_t counth; // Count register, 15:8
    uint8_t rsv3; // Reserved
    uint8_t e_status; // New value of status register

    // DWORD 4
    uint16_t tc; // Transfer count
    uint8_t rsv4[2]; // Reserved
} FIS_PIO_SETUP;
/*
6) Example

This example illustrates the steps to read the Identify data from a device. Error detection and recovery is ignored.

To issue an ATA Identify command to the device, the FIS is constructed at follows.

FIS_REG_H2D fis;
memset(&fis, 0, sizeof(FIS_REG_H2D));
fis->fis_type = FIS_TYPE_REG_H2D;
fis->command = ATA_CMD_IDENTIFY;	// 0xEC
fis->device = 0;			// Master device
fis->c = 1;				// Write command register

After the device receives this FIS and successfully read the 256 words data into its internal buffer, it sends a PIO Setup FIS � Device to Host to tell the host that it�s ready to transfer data and the data size (FIS_PIO_SETUP.tc).

After the PIO Setup FIS � Device to Host has been sent correctly, the device sends a DATA FIS to the host which contains the received data payload (FIS_DATA.data).

This scenario is described in SATA revision 3.0 as a PIO data-in command protocol. But an AHCI controller will do the latter two steps for the host. The host software needs only setup and issue the command FIS, and tells the AHCI controller the memory address and size to store the received data. After everything is done, the AHCI controller will issue an interrupt (if enabled) to notify the host to check the data. 
 */