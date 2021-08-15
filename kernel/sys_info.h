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

#include <arch/x86_64/descriptor_tables.h>
#include <stdint.h>
#include <uuid.h>

#pragma pack(push, 1)
typedef struct system_info
{
    uint32_t no_of_cpus;
    gdt_t gdt;
    idt_t idt;
    void *memory_map;
    uint32_t memory_map_len;
    uint32_t kernel_end;
    struct local_apic_structure *local_apic;
    struct acpi_rsdp *rsdp;
} system_info_t;
#pragma pack(pop)

extern system_info_t sys_info;
