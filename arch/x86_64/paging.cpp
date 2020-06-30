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

#include "paging.h"
#include "global.h"
#include "mm.h"
#include "stdlib.h"
#include "string.h"
#include <errno.h>
#include <kernel/config.h>
#include <stdint.h>
#include <kernel/bitmap_allocator.h>

uint64_t cpu_local_pml4t = 0;
bitmap_allocator<512, 4096> default_pagemem_allocator;

//  keep track of memory mappings in kernel early stages, before paging is correctly setup.
static MemPage early_kernel_mappings[3] = {};
size_t early_kernel_mappings_count = 0;

template <int level = 4>
int page_allocation_helper(page_struct *pd, uint64_t vaddr, uint64_t paddr, uint64_t length, PageManager::Privilege privilege, PageManager::PageType pageType)
{
    if (level <= 0)
    {
        //  never reach here;
        asm("cli;hlt");
        return 0;
    }

    uint64_t p = 0, v = 0;
    int index = PG_LVL_IDX(vaddr, level);
    const uint64_t PG_SIZE = PG_LVL_SZ(level);
    uint64_t vaddr_end = vaddr + length;
    uint64_t page_offset = PG_LVL_OFFSET(vaddr, level);

    while ((level != 4) && (((level == 1) && (vaddr_end > vaddr)) || ((page_offset <= (PG_SIZE / 2)) && (vaddr_end >= (vaddr + (PG_SIZE / 2))))))
    {
        paddr = PG_LVL_ALIGN(paddr, level);
        vaddr = PG_LVL_ALIGN(vaddr, level);
        pd[index].paddr = (paddr >> 12);
        pd[index].present = 1;
        if (pageType == PageManager::PageType::Read_Write)
            pd[index].rw = 1;
        if (level != 1)
            pd[index].ps = 1;
        if (privilege == PageManager::Privilege::User)
            pd[index].us = 1;
        asm volatile("invlpg (%0)" ::"r"(vaddr));
        index++;
        paddr += PG_SIZE;
        vaddr += PG_SIZE;
        page_offset = PG_LVL_OFFSET(vaddr, level);
    }

    if (vaddr_end <= vaddr || level == 1)
        return 0;

    if (pd[index].present == 0)
    {
        if (bitmap_alloc(default_pagemem_allocator, p, v))
        {
            printf("pagemem allocator cannot fail\n");
            asm("cli;hlt");
        }
        memset((void *)v, 0, 4096);
        pd[index].paddr = (p >> 12);
        pd[index].present = 1;
        pd[index].rw = 1;
        asm volatile("invlpg (%0)" ::"r"(vaddr));
    }
    else
    {
        p = pd[index].paddr << 12;
        v = (p - default_pagemem_allocator.paddr) + default_pagemem_allocator.vaddr;
    }
    return page_allocation_helper < (level > 0) ? level - 1 : 0 > ((page_struct *)v, vaddr, paddr, vaddr_end - vaddr, privilege, pageType);
}

int PageManager::initialize()
{
    //  get a 2 MB RAM frame
    default_pagemem_allocator.paddr = get_2mb_block();
    add_early_kernel_mapping(default_pagemem_allocator.paddr + KERNEL_BASE_PTR, default_pagemem_allocator.paddr, PAGESIZE);
    if (default_pagemem_allocator.paddr >= 0x40000000)
    {
        printf("paddr cant be greater than 1GB, not mapped yet.\n");
        asm("cli;hlt");
    }
    default_pagemem_allocator.vaddr = default_pagemem_allocator.paddr;
    if (bitmap_alloc(default_pagemem_allocator, cpu_local_pml4t, cpu_local_pml4t) < 0)
    {
        printf("Failed to setup memory for page directory!\n");
        asm("cli;hlt");
    }
    memset((void *)cpu_local_pml4t, 0, 4096);
    //  start mapping
    uint64_t vaddr = (uint64_t)&kernel_start & 0xFFFFFFFFFFFFF000;
    uint64_t paddr = vaddr - KERNEL_BASE_PTR;
    uint64_t length = (uint64_t)&kernel_end - vaddr;
    page_allocation_helper((page_struct *)cpu_local_pml4t, vaddr, paddr, length, Privilege::Supervisor, PageType::Read_Write);
    page_allocation_helper((page_struct *)cpu_local_pml4t, paddr, paddr, length, Privilege::Supervisor, PageType::Read_Write);
    for (size_t i = 0; i < early_kernel_mappings_count; i++)
    {
        page_allocation_helper((page_struct *)cpu_local_pml4t, early_kernel_mappings[i].vaddr, early_kernel_mappings[i].paddr, early_kernel_mappings[i].size, Privilege::Supervisor, PageType::Read_Write);
    }

    asm volatile(".intel_syntax noprefix;"
                 "mov cr3, rax;" ::"a"(cpu_local_pml4t));
    cpu_local_pml4t += KERNEL_BASE_PTR;
    default_pagemem_allocator.vaddr = default_pagemem_allocator.vaddr + KERNEL_BASE_PTR;
    return 0;
}

/**
 * Round up to page size
 *
 */
uint64_t PageManager::roundToPageSize(uint64_t sz)
{
    return ((sz + 0x1FFFFF) & 0xFFFFFFFFFFE00000);
}

/**
 * Round up to page boundry
 *
 */
uint64_t PageManager::roundToPageBoundry(uint64_t addr)
{
    return (addr & 0xFFFFFFFFFFE00000);
}

int PageManager::setPageAllocation(uint64_t vaddr, uint64_t size, Privilege privilege, PageType pageType)
{
    int ret = 0;
    for (size_t i = 0; i < size; i += PAGESIZE)
    {
        uint64_t memory_slab = get_2mb_block();
        printf("SLAB addr [%x]\n", memory_slab);
        ret = setVirtualToPhysicalMemory(vaddr + i, memory_slab, PAGESIZE, privilege, pageType);
        if (ret < 0)
            break;
    }
    if (ret < 0)
    {
        freeVirtualMemory(vaddr, size);
    }
    return ret;
}

int PageManager::setVirtualToPhysicalMemory(uint64_t vaddr, uint64_t paddr, uint64_t size, Privilege privilege, PageType pageType)
{
    int ret = 0;
    ret = page_allocation_helper((page_struct *)cpu_local_pml4t, vaddr, paddr, size, privilege, pageType);
    return ret;
}

template <int level = 4>
static int getphysicaladdress_helper(page_struct *pg, uint64_t vaddr, uint64_t &paddr)
{
    uint64_t l1 = PG_LVL_IDX(vaddr, level);
    if (pg[l1].present == 1)
    {
        if (level > 1 && pg[l1].ps == 0)
        {
            auto new_pg = (page_struct *)(PG_GET_ADDRESS(&pg[l1]) - default_pagemem_allocator.paddr + default_pagemem_allocator.vaddr);
            return getphysicaladdress_helper < (level > 1) ? level - 1 : 1 > (new_pg, vaddr, paddr);
        }
        else
        {
            paddr = PG_GET_ADDRESS(&pg[l1]) + PG_LVL_OFFSET(vaddr, level);
            return 0;
        }
    }
    else
    {
        return -ENOMEM;
    }
    return 0;
}

int PageManager::getPhysicalAddress(uint64_t vaddr, uint64_t &paddr)
{
    return getphysicaladdress_helper((page_struct *)cpu_local_pml4t, vaddr, paddr);
}

template <int level = 4>
static int freememory_helper(page_struct *pg, uint64_t vaddr, uint64_t sz)
{
    for (uint64_t l1 = PG_LVL_IDX(vaddr, level); l1 < (sz + PG_LVL_SZ(level) - 1) / PG_LVL_SZ(level); l1++)
    {
        if (pg[l1].present == 1)
        {
            auto new_pg = (page_struct *)(PG_GET_ADDRESS(&pg[l1]) - default_pagemem_allocator.paddr + default_pagemem_allocator.vaddr);
            if (level > 1 && pg[l1].ps == 0)
            {
                freememory_helper<(level > 1) ? level - 1 : 1>(new_pg, vaddr, sz);
            }
            else
            {
                new_pg->addr = 0;
            }
        }
    }
    return 0;
}

int PageManager::freeVirtualMemory(uint64_t vaddr, uint64_t size)
{
    return freememory_helper((page_struct *)cpu_local_pml4t, vaddr, size);
}

int PageManager::set2MBPage([[maybe_unused]] uint64_t vaddr, [[maybe_unused]] uint64_t paddr, [[maybe_unused]] Privilege privilege, [[maybe_unused]] PageType pageType)
{
    printf("NOT IMPLEMENTED!\n");
    asm("cli;hlt");
    return -ENOSYS;
}

int PageManager::IdentityMap2MBPages(uint64_t paddr)
{
    int ret = 0;
    ret = page_allocation_helper((page_struct *)cpu_local_pml4t, paddr, paddr, PAGESIZE, Privilege::Supervisor, PageType::Read_Write);
    return ret;
}

int PageManager::findVirtualMemory([[maybe_unused]] uint64_t paddr, [[maybe_unused]] uint64_t &vaddr)
{
    printf("NOT IMPLEMENTED!\n");
    asm("cli;hlt");
    return -ENOSYS;
}

template <int level = 4>
static int findfreememory_helper(page_struct *pg, uint64_t &vaddr, uint64_t sz, uint64_t offset, uint64_t &sz_alloc, uint64_t parent_vaddr = 0, bool restart_counting = true)
{
    auto base = std::max(parent_vaddr, offset);
    for (uint64_t l1 = PG_LVL_IDX(base, level); l1 < 512; l1++)
    {
        if (pg[l1].present == 1)
        {
            if (level > 1 && pg[l1].ps == 0)
            {
                auto new_pg = (page_struct *)(PG_GET_ADDRESS(&pg[l1]) - default_pagemem_allocator.paddr + default_pagemem_allocator.vaddr);
                restart_counting = findfreememory_helper < (level > 1) ? level - 1 : 1 > (new_pg, vaddr, sz, offset, sz_alloc, parent_vaddr + (l1 * PG_LVL_SZ(level)), restart_counting);
                if (sz_alloc >= sz)
                    return true;
            }
            else if (!restart_counting)
            {
                printf("vaddr:%#llx, size:%#llx\n", vaddr, sz_alloc);
                restart_counting = true;
            }
        }
        else if (pg[l1].present == 0)
        {
            if (restart_counting)
            {
                vaddr = parent_vaddr + (l1 * PG_LVL_SZ(level));
                sz_alloc = 0;
            }
            sz_alloc += PG_LVL_SZ(level);
            restart_counting = false;
            if (sz_alloc >= sz)
                return true;
        }
        else
        {
            printf("NOT IMPLIMENTED!\n");
            asm("cli;hlt");
        }
    }
    if (level == 4 && !restart_counting)
    {
        printf("vaddr:%#llx, size:%#llx\n", vaddr, sz_alloc);
        restart_counting = true;
    }
    return restart_counting;
}

int PageManager::findFreeVirtualMemory(uint64_t &vaddr, uint64_t sz, uint64_t offset)
{
    int ret = 0;
    uint64_t sz_alloc = 0;

    page_struct *pg = (page_struct *)cpu_local_pml4t;
    findfreememory_helper(pg, vaddr, sz, offset, sz_alloc);
    return ret;
}

int PageManager::getMemoryMap([[maybe_unused]] std::vector<MemPage> &memMap)
{
    printf("NOT IMPLEMENTED!\n");
    asm("cli;hlt");
    return -ENOSYS;
}

int PageManager::add_early_kernel_mapping(uint64_t vaddr, uint64_t paddr, uint64_t size)
{
    if (early_kernel_mappings_count >= (sizeof(early_kernel_mappings) / sizeof(early_kernel_mappings[0])))
    {
        printf("too many mappings\n");
        asm("cli;hlt");
    }
    early_kernel_mappings[early_kernel_mappings_count].vaddr = vaddr;
    early_kernel_mappings[early_kernel_mappings_count].paddr = paddr;
    early_kernel_mappings[early_kernel_mappings_count].size = size;
    early_kernel_mappings_count++;
    return 0;
}
