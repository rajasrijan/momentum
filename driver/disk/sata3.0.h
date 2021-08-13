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

#pragma once
#include <stdint.h>

#define Save_64BitPtr(Reg, Val)                                                                                                                                \
    Reg = (uint32_t)(uint64_t)Val;                                                                                                                             \
    Reg##u = (uint32_t)((uint64_t)Val >> 32)

#define Load_64BitPtr(Reg, Type) (Type *)(((uint64_t)Reg##u << 32) | ((uint64_t)Reg))

enum FIS_TYPE
{
    FIS_TYPE_REG_H2D = 0x27,   // Register FIS - host to device
    FIS_TYPE_REG_D2H = 0x34,   // Register FIS - device to host
    FIS_TYPE_DMA_ACT = 0x39,   // DMA activate FIS - device to host
    FIS_TYPE_DMA_SETUP = 0x41, // DMA setup FIS - bidirectional
    FIS_TYPE_DATA = 0x46,      // Data FIS - bidirectional
    FIS_TYPE_BIST = 0x58,      // BIST activate FIS - bidirectional
    FIS_TYPE_PIO_SETUP = 0x5F, // PIO setup FIS - device to host
    FIS_TYPE_DEV_BITS = 0xA1,  // Set device bits FIS - device to host
};

enum HBA_PxIS
{
    CPDS = 1 << 31, // Cold Port Detect Status (CPDS):
    TFES = 1 << 30, // Task File Error Status (TFES):
    HBFS = 1 << 29, // Host Bus Fatal Error Status (HBFS):
    HBDS = 1 << 28, // Host Bus Data Error Status (HBDS):
    IFS = 1 << 27,  // Interface Fatal Error Status (IFS):
    INFS = 1 << 26, // Interface Non-fatal Error Status (INFS):
    OFS = 1 << 24,  // Overflow Status (OFS):
    IPMS = 1 << 23, // Incorrect Port Multiplier Status (IPMS):
    PRCS = 1 << 22, // PhyRdy Change Status (PRCS):
    DMPS = 1 << 7,  // Device Mechanical Presence Status (DMPS):
    PCS = 1 << 6,   // Port Connect Change Status (PCS):
    DPS = 1 << 5,   // Descriptor Processed (DPS):
    UFS = 1 << 4,   // Unknown FIS Interrupt (UFS):
    SDBS = 1 << 3,  // Set Device Bits Interrupt (SDBS):
    DSS = 1 << 2,   // DMA Setup FIS Interrupt (DSS):
    PSS = 1 << 1,   // PIO Setup FIS Interrupt (PSS):
    DHRS = 1 << 0   // Device to Host Register FIS Interrupt (DHRS):
};

#pragma pack(push, 1)
typedef struct tagFIS_REG_H2D
{
    // DWORD 0
    uint8_t fis_type; // FIS_TYPE_REG_H2D

    uint8_t pmport:4; // Port multiplier
    uint8_t rsv0:3;   // Reserved
    uint8_t c:1;      // 1: Command, 0: Control

    uint8_t command;  // Command register
    uint8_t featurel; // Feature register, 7:0

    // DWORD 1
    uint8_t lba0;   // LBA low register, 7:0
    uint8_t lba1;   // LBA mid register, 15:8
    uint8_t lba2;   // LBA high register, 23:16
    uint8_t device; // Device register

    // DWORD 2
    uint8_t lba3;     // LBA register, 31:24
    uint8_t lba4;     // LBA register, 39:32
    uint8_t lba5;     // LBA register, 47:40
    uint8_t featureh; // Feature register, 15:8

    // DWORD 3
    uint8_t countl;  // Count register, 7:0
    uint8_t counth;  // Count register, 15:8
    uint8_t icc;     // Isochronous command completion
    uint8_t control; // Control register

    // DWORD 4
    uint8_t rsv1[4]; // Reserved
} FIS_REG_H2D;

typedef struct tagFIS_REG_D2H
{
    // DWORD 0
    uint8_t fis_type; // FIS_TYPE_REG_D2H

    uint8_t pmport:4; // Port multiplier
    uint8_t rsv0:2;   // Reserved
    uint8_t i:1;      // Interrupt bit
    uint8_t rsv1:1;   // Reserved

    uint8_t status; // Status register
    uint8_t error;  // Error register

    // DWORD 1
    uint8_t lba0;   // LBA low register, 7:0
    uint8_t lba1;   // LBA mid register, 15:8
    uint8_t lba2;   // LBA high register, 23:16
    uint8_t device; // Device register

    // DWORD 2
    uint8_t lba3; // LBA register, 31:24
    uint8_t lba4; // LBA register, 39:32
    uint8_t lba5; // LBA register, 47:40
    uint8_t rsv2; // Reserved

    // DWORD 3
    uint8_t countl;  // Count register, 7:0
    uint8_t counth;  // Count register, 15:8
    uint8_t rsv3[2]; // Reserved

    // DWORD 4
    uint8_t rsv4[4]; // Reserved
} FIS_REG_D2H;

typedef struct tagFIS_DATA
{
    // DWORD 0
    uint8_t fis_type; // FIS_TYPE_DATA

    uint8_t pmport:4; // Port multiplier
    uint8_t rsv0:4;   // Reserved

    uint8_t rsv1[2]; // Reserved

    // DWORD 1 ~ N
    uint32_t data[1]; // Payload
} FIS_DATA;

typedef struct tagFIS_PIO_SETUP
{
    // DWORD 0
    uint8_t fis_type; // FIS_TYPE_PIO_SETUP

    uint8_t pmport:4; // Port multiplier
    uint8_t rsv0:1;   // Reserved
    uint8_t d:1;      // Data transfer direction, 1 - device to host
    uint8_t i:1;      // Interrupt bit
    uint8_t rsv1:1;

    uint8_t status; // Status register
    uint8_t error;  // Error register

    // DWORD 1
    uint8_t lba0;   // LBA low register, 7:0
    uint8_t lba1;   // LBA mid register, 15:8
    uint8_t lba2;   // LBA high register, 23:16
    uint8_t device; // Device register

    // DWORD 2
    uint8_t lba3; // LBA register, 31:24
    uint8_t lba4; // LBA register, 39:32
    uint8_t lba5; // LBA register, 47:40
    uint8_t rsv2; // Reserved

    // DWORD 3
    uint8_t countl;   // Count register, 7:0
    uint8_t counth;   // Count register, 15:8
    uint8_t rsv3;     // Reserved
    uint8_t e_status; // New value of status register

    // DWORD 4
    uint16_t tc;     // Transfer count
    uint8_t rsv4[2]; // Reserved
} FIS_PIO_SETUP;

struct FIS_DEV_BITS
{
    // DWORD 0
    uint8_t fis_type; // FIS_DEV_BITS

    uint8_t pmport:4; // Port multiplier
    uint8_t rsv0:1;   // Reserved
    uint8_t d:1;      // Data transfer direction, 1 - device to host
    uint8_t i:1;      // Interrupt bit
    uint8_t rsv1:1;

    uint8_t status; // Status register
    uint8_t error;  // Error register

    // DWORD 1
    uint8_t rsv2[4]; // Reserved
};

struct HBA_PORT
{
    uint32_t clb;  // 0x00, command list base address, 1K-byte aligned
    uint32_t clbu; // 0x04, command list base address upper 32 bits
    uint32_t fb;   // 0x08, FIS base address, 256-byte aligned
    uint32_t fbu;  // 0x0C, FIS base address upper 32 bits
    uint32_t is;   // 0x10, interrupt status
    uint32_t ie;   // 0x14, interrupt enable
    struct
    {
        uint8_t st:1;    //  start
        uint8_t sud:1;   //  spin up device
        uint8_t pod:1;   //  power on device
        uint8_t clo:1;   //  comand list override
        uint8_t fre:1;   //  FIS receive enable
        uint8_t resv0:3; //  reserved
        uint8_t ccs:5;   //  current command slot
        uint8_t mpss:1;  //  mechanical presance switch
        uint8_t fr:1;    //  FIS receive running
        uint8_t cr:1;    //  command list running
        uint8_t cps:1;   //  cold presence state
        uint8_t pma:1;   //  port multiplier attached
        uint8_t hpcp:1;  //  hot plug capable
        uint8_t mpsp:1;  //  Mechanical Presence Switch Attached to Port
        uint8_t cpd:1;   //  Cold Presence Detection
        uint8_t esp:1;   //  External SATA Port
        uint8_t fbscp:1; //  FIS-based Switching Capable Port
        uint8_t apste:1; //  Automatic Partial to Slumber Transitions Enabled
        uint8_t atapi:1; //  Device is ATAPI
        uint8_t dlae:1;  //  Drive LED on ATAPI Enable
        uint8_t alpe:1;  //  Aggressive Link Power Management Enable
        uint8_t asp:1;   //  Aggressive Slumber / Partial
        uint8_t icc:4;   //  Interface Communication Control
    } cmd;               // 0x18, command and status
    uint32_t rsv0;       // 0x1C, Reserved
    uint32_t tfd;        // 0x20, task file data
    uint32_t sig;        // 0x24, signature
    struct
    {
        uint8_t det:4;
        uint8_t spd:4;
        uint8_t ipm:4;
        uint32_t resv:20;
    } ssts; // 0x28, SATA status (SCR0:SStatus)
    struct
    {
        uint8_t det:4;   // Device Detection Initialization (DET)
        uint8_t spd:4;   // Speed Allowed (SPD)
        uint8_t ipm:4;   // Interface Power Management Transitions Allowed (IPM)
        uint8_t spm:4;   // Select Power Management (SPM): This field is not used by AHCI
        uint8_t pmp:4;   // Port Multiplier Port (PMP): This field is not used by AHCI.
        uint16_t rsv:12; // Reserved

    } sctl;             // 0x2C, SATA control (SCR2:SControl)
    uint32_t serr;      // 0x30, SATA error (SCR1:SError)
    uint32_t sact;      // 0x34, SATA active (SCR3:SActive)
    uint32_t ci;        // 0x38, command issue
    uint32_t sntf;      // 0x3C, SATA notification (SCR4:SNotification)
    uint32_t fbs;       // 0x40, FIS-based switch control
    uint32_t rsv1[11];  // 0x44 ~ 0x6F, Reserved
    uint32_t vendor[4]; // 0x70 ~ 0x7F, vendor specific
};

enum HBA_GHC
{
    HR = 1,
    IE = 2
};

struct HBA_MEM
{
    // 0x00 - 0x2B, Generic Host Control
    uint32_t cap;     // 0x00, Host capability
    uint32_t ghc;     // 0x04, Global host control
    uint32_t is;      // 0x08, Interrupt status
    uint32_t pi;      // 0x0C, Port implemented
    uint32_t vs;      // 0x10, Version
    uint32_t ccc_ctl; // 0x14, Command completion coalescing control
    uint32_t ccc_pts; // 0x18, Command completion coalescing ports
    uint32_t em_loc;  // 0x1C, Enclosure management location
    uint32_t em_ctl;  // 0x20, Enclosure management control
    uint32_t cap2;    // 0x24, Host capabilities extended
    uint32_t bohc;    // 0x28, BIOS/OS handoff control and status

    // 0x2C - 0x9F, Reserved
    uint8_t rsv[0xA0 - 0x2C];

    // 0xA0 - 0xFF, Vendor specific registers
    uint8_t vendor[0x100 - 0xA0];

    // 0x100 - 0x10FF, Port control registers
    HBA_PORT ports[32]; // 1 ~ 32
};

struct HBA_CMD_HEADER
{
    // DW0
    uint8_t cfl:5; // Command FIS length in DWORDS, 2 ~ 16
    uint8_t a:1;   // ATAPI
    uint8_t w:1;   // Write, 1: H2D, 0: D2H
    uint8_t p:1;   // Prefetchable

    uint8_t r:1;    // Reset
    uint8_t b:1;    // BIST
    uint8_t c:1;    // Clear busy upon R_OK
    uint8_t rsv0:1; // Reserved
    uint8_t pmp:4;  // Port multiplier port

    uint16_t prdtl; // Physical region descriptor table length in entries

    // DW1
    volatile uint32_t prdbc; // Physical region descriptor byte count transferred

    // DW2, 3
    uint32_t ctba;  // Command table descriptor base address
    uint32_t ctbau; // Command table descriptor base address upper 32 bits

    // DW4 - 7
    uint32_t rsv1[4]; // Reserved
};

struct FIS_DMA_SETUP
{
    // DWORD 0
    uint8_t fis_type; // FIS_TYPE_DMA_SETUP

    uint8_t pmport:4; // Port multiplier
    uint8_t rsv0:1;   // Reserved
    uint8_t d:1;      // Data transfer direction, 1 - device to host
    uint8_t i:1;      // Interrupt bit
    uint8_t a:1;      // Auto-activate. Specifies if DMA Activate FIS is needed

    uint8_t rsved[2]; // Reserved

    // DWORD 1&2

    uint64_t DMAbufferID; // DMA Buffer Identifier. Used to Identify DMA buffer in host memory. SATA Spec says host
                          // specific and not in Spec. Trying AHCI spec might work.

    // DWORD 3
    uint32_t rsvd; // More reserved

    // DWORD 4
    uint32_t DMAbufOffset; // Byte offset into buffer. First 2 bits must be 0

    // DWORD 5
    uint32_t TransferCount; // Number of bytes to transfer. Bit 0 must be 0

    // DWORD 6
    uint32_t resvd; // Reserved
};

struct HBA_FIS
{
    // 0x00
    FIS_DMA_SETUP dsfis; // DMA Setup FIS
    uint8_t pad0[4];

    // 0x20
    FIS_PIO_SETUP psfis; // PIO Setup FIS
    uint8_t pad1[12];

    // 0x40
    FIS_REG_D2H rfis; // Register – Device to Host FIS
    uint8_t pad2[4];

    // 0x58
    FIS_DEV_BITS sdbfis; // Set Device Bit FIS

    // 0x60
    uint8_t ufis[64];

    // 0xA0
    uint8_t rsv[0x100 - 0xA0];
};

struct HBA_PRDT_ENTRY
{
    uint32_t dba;  // Data base address
    uint32_t dbau; // Data base address upper 32 bits
    uint32_t rsv0; // Reserved

    // DW3
    uint32_t dbc:22; // Byte count, 4M max
    uint32_t rsv1:9; // Reserved
    uint32_t i:1;    // Interrupt on completion
};

struct HBA_CMD_TBL
{
    // 0x00
    uint8_t cfis[64]; // Command FIS

    // 0x40
    uint8_t acmd[16]; // ATAPI command, 12 or 16 bytes

    // 0x50
    uint8_t rsv[48]; // Reserved

    // 0x80
    HBA_PRDT_ENTRY prdt_entry[1]; // Physical region descriptor table entries, 0 ~ 65535
};

#pragma pack(pop)