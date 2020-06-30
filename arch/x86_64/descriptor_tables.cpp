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

#include "global.h"
#include "descriptor_tables.h"
#include <kernel/sys_info.h>

void set_gdt_gate(gdt_entry_t *gdt_entries, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    gdt_entries->base_low = (base & 0xFFFF);
    gdt_entries->base_middle = (base >> 16) & 0xFF;
    gdt_entries->base_high = (unsigned char)((base >> 24) & 0xFF);

    gdt_entries->limit_low = (limit & 0xFFFF);
    gdt_entries->granularity = (limit >> 16) & 0x0F;

    gdt_entries->granularity = (uint8_t)(gdt_entries->granularity | (gran & 0xF0));
    gdt_entries->access = access;
}

void idt_set_gate(idt_entry_t *idt_entries, uint8_t num, uint64_t base, uint16_t sel, uint8_t flags)
{
    idt_entries[num].base_lo_lo = base & 0xFFFF;
    idt_entries[num].base_lo_hi = (uint16_t)((base >> 16) & 0xFFFF);
    idt_entries[num].base_hi = (uint32_t)((base >> 32) & 0xFFFFFFFF);

    idt_entries[num].sel = sel;
    idt_entries[num].ist = 0;
    // We must uncomment the OR below when we get to using user-mode.
    // It sets the interrupt gate's privilege level to 3.
    idt_entries[num].flags = flags /* | 0x60 */;
}

int init_idt(void)
{
    sys_info.idt.ip.limit = (sizeof(idt_entry_t) * 256) - 1;
    sys_info.idt.ip.base = (uint64_t)sys_info.idt.ie;
    idt_set_gate(sys_info.idt.ie, 0, (uint64_t)isr0, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 1, (uint64_t)isr1, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 2, (uint64_t)isr2, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 3, (uint64_t)isr3, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 4, (uint64_t)isr4, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 5, (uint64_t)isr5, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 6, (uint64_t)isr6, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 7, (uint64_t)isr7, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 8, (uint64_t)isr8, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 9, (uint64_t)isr9, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 10, (uint64_t)isr10, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 11, (uint64_t)isr11, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 12, (uint64_t)isr12, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 13, (uint64_t)isr13, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 14, (uint64_t)isr14, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 15, (uint64_t)isr15, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 16, (uint64_t)isr16, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 17, (uint64_t)isr17, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 18, (uint64_t)isr18, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 19, (uint64_t)isr19, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 20, (uint64_t)isr20, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 21, (uint64_t)isr21, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 22, (uint64_t)isr22, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 23, (uint64_t)isr23, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 24, (uint64_t)isr24, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 25, (uint64_t)isr25, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 26, (uint64_t)isr26, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 27, (uint64_t)isr27, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 28, (uint64_t)isr28, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 29, (uint64_t)isr29, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 30, (uint64_t)isr30, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 31, (uint64_t)isr31, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 32, (uint64_t)isr32, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 33, (uint64_t)isr33, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 34, (uint64_t)isr34, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 35, (uint64_t)isr35, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 36, (uint64_t)isr36, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 37, (uint64_t)isr37, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 38, (uint64_t)isr38, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 39, (uint64_t)isr39, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 40, (uint64_t)isr40, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 41, (uint64_t)isr41, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 42, (uint64_t)isr42, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 43, (uint64_t)isr43, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 44, (uint64_t)isr44, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 45, (uint64_t)isr45, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 46, (uint64_t)isr46, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 47, (uint64_t)isr47, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 48, (uint64_t)isr48, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 49, (uint64_t)isr49, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 50, (uint64_t)isr50, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 51, (uint64_t)isr51, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 52, (uint64_t)isr52, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 53, (uint64_t)isr53, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 54, (uint64_t)isr54, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 55, (uint64_t)isr55, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 56, (uint64_t)isr56, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 57, (uint64_t)isr57, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 58, (uint64_t)isr58, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 59, (uint64_t)isr59, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 60, (uint64_t)isr60, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 61, (uint64_t)isr61, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 62, (uint64_t)isr62, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 63, (uint64_t)isr63, 0x08, 0x8E | 0x60);
    idt_set_gate(sys_info.idt.ie, 64, (uint64_t)isr64, 0x08, 0x8E | 0x60);
    for (uint64_t i = 65; i < 256; i++)
    {
        idt_set_gate(sys_info.idt.ie, (uint8_t)i, (uint64_t)isr65, 0x08, 0x8E);
    }
    load_interrupt_descriptor_table(&(sys_info.idt.ip));
    return 0;
}