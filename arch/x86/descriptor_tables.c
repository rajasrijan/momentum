/*
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

#include "descriptor_tables.h"

gdt_t *gt;
idt_t *it;

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
