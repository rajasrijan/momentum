#ifndef MM_H_COMMON
#define MM_H_COMMON

#ifdef _arch_x86_64_
#include <arch/x86_64/mm.h>
#endif

#ifdef _arch_efi_
#include <arch/efi/mm.h>
#endif

struct physical_memory_map
{
    uint64_t addr; //  physical address
    uint64_t len;  //  number of pages
};

int allocate_physical_pages(uint64_t &addr, size_t no_pages);
int free_physical_pages(uint64_t addr, size_t no_pages);

#endif // MM_H_COMMON