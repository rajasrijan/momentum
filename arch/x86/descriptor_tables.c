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
