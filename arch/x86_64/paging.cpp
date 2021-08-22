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

#include "paging.h"
#include "global.h"
#include "stdlib.h"
#include "string.h"
#include <arch/mm.h>
#include <errno.h>
#include <kernel/bitmap_allocator.h>
#include <kernel/config.h>
#include <kernel/logging.h>
#include <stdint.h>
#include <algorithm>

uint64_t cpu_local_pml4t = 0;
bitmap_allocator<512, 4096> default_pagemem_allocator;

//  keep track of memory mappings in kernel early stages, before paging is correctly setup.
static MemPage early_kernel_mappings[3] = {};
size_t early_kernel_mappings_count = 0;

template <int level = 4>
int page_allocation_helper(page_struct *pd, uint64_t vaddr, uint64_t paddr, uint64_t length, PageManager::Privilege privilege, PageManager::PageType pageType,
                           uint64_t &sz_alloc)
{
    int ret = 0;
    uint64_t dump = 0;
    for (size_t l1 = PG_LVL_IDX(vaddr + sz_alloc, level); l1 < 512 && sz_alloc < length; l1++)
    {
        page_struct *new_pg = nullptr;
        if ((level != 4) && ((level == 1) || (((length >= (PG_LVL_SZ(level) + sz_alloc)) && (PG_LVL_OFFSET(vaddr + sz_alloc, level) == 0)))))
        {
            pd[l1].paddr = (paddr + sz_alloc) >> 12;
            pd[l1].present = 1;
            if (pageType == PageManager::PageType::Read_Write)
                pd[l1].rw = 1;
            if (level != 1)
                pd[l1].ps = 1;
            if (privilege == PageManager::Privilege::User)
                pd[l1].us = 1;
            //  page got mapped, reduce length.
            sz_alloc += PG_LVL_SZ(level);
            asm volatile("invlpg (%0)" ::"r"(vaddr));
        }
        else
        {
            if (pd[l1].addr == 0)
            {
                ret = bitmap_alloc(default_pagemem_allocator, pd[l1].addr, dump);
                if (ret)
                {
                    log_error("pagemem allocator cannot fail\n");
                    asm("cli;hlt");
                }
                new_pg = (page_struct *)(PG_GET_ADDRESS(&pd[l1]) - default_pagemem_allocator.paddr + default_pagemem_allocator.vaddr);
                memset((void *)new_pg, 0, 0x1000);
            }
            pd[l1].present = 1;
            if (pageType == PageManager::PageType::Read_Write)
                pd[l1].rw = 1;
            if (privilege == PageManager::Privilege::User)
                pd[l1].us = 1;
            new_pg = (page_struct *)(PG_GET_ADDRESS(&pd[l1]) - default_pagemem_allocator.paddr + default_pagemem_allocator.vaddr);
            ret = page_allocation_helper < (level > 1) ? (level - 1) : 1 > (new_pg, vaddr, paddr, length, privilege, pageType, sz_alloc);
        }
    }
    return ret;
}

int PageManager::initialize()
{
    int ret = 0;
    //  get a 2 MB RAM frame
    ret = allocate_physical_pages(default_pagemem_allocator.paddr, 512);
    add_early_kernel_mapping(KERNEL_PAGE_DIRECTORY_VADDR_PTR, default_pagemem_allocator.paddr, PAGESIZE * 512);
    if (default_pagemem_allocator.paddr >= 0x40000000)
    {
        log_error("paddr cant be greater than 1GB, not mapped yet.\n");
        asm("cli;hlt");
    }
    default_pagemem_allocator.vaddr = default_pagemem_allocator.paddr;
    if (bitmap_alloc(default_pagemem_allocator, cpu_local_pml4t, cpu_local_pml4t) < 0)
    {
        log_error("Failed to setup memory for page directory!\n");
        asm("cli;hlt");
    }
    memset((void *)cpu_local_pml4t, 0, 4096);
    //  start mapping
    uint64_t vaddr = PageManager::round_down_to_pagesize((uint64_t)&kernel_start);
    uint64_t paddr = vaddr - KERNEL_BASE_VADDR_PTR + KERNEL_BASE_PADDR_PTR;
    uint64_t length = PageManager::round_up_to_pagesize((uint64_t)&kernel_end) - vaddr;
    setVirtualToPhysicalMemory(vaddr, paddr, length, Privilege::Supervisor, PageType::Read_Write);

    uint64_t stack_pointer = 0;
    asm volatile(".intel_syntax noprefix;"
                 "mov rbx, rsp;"
                 : "=b"(stack_pointer));
    stack_pointer = PageManager::round_up_to_pagesize(stack_pointer);
    stack_pointer -= (PAGESIZE * 4);

    setVirtualToPhysicalMemory(stack_pointer, stack_pointer, (PAGESIZE * 4), Privilege::Supervisor, PageType::Read_Write);
    for (size_t i = 0; i < early_kernel_mappings_count; i++)
    {
        setVirtualToPhysicalMemory(early_kernel_mappings[i].vaddr, early_kernel_mappings[i].paddr, early_kernel_mappings[i].size, Privilege::Supervisor,
                                   PageType::Read_Write);
    }

    asm volatile(".intel_syntax noprefix;"
                 "mov cr3, rbx;" ::"b"(cpu_local_pml4t));
    cpu_local_pml4t = KERNEL_PAGE_DIRECTORY_VADDR_PTR;
    default_pagemem_allocator.vaddr = KERNEL_PAGE_DIRECTORY_VADDR_PTR;
    return ret;
}

/**
 * Round up to page size
 *
 */
uint64_t PageManager::round_up_to_pagesize(uint64_t sz)
{
    return ((sz + PAGEMASK) & (~PAGEMASK));
}

/**
 * Round up to page boundry
 *
 */
uint64_t PageManager::round_down_to_pagesize(uint64_t addr)
{
    return (addr & (~PAGEMASK));
}

int PageManager::setPageAllocation(uint64_t vaddr, uint64_t size, Privilege privilege, PageType pageType)
{
    int ret = 0;
    uint64_t memory_slab = 0;
    ret = allocate_physical_pages(memory_slab, size / PAGESIZE);
    ret = setVirtualToPhysicalMemory(vaddr, memory_slab, size, privilege, pageType);
    return ret;
}

template <int lvl = 4>
int allocate_vaddr(page_struct *pg, uint64_t vaddr, uint64_t paddr, uint64_t size, PageManager::Privilege privilege, PageManager::PageType pageType)
{
    int ret = 0;
    auto vaddr_end = vaddr + size, vaddr_it = vaddr;
    while (vaddr_it < vaddr_end)
    {
        auto pg_idx = PG_LVL_IDX(vaddr_it, lvl);

        //  if the virtual address is aligned and size is greater than page size of this level, then allocate the whole page.
        if (((PG_LVL_OFFSET(vaddr_it, lvl) == 0) && (PG_LVL_OFFSET(paddr, lvl) == 0) && ((vaddr_end - vaddr_it) >= PG_LVL_SZ(lvl))) || (lvl == 1))
        {
            pg[pg_idx].addr = paddr;
            pg[pg_idx].present = 1;
            pg[pg_idx].rw = (pageType == PageManager::PageType::Read_Write) ? 1 : 0;
            pg[pg_idx].us = (privilege == PageManager::Privilege::User) ? 1 : 0;

            if (lvl > 1)
                pg[pg_idx].ps = 1;

            asm volatile("invlpg (%0)" ::"r"(vaddr_it));

            vaddr_it += PG_LVL_SZ(lvl);
            paddr += PG_LVL_SZ(lvl);
        }
        else
        {
            uint64_t addr = 0, dump;
            if (pg[pg_idx].addr == 0)
            {
                ret = bitmap_alloc(default_pagemem_allocator, addr, dump);
                if (ret)
                {
                    log_error("Failed to allocate page\n");
                    return ret;
                }
                pg[pg_idx].addr = addr;
                pg[pg_idx].present = 1;
                pg[pg_idx].rw = (pageType == PageManager::PageType::Read_Write) ? 1 : 0;
                pg[pg_idx].us = (privilege == PageManager::Privilege::User) ? 1 : 0;

                bitmap_get_vaddr(default_pagemem_allocator, addr, addr);
                memset((void *)addr, 0, PageManager::PAGESIZE);
            }
            auto size_for_lower_lvl = std::min(vaddr_end, PG_LVL_ALIGN(vaddr_it + PG_LVL_SZ(lvl), lvl)) - vaddr_it;

            bitmap_get_vaddr(default_pagemem_allocator, PG_GET_ADDRESS(&pg[pg_idx]), addr);
            ret = allocate_vaddr < (lvl > 1) ? lvl - 1 : lvl > ((page_struct *)addr, vaddr_it, paddr, size_for_lower_lvl, privilege, pageType);
            if (ret)
            {
                log_error("Failed to allocate_vaddr\n");
                return ret;
            }
            pg[pg_idx].us = (privilege == PageManager::Privilege::User) ? 1 : 0;
            vaddr_it += size_for_lower_lvl;
            paddr += size_for_lower_lvl;
        }
    }
    return ret;
}

int PageManager::setVirtualToPhysicalMemory(uint64_t vaddr, uint64_t paddr, uint64_t size, Privilege privilege, PageType pageType)
{
    int ret = 0;
    ret = allocate_vaddr((page_struct *)cpu_local_pml4t, vaddr, paddr, size, privilege, pageType);
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
static int freememory_helper(page_struct *pg, uint64_t vaddr, uint64_t sz, uint64_t &sz_alloc)
{
    for (uint64_t l1 = PG_LVL_IDX(vaddr + sz_alloc, level); (l1 < 512) && (sz_alloc < sz); l1++)
    {
        if (pg[l1].present == 1)
        {
            if (level > 1 && pg[l1].ps == 0)
            {
                auto new_pg = (page_struct *)(PG_GET_ADDRESS(&pg[l1]) - default_pagemem_allocator.paddr + default_pagemem_allocator.vaddr);
                freememory_helper<(level > 1) ? level - 1 : 1>(new_pg, vaddr, sz, sz_alloc);
            }
            else
            {
                // if (level > 1)
                // {
                //     bitmap_free(default_pagemem_allocator, PG_GET_ADDRESS(&pg[l1]));
                // }
                pg[l1].addr = 0;
                asm volatile("invlpg (%0)" ::"r"(vaddr + sz_alloc));
                sz_alloc += PG_LVL_SZ(level);
            }
        }
    }
    //  dont reclaim physical memory here. multitask relies on this.
    return 0;
}

int PageManager::freeVirtualMemory(uint64_t vaddr, uint64_t size)
{
    uint64_t sz_alloc = 0;
    return freememory_helper((page_struct *)cpu_local_pml4t, vaddr, size, sz_alloc);
    //  dont reclaim physical memory here. multitask relies on this.
}

int PageManager::set2MBPage([[maybe_unused]] uint64_t vaddr, [[maybe_unused]] uint64_t paddr, [[maybe_unused]] Privilege privilege,
                            [[maybe_unused]] PageType pageType)
{
    log_error("NOT IMPLEMENTED!\n");
    asm("cli;hlt");
    return -ENOSYS;
}

int PageManager::IdentityMapPages(uint64_t paddr, uint64_t size)
{
    int ret = 0;
    uint64_t sz_alloc = 0;
    ret = allocate_vaddr((page_struct *)cpu_local_pml4t, paddr, paddr, size, PageManager::Privilege::Supervisor, PageManager::PageType::Read_Write);
    return ret;
}

int PageManager::findVirtualMemory([[maybe_unused]] uint64_t paddr, [[maybe_unused]] uint64_t &vaddr)
{
    log_error("NOT IMPLEMENTED!\n");
    asm("cli;hlt");
    return -ENOSYS;
}

template <int level = 4>
static int findfreememory_helper(page_struct *pg, uint64_t &vaddr, uint64_t sz, uint64_t offset, uint64_t &sz_alloc, uint64_t parent_vaddr = 0,
                                 bool restart_counting = true)
{
    auto base = std::max(parent_vaddr, offset);
    for (uint64_t l1 = PG_LVL_IDX(base, level); l1 < 512; l1++)
    {
        if (pg[l1].present == 1)
        {
            if (level > 1 && pg[l1].ps == 0)
            {
                auto new_pg = (page_struct *)(PG_GET_ADDRESS(&pg[l1]) - default_pagemem_allocator.paddr + default_pagemem_allocator.vaddr);
                restart_counting = findfreememory_helper < (level > 1)
                                     ? level - 1
                                     : 1 > (new_pg, vaddr, sz, offset, sz_alloc, parent_vaddr + (l1 * PG_LVL_SZ(level)), restart_counting);
                if (sz_alloc >= sz)
                    return true;
            }
            else if (!restart_counting)
            {
                log_debug("vaddr:%#llx, size:%#llx\n", vaddr, sz_alloc);
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
            log_error("NOT IMPLIMENTED!\n");
            asm("cli;hlt");
        }
    }
    if (level == 4 && !restart_counting)
    {
        log_debug("vaddr:%#llx, size:%#llx\n", vaddr, sz_alloc);
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
    log_error("NOT IMPLEMENTED!\n");
    asm("cli;hlt");
    return -ENOSYS;
}

int PageManager::add_early_kernel_mapping(uint64_t vaddr, uint64_t paddr, uint64_t size)
{
    if (early_kernel_mappings_count >= (sizeof(early_kernel_mappings) / sizeof(early_kernel_mappings[0])))
    {
        log_error("too many mappings\n");
        asm("cli;hlt");
    }
    early_kernel_mappings[early_kernel_mappings_count].vaddr = vaddr;
    early_kernel_mappings[early_kernel_mappings_count].paddr = paddr;
    early_kernel_mappings[early_kernel_mappings_count].size = size;
    early_kernel_mappings_count++;
    return 0;
}

int PageManager::applyMemoryMap(const std::vector<MemPage> &memMap, Privilege privilege, PageType pageType)
{
    for (auto map : memMap)
    {
        setVirtualToPhysicalMemory(map.vaddr, map.paddr, map.size, privilege, pageType);
    }
    return 0;
}
int PageManager::removeMemoryMap(const std::vector<MemPage> &memMap)
{
    int ret = 0;
    for (auto map : memMap)
    {
        ret = freeVirtualMemory(map.vaddr, map.size);
    }
    return ret;
}
