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

#include "global.h"
#include "descriptor_tables.h"

void set_gdt_gate(gdt_entry_t *gdt_entries, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    gdt_entries->base_low = (base & 0xFFFF);
    gdt_entries->base_middle = (base >> 16) & 0xFF;
    gdt_entries->base_high = (unsigned char) ((base >> 24) & 0xFF);

    gdt_entries->limit_low = (limit & 0xFFFF);
    gdt_entries->granularity = (limit >> 16) & 0x0F;

    gdt_entries->granularity = (uint8_t) (gdt_entries->granularity | (gran & 0xF0));
    gdt_entries->access = access;
}

void idt_set_gate(idt_entry_t *idt_entries, uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
    idt_entries[num].base_lo = base & 0xFFFF;
    idt_entries[num].base_hi = (uint16_t) ((base >> 16) & 0xFFFF);

    idt_entries[num].sel = sel;
    idt_entries[num].always0 = 0;
    // We must uncomment the OR below when we get to using user-mode.
    // It sets the interrupt gate's privilege level to 3.
    idt_entries[num].flags = flags /* | 0x60 */;
}

void init_idt(void)
{
    sys_info.idt_ptr[0].ip.limit = (sizeof (idt_entry_t) * 256) - 1;
    sys_info.idt_ptr[0].ip.base = (uint32_t) sys_info.idt_ptr[0].ie;
    idt_set_gate(sys_info.idt_ptr[0].ie, 0, (uint32_t) isr0, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 1, (uint32_t) isr1, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 2, (uint32_t) isr2, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 3, (uint32_t) isr3, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 4, (uint32_t) isr4, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 5, (uint32_t) isr5, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 6, (uint32_t) isr6, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 7, (uint32_t) isr7, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 8, (uint32_t) isr8, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 9, (uint32_t) isr9, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 10, (uint32_t) isr10, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 11, (uint32_t) isr11, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 12, (uint32_t) isr12, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 13, (uint32_t) isr13, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 14, (uint32_t) isr14, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 15, (uint32_t) isr15, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 16, (uint32_t) isr16, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 17, (uint32_t) isr17, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 18, (uint32_t) isr18, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 19, (uint32_t) isr19, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 20, (uint32_t) isr20, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 21, (uint32_t) isr21, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 22, (uint32_t) isr22, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 23, (uint32_t) isr23, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 24, (uint32_t) isr24, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 25, (uint32_t) isr25, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 26, (uint32_t) isr26, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 27, (uint32_t) isr27, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 28, (uint32_t) isr28, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 29, (uint32_t) isr29, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 30, (uint32_t) isr30, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 31, (uint32_t) isr31, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 32, (uint32_t) isr32, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 33, (uint32_t) isr33, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 34, (uint32_t) isr34, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 35, (uint32_t) isr35, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 36, (uint32_t) isr36, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 37, (uint32_t) isr37, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 38, (uint32_t) isr38, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 39, (uint32_t) isr39, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 40, (uint32_t) isr40, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 41, (uint32_t) isr41, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 42, (uint32_t) isr42, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 43, (uint32_t) isr43, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 44, (uint32_t) isr44, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 45, (uint32_t) isr45, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 46, (uint32_t) isr46, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 47, (uint32_t) isr47, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 48, (uint32_t) isr48, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 49, (uint32_t) isr49, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 50, (uint32_t) isr50, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 51, (uint32_t) isr51, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 52, (uint32_t) isr52, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 53, (uint32_t) isr53, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 54, (uint32_t) isr54, 0x08, 0x8E);
    idt_set_gate(sys_info.idt_ptr[0].ie, 55, (uint32_t) isr55, 0x08, 0x8E);
    for (uint32_t i = 56; i < 256; i++)
    {
        idt_set_gate(sys_info.idt_ptr[0].ie, (uint8_t) i, (uint32_t) isr64, 0x08, 0x8E);
    }
    __asm__ volatile (
            "lidt       (%0)\n"
            "nop        \n"
            :
            : "b"(&sys_info.idt_ptr[0].ip));
}