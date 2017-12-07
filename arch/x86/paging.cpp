/*
 * Copyright 2009-2012 Srijan Kumar Sharma
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

#include "paging.h"
#include "mm.h"
#include "interrupts.h"
#include "global.h"
#include "../../libc/string.h"
#include "../../libc/stdlib.h"

void new_paging_structure(paging_structure_t* ps)
{

}

void map_4mb(uint32_t virtual_address)
{
     
    uint32_t boundry_4mb = (virtual_address / 0x400000);
     
    uint32_t boundry_4kb = boundry_4mb * 0x400;
     
    uint32_t memory_slab = get_4mb_block();
     
    if (memory_slab == 0)
        __asm__("cli;hlt;");
     
    __asm__("xchg %bx,%bx;");
     
    sys_info.pst->page_directory[boundry_4mb] = (uint32_t)&(sys_info.pst->page_table[boundry_4kb]) | 3;
     
    for (uint32_t i = 0; i < 0x400; i++)
    {
        sys_info.pst->page_table[boundry_4kb + i] = memory_slab | 3;
        memory_slab += 0x1000;
        __asm__ volatile("invlpg %0"::"m" (*(char *) (memory_slab)));
    }
     
}

static void page_fault_handler_4kpages(registers_t* regs)
{
    printf("\nError code: 0x%x", regs->err_esp);
    uint32_t cr2;
    __asm__ volatile("mov %%cr2, %0" : "=b"(cr2));
    printf("\npage fault at address: 0x%x", cr2);
    if (IsMemoryReserved(cr2))
    {
        printf("\nMemmory is reserved.");
        identity_map_4mb(cr2);
    }
    else
        map_4mb(cr2);
}

void init_paging()
{
    /*
     * stage 1 old.
     */
    //pst = (void*) ((uint32_t) ps + 0xC0000000);
    register_interrupt_handler(0x0E, page_fault_handler_4kpages);
}

void identity_map_4mb(uint32_t address)
{
    uint32_t boundry_4mb = (address / 0x400000);
    uint32_t boundry_4kb = boundry_4mb * 0x400;
#warning "wtf is this"
    /*
     * wtf is this
     * 'uint32_t memory_slab = address & 0xFFC00000;'
     */
    uint32_t memory_slab = address & 0xFFC00000;
    sys_info.pst->page_directory[boundry_4mb] = (uint32_t)&(sys_info.pst->page_table[boundry_4kb]) | 3;
    for (uint32_t i = 0; i < 0x400; i++)
    {
        sys_info.pst->page_table[boundry_4kb + i] = memory_slab | 3;
        memory_slab += 0x1000;
        __asm__ volatile("invlpg %0"::"m" (*(char *) (memory_slab)));
    }
}

void force_map(uint32_t physical, uint32_t virtual_address, uint32_t pages)
{
    uint32_t boundry_4mb = (virtual_address / 0x400000);
    uint32_t boundry_4kb = boundry_4mb * 0x400;
    uint32_t memory_slab = physical & 0xFFFFF000;
    sys_info.pst->page_directory[boundry_4mb] = (uint32_t)&(sys_info.pst->page_table[boundry_4kb]) | 3;
    for (uint32_t i = 0; i < pages; i++)
    {
        sys_info.pst->page_table[(virtual_address / 0x1000) + i] = memory_slab | 3;
        memory_slab += 0x1000;
        __asm__ volatile("invlpg %0"::"m" (*(char *) (memory_slab)));
    }
}

uint32_t get_physical_address(uint32_t virtual_address)
{
    return ((sys_info.pst->page_table[(virtual_address / 0x1000)] & 0xFFFFF000) + (virtual_address & 0x00000FFF));
}