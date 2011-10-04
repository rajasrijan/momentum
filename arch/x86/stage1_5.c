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

#include <stdint.h>
#include <string.h>
#include "global.h"
#include "multiboot.h"
#include "paging.h"
#include "acpi.h"
#include "descriptor_tables.h"
#include "mm.h"

extern uint32_t kernel_end;
extern uint32_t kernel_start;
void stage1_5(void* mbd, unsigned int magic);
uint8_t kmemcmp(const char* src, const char* dst, uint32_t size);
void kmemcpy(uint8_t* src, uint8_t* dst, uint32_t size);
void default_paging(void);
void init_gdt(void);
static void init_idt(void);
static void gdt_set_gate(gdt_entry_t *gdt_entries, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);
static void idt_set_gate(idt_entry_t *idt_entries, uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
extern void stage2(void);

__attribute__((section(".sdata"))) uint32_t kend;
__attribute__((section(".sdata"))) paging_structure_t *ps;
__attribute__((section(".sdata"))) gdt_t *gdt;
__attribute__((section(".sdata"))) idt_t *idt;
__attribute__((section(".sdata"))) uint32_t kernel_length;
__attribute__((section(".sdata"))) void* memory_map;
__attribute__((section(".sdata"))) uint32_t memory_map_len;

__attribute__((section(".stext"))) void stage1_5(void* mbd, unsigned int magic)
{
    /*
     * First deal with multiboot.Its located just after the kernel in GRUB2.
     */
    kend = ((uint32_t) (&kernel_end) - 0xC0000000);
    coax_multiboot(mbd);
    parse_acpi();
    init_gdt();
    init_idt();
    default_paging();
    gt=gdt;
    stage2();
}

__attribute__((section(".stext"))) void coax_multiboot(void *mbd)
{
    multiboot_info_t mbi = *(multiboot_info_t*) mbd;

    if (mbi.flags & MULTIBOOT_INFO_MEM_MAP == MULTIBOOT_INFO_MEM_MAP)
    {
        memory_map_len = mbi.mmap_length;
        memory_map = _kalloc(mbi.mmap_length, 1);
        kmemcpy((void*) (mbi.mmap_addr), memory_map, mbi.mmap_length);
    }
    else
    {
        //print("\nERROR:Memory map not returned by GRUB.");
        __asm__("cli");
        __asm__("hlt");
    }
}

__attribute__((section(".stext"))) void* _kalloc(uint32_t size, uint32_t align)
{
    if (align == 0)
        align = 1;
    kend = ((((kend + align - 1) / align) * align) + size);
    return (void*) (kend - size);
}

__attribute__((section(".stext"))) void default_paging(void)
{
    ps = _kalloc(sizeof (paging_structure_t), 0x1000);
    kernel_length = kend;
    if (kernel_length >= 0xC0000000) //precaution
        kernel_length -= 0xC0000000;
    kernel_length = ((kernel_length + 0x3FFFFF)&0xFFC00000);



    for (int32_t i = 0; i < 1024; i++)
    {
        ps->page_directory[i] = (uint32_t) (&(ps->page_table[i * 1024]));
    }
    for (uint32_t i = 0, j = 0; i < (kernel_length); i += 0x1000, j++)
    {
        /*
         * Identity map lower memory
         */
        ps->page_table[j] = i | 3;
        /*
         * Map higher memory.
         */
        ps->page_table[j + 0xC0000] = i | 3;
        if ((j % 1024) == 0)
        {
            ps->page_directory[(j / 1024)] |= 3;
            ps->page_directory[(j / 1024) + 0x300] |= 3;
        }
    }




    //
    //	Start Paging.
    //
    __asm__ volatile("mov %0, %%cr3"::"b"(ps->page_directory));
    uint32_t cr0;
    __asm__ volatile("mov %%cr0, %0" : "=b"(cr0));
    cr0 |= 0x80000000;
    __asm__ volatile("mov %0, %%cr0"::"b"(cr0));
}

__attribute__((section(".stext"))) uint8_t kmemcmp(const char* src, const char* dst, uint32_t size)
{
    int i = 0;
    while (i < size)
    {
        if ((dst[i] - src[i]) != 0)
        {
            return (uint8_t) (dst[i] - src[i]);
        }
        i++;
    }
    return 0;
}

__attribute__((section(".stext"))) void kmemcpy(uint8_t* src, uint8_t* dst, uint32_t size)
{
    for (int i = 0; i < size; i++)
        dst[i] = src[i];
}

__attribute__((section(".stext"))) void init_gdt()
{
    gdt = (void*) _kalloc(sizeof (gdt_t), 4);
    gdt[0].gp.base = (uint32_t) (gdt[0].ge);
    gdt[0].gp.limit = (GDT_COUNT * sizeof (gdt_entry_t)) - 1;
    gdt_set_gate(&gdt[0].ge[0], 0, 0, 0, 0); // Null segment
    gdt_set_gate(&gdt[0].ge[1], 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    gdt_set_gate(&gdt[0].ge[2], 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
    gdt_set_gate(&gdt[0].ge[3], 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
    gdt_set_gate(&gdt[0].ge[4], 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment
    gdt_set_gate(&gdt[0].ge[5], 0, 0xFFFFFFFF, 0xF2, 0xCF); // Thread information block
    __asm__ volatile (
            "lgdt       (%0)\n"
            "mov        $0x10,%%ax\n"
            "mov        %%ax,%%ds\n"
            "mov        %%ax,%%es\n"
            "mov        %%ax,%%gs\n"
            "mov        %%ax,%%fs\n"
            "mov        %%ax,%%ss\n"
            "ljmp        $0x8,$farjmp\n"
            "farjmp:    \n"
            "nop        \n"
            :
            : "b"(&gdt[0].gp));
}

__attribute__((section(".stext"))) static void init_idt(void)
{
    idt = (void*) _kalloc(sizeof (idt_t), 4);
    idt[0].ip.limit = (sizeof (idt_entry_t) * 256) - 1;
    idt[0].ip.base = (uint32_t) idt[0].ie;
    idt_set_gate(idt[0].ie, 0, (uint32_t) isr0, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 1, (uint32_t) isr1, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 2, (uint32_t) isr2, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 3, (uint32_t) isr3, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 4, (uint32_t) isr4, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 5, (uint32_t) isr5, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 6, (uint32_t) isr6, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 7, (uint32_t) isr7, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 8, (uint32_t) isr8, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 9, (uint32_t) isr9, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 10, (uint32_t) isr10, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 11, (uint32_t) isr11, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 12, (uint32_t) isr12, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 13, (uint32_t) isr13, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 14, (uint32_t) isr14, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 15, (uint32_t) isr15, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 16, (uint32_t) isr16, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 17, (uint32_t) isr17, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 18, (uint32_t) isr18, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 19, (uint32_t) isr19, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 20, (uint32_t) isr20, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 21, (uint32_t) isr21, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 22, (uint32_t) isr22, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 23, (uint32_t) isr23, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 24, (uint32_t) isr24, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 25, (uint32_t) isr25, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 26, (uint32_t) isr26, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 27, (uint32_t) isr27, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 28, (uint32_t) isr28, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 29, (uint32_t) isr29, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 30, (uint32_t) isr30, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 31, (uint32_t) isr31, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 32, (uint32_t) isr32, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 33, (uint32_t) isr33, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 34, (uint32_t) isr34, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 35, (uint32_t) isr35, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 36, (uint32_t) isr36, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 37, (uint32_t) isr37, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 38, (uint32_t) isr38, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 39, (uint32_t) isr39, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 40, (uint32_t) isr40, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 41, (uint32_t) isr41, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 42, (uint32_t) isr42, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 43, (uint32_t) isr43, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 44, (uint32_t) isr44, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 45, (uint32_t) isr45, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 46, (uint32_t) isr46, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 47, (uint32_t) isr47, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 48, (uint32_t) isr48, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 49, (uint32_t) isr49, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 50, (uint32_t) isr50, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 51, (uint32_t) isr51, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 52, (uint32_t) isr52, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 53, (uint32_t) isr53, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 54, (uint32_t) isr54, 0x08, 0x8E);
    idt_set_gate(idt[0].ie, 55, (uint32_t) isr55, 0x08, 0x8E);
    for (uint32_t i = 56; i < 256; i++)
    {
        idt_set_gate(idt[0].ie, (uint8_t) i, (uint32_t) isr64, 0x08, 0x8E);
    }
    __asm__ volatile (
            "lidt       (%0)\n"
            "nop        \n"
            :
            : "b"(&idt[0].ip));
}

__attribute__((section(".stext"))) static void gdt_set_gate(gdt_entry_t *gdt_entries, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    gdt_entries->base_low = (base & 0xFFFF);
    gdt_entries->base_middle = (base >> 16) & 0xFF;
    gdt_entries->base_high = (unsigned char) ((base >> 24) & 0xFF);

    gdt_entries->limit_low = (limit & 0xFFFF);
    gdt_entries->granularity = (limit >> 16) & 0x0F;

    gdt_entries->granularity = (uint8_t) (gdt_entries->granularity | (gran & 0xF0));
    gdt_entries->access = access;
}

__attribute__((section(".stext"))) static void idt_set_gate(idt_entry_t *idt_entries, uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
    idt_entries[num].base_lo = base & 0xFFFF;
    idt_entries[num].base_hi = (uint16_t) ((base >> 16) & 0xFFFF);

    idt_entries[num].sel = sel;
    idt_entries[num].always0 = 0;
    // We must uncomment the OR below when we get to using user-mode.
    // It sets the interrupt gate's privilege level to 3.
    idt_entries[num].flags = flags /* | 0x60 */;
}

