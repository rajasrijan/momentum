/*
 * Copyright 2009-2020 Srijan Kumar Sharma
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
#define ACPI_H

extern "C"
{
#include <acpi.h>
#include <platform/acenv.h>
#include <actypes.h>
#include <actbl.h>
#include <aclocal.h>
#include <acexcep.h>
#include <acobject.h>
#include <acstruct.h>
#include <acnamesp.h>
#include <acresrc.h>
}

#ifdef __cplusplus
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
        uint32_t lapicAddr;
        uint32_t flags;
        struct madt_entry_structure apic_entries;
    } __attribute__((packed));

    struct ioapic_info
    {
        struct madt_entry_structure header;
        uint8_t apicId;
        uint8_t resv;
        uint32_t ioapicAddr;
        uint32_t GlobalSystemInterruptBase;

    } __attribute__((packed));

    struct interrupt_override
    {
        struct madt_entry_structure header;
        uint8_t bus;
        uint8_t source;
        uint32_t global_system_interrupt;
        uint16_t flags;

    } __attribute__((packed));

#define ACPI_RSDP_SIGNATURE ("RSD PTR ")
#define ACPI_DESCRIPTION_HEADER_MADT (0x43495041) // APIC
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

    extern void *apic_base;
    void parse_acpi(void);
    void fix_refferances(void);
    uint8_t get_acpi_tables(void);
    int InitializeFullAcpi(void);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#include <vector>

std::vector<struct pci_routing> &get_pci_routing_table();

namespace Acpi
{

inline ACPI_STATUS Evaluate(const char *method_name, [[maybe_unused]] ACPI_BUFFER ReturnValue)
{
    ACPI_STATUS status = AE_OK;
    ACPI_EVALUATE_INFO info = {};
    info.RelativePathname = method_name;
    status = AcpiNsEvaluate(&info);
    return status;
}
} // namespace Acpi
#endif

#endif /* ACPI_H */
