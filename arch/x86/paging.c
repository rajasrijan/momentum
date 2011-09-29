#include "paging.h"
#include "mm.h"
#include "interrupts.h"
#include "global.h"
static paging_structure_t *pst;

void new_paging_structure(paging_structure_t* ps)
{

}

void map_4mb(uint32_t virtual_address)
{
    uint32_t boundry_4mb = (virtual_address / 0x400000);
    uint32_t boundry_4kb = boundry_4mb * 0x400;
    uint32_t memory_slab = get_4mb_block();
    pst->page_directory[boundry_4mb] |= 3;
    for (uint32_t i = 0; i < 0x400; i++)
    {
        pst->page_table[boundry_4kb + i] = memory_slab | 3;
        memory_slab += 0x1000;
    }
}

static void page_fault_handler_4kpages(registers_t* regs)
{
    printf("\nError code: 0x%x", regs->err_code);
    uint32_t cr2;
    __asm__ volatile("mov %%cr2, %0" : "=b"(cr2));
    printf("\npage fault at address: 0x%x", cr2);
    if (IsMemoryReserved(cr2))
    {
        printf("\nMemmory is reserved.");
        identity_map_4mb(cr2);
        return;
    }
    map_4mb(cr2);
}

extern paging_structure_t *ps;

void init_paging()
{
    pst = (void*) ((uint32_t) ps + 0xC0000000);
    register_interrupt_handler(0x0E, page_fault_handler_4kpages);
}

void identity_map_4mb(uint32_t address)
{
    uint32_t boundry_4mb = (address / 0x400000);
    uint32_t boundry_4kb = boundry_4mb * 0x400;
    uint32_t memory_slab = address & 0xFFC00000;
    pst->page_directory[boundry_4mb] |= 3;
    for (uint32_t i = 0; i < 0x400; i++)
    {
        pst->page_table[boundry_4kb + i] = memory_slab | 3;
        memory_slab += 0x1000;
    }
}

void force_map(uint32_t physical, uint32_t virtual, uint32_t pages)
{
    uint32_t boundry_4mb = (virtual / 0x400000);
    uint32_t boundry_4kb = boundry_4mb * 0x400;
    uint32_t memory_slab = physical & 0xFFC00000;
    pst->page_directory[boundry_4mb] |= 3;
    for (uint32_t i = 0; i < pages; i++)
    {
        pst->page_table[boundry_4kb + i] = memory_slab | 3;
        memory_slab += 0x1000;
    }
}
