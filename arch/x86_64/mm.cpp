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

#include <stdint.h>
#include <arch/mm.h>
#include "global.h"
#include "paging.h"
#include <sys/types.h>
#include <ctype.h>
#include <errno.h>
#include <kernel/bitmap_allocator.h>
#include <kernel/config.h>
#include <kernel/heap.h>
#include <string.h>

bitmap_allocator<KERNEL_MAXIMUM_PHYSICAL_RAM / PageManager::PAGESIZE, PageManager::PAGESIZE> mem_available_heap = {};

//  list of physical memory available to the OS
physical_memory_map mem_available[64] = {};
uint64_t mem_available_count = 0;

//  list of physical memory that is reserved
physical_memory_map mem_reserved[64] = {};
uint64_t mem_reserved_count = 0;

uint64_t available_memory = 0;

int initilize_memorymanager()
{
    //  mark all memory as allocated
    bitmap_setall(mem_available_heap);

    for (size_t i = 0; i < mem_available_count; i++)
        for (size_t j = 0; j < mem_available[i].len; j++)
        {
            bitmap_free(mem_available_heap, mem_available[i].addr + (j * PageManager::PAGESIZE));
            available_memory += PageManager::PAGESIZE;
        }

    //  mark kernel memory as allocated
    bitmap_set(mem_available_heap, KERNEL_BASE_PADDR_PTR,
               (PageManager::round_up_to_pagesize((uint64_t)&kernel_end) - PageManager::round_down_to_pagesize((uint64_t)&kernel_start)) /
                   PageManager::PAGESIZE);

    bitmap_set(mem_available_heap, 0, 1);

    return 0;
}

int allocate_physical_pages(uint64_t &addr, size_t no_pages)
{
    uint64_t vaddr = 0;
    int ret = bitmap_alloc(mem_available_heap, addr, vaddr, no_pages);
    return ret;
}

int free_physical_pages(uint64_t addr, size_t no_pages)
{
    int ret = bitmap_free(mem_available_heap, addr, no_pages);
    return ret;
}
/*
 * Create kernel heap. 128MB to start with.
 */
int create_kernel_heap()
{
    int ret = 0;
    if ((ret = PageManager::setPageAllocation(KERNEL_HEAP_PTR, HEAP_DEFAULT_SIZE, PageManager::Privilege::Supervisor, PageManager::PageType::Read_Write)) < 0)
    {
        printf("failed to allocate pages for kernel heap\n");
        return ret;
    }
    pHeap = (heap_t *)KERNEL_HEAP_PTR;
    pHeap->flags = HEAP_EMPTY;
    pHeap->next = nullptr;
    pHeap->size = HEAP_DEFAULT_SIZE;
    pHeap->checksum = 0;
    pHeap->checksum = getsum((uint8_t *)pHeap, sizeof(heap_t));
    return 0;
}
