/*
 * Copyright 2009-2011 Srijan Kumar Sharma
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

#ifndef ACPI_H
#define	ACPI_H

#ifdef	__cplusplus
extern "C"
{
#endif
#include <stdint.h>
#include "apic.h"

struct acpi_description_header
{
    uint32_t Signature;
    uint32_t Length;
    uint8_t Revision;
    uint8_t Checksum;
    uint8_t OemId[6];
    uint64_t OemTableId;
    uint32_t OemRevision;
    uint32_t CreatorId;
    uint32_t CreatorRevision;
} __attribute__((packed));

struct acpi_rsdp
{
    uint8_t Signature[8];
    uint8_t Checksum;
    uint8_t OemId[6];
    uint8_t Revision;
    uint32_t RsdtAddress;
    uint32_t Length;
    uint64_t XsdtAddress;
    uint8_t ExChecksum;
    uint8_t resv0;
    uint8_t resv1;
    uint8_t resv2;
} __attribute__((packed));

struct acpi_rsdt
{
    struct acpi_description_header header;
} __attribute__((packed));

struct acpi_xsdt
{
    struct acpi_description_header header;
} __attribute__((packed));

struct interrupt_source_override_structure
{
    uint8_t type;
    uint8_t length;
    uint8_t bus;
    uint8_t source;
    uint32_t global_interrupt;
    uint16_t flags;
} __attribute__((packed));

struct madt_entry_structure
{
    uint8_t type;
    uint8_t length;
} __attribute__((packed));

struct madt_structure
{
    struct acpi_description_header header;
    lapic_t* lapic;
    uint32_t flags;
    struct madt_entry_structure apic_entries;
} __attribute__((packed));

typedef struct interrupt_override
{
    struct madt_entry_structure header;
    uint8_t bus;
    uint8_t source;
    uint32_t global_system_interrupt;
    uint16_t flags;

} __attribute__((packed)) interrupt_override_t;

#define ACPI_RSDP_SIGNATURE ("RSD PTR ")
#define ACPI_DESCRIPTION_HEADER_MADT (0x43495041) //APIC
#define ACPI_DESCRIPTION_HEADER_RSDT ("RSDT")

enum ACPI_MADT_TYPE
{
    LOCAL_APIC = 0,
    IO_APIC,
    INTERRUPT_OVERRIDE,
    NMI,
    LOCAL_APIC_NMI,
    LOCAL_APIC_ADDRESS_OVERRIDE,
    IO_SAPIC,
    LOCAL_SAPIC,
    PLATFORM_INTERRUPT_SOURCE,
    LOCAL_APICX2,
    LOCAL_APICX2_NMI,
};
typedef struct acpi_rsdp acpi_rsdp_t;
typedef struct acpi_rsdt acpi_rsdt_t;
typedef struct acpi_xsdt acpi_xsdt_t;
typedef struct acpi_description_header acpi_description_header_t;
typedef struct interrupt_source_override_structure interrupt_source_override_structure_t;
typedef struct madt_structure madt_structure_t;
typedef struct madt_entry_structure madt_entry_structure_t;


extern void* apic_base;
void parse_acpi(void);
void fix_refferances(void);
uint8_t get_acpi_tables(void);


#ifdef	__cplusplus
}
#endif

#endif	/* ACPI_H */

