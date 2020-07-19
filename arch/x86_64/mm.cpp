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

#include "mm.h"
#include "global.h"
#include "paging.h"
#include <ctype.h>
#include <string.h>
#include <kernel/config.h>
#include "multiboot2.h"
#include <errno.h>
#include <kernel/bitmap_allocator.h>

bitmap_allocator<KERNEL_MAXIMUM_PHYSICAL_RAM / PageManager::PAGESIZE, PageManager::PAGESIZE> mem_available = {};

heap_t *pHeap;

struct MemMap_t
{
    uint64_t addr, len;
} g_pMemAvailable[64] = {}, g_pMemReserved[64] = {};
size_t g_szMemAvailableCount = 0, g_szMemReservedCount = 0;

int initilize_memorymanager(multiboot_tag_mmap *mbi)
{
    for (uint64_t &v : mem_available.bitmap)
        v = 0xFFFFFFFFFFFFFFFF;
    multiboot_memory_map_t *mem_map = mbi->entries;
    ssize_t length = mbi->size - sizeof(multiboot_tag_mmap);
    uint64_t mAvailableMemory = 0;
    uint64_t gKernelEnd = (uint64_t)&kernel_end - KERNEL_BASE_PTR;
    const char *memType[] = {"AVAILABLE", "RESERVED", "ACPI RECLAIMABLE", "NVS", "BADRAM"};
    for (int i = 0; (i < 64) && (length > 0); i++)
    {
        printf("addr [0x%x], len [0x%x], type [%s]\n", mem_map->addr, mem_map->len, memType[mem_map->type - 1]);
        if (mem_map->type == MULTIBOOT_MEMORY_AVAILABLE)
        {
            g_pMemAvailable[g_szMemAvailableCount].addr = mem_map->addr;
            g_pMemAvailable[g_szMemAvailableCount].len = mem_map->len;
            mAvailableMemory += g_pMemAvailable[g_szMemAvailableCount].len;
            g_szMemAvailableCount++;
        }
        else if ((mem_map->type == MULTIBOOT_MEMORY_RESERVED) || (mem_map->type == MULTIBOOT_MEMORY_ACPI_RECLAIMABLE))
        {
            g_pMemReserved[g_szMemReservedCount].addr = mem_map->addr;
            g_pMemReserved[g_szMemReservedCount].len = mem_map->len;
            g_szMemReservedCount++;
        }
        length -= mbi->entry_size;
        mem_map = (multiboot_memory_map_t *)((char *)mem_map + mbi->entry_size);
    }
    printf("%d Bytes usable memory found.\n", mAvailableMemory);
    for (size_t i = 0; i < g_szMemAvailableCount; i++)
    {
        if (g_pMemAvailable[i].addr == 0 && g_pMemAvailable[i].len == 0)
            break;
        uint64_t addr = PageManager::roundToPageSize(g_pMemAvailable[i].addr);
        for (; addr < g_pMemAvailable[i].addr + g_pMemAvailable[i].len; addr += PageManager::PAGESIZE)
        {
            if (addr < gKernelEnd)
            {
                continue;
            }
            bitmap_free(mem_available, addr);
        }
    }
    return 0;
}

/*
 * Create kernel heap. 128MB to start with.
 */
int create_kernel_heap()
{
    int ret = 0;
    if ((ret = PageManager::setPageAllocation(KERNEL_HEAP_PTR, PageManager::PAGESIZE * 8, PageManager::Privilege::Supervisor, PageManager::PageType::Read_Write)) < 0)
    {
        printf("failed to allocate pages for kernel heap\n");
        return ret;
    }
    pHeap = (heap_t *)KERNEL_HEAP_PTR;
    pHeap->flags = HEAP_EMPTY;
    pHeap->size = PageManager::PAGESIZE * 8;
    pHeap->checksum = 0;
    pHeap->checksum = getsum((uint8_t *)pHeap, sizeof(heap_t));
    return 0;
}

uint64_t get_2mb_block()
{
    uint64_t p = 0, v = 0;
    if (bitmap_alloc(mem_available, p, v))
        asm("cli;hlt");
    return p;
}

void rel_2mb_block(uint64_t p)
{
    if (bitmap_free(mem_available, p))
        asm("cli;hlt");
}

/*
 * Aligned allocate memory in kernel heap.B
 */
extern "C" void *_aligned_malloc(uint32_t len, int n)
{
    heap_t *heap_ptr = pHeap;
    volatile void *ptr = 0;
    uint32_t alignment = 1 << n;
    while (true)
    {
        if (checksum((uint8_t *)heap_ptr, sizeof(heap_t)) == 0)
        {
            //	allocations are in reverse order
            uint32_t length = (uint32_t)(((uint64_t)heap_ptr + heap_ptr->size) - (((uint64_t)heap_ptr + heap_ptr->size - len) & (~(alignment - 1))));
            if ((heap_ptr->flags == HEAP_EMPTY) && (heap_ptr->size >= length + (2 * sizeof(heap_t))))
            {
                uint32_t size = length + sizeof(heap_t);
                heap_ptr->size -= size;
                heap_ptr->checksum = 0;
                heap_ptr->checksum = getsum((uint8_t *)heap_ptr, sizeof(heap_t));
                heap_ptr = (heap_t *)((uint64_t)heap_ptr + heap_ptr->size);
                heap_ptr->flags = HEAP_FULL;
                heap_ptr->checksum = 0;
                heap_ptr->checksum = getsum((uint8_t *)heap_ptr, sizeof(heap_t));
                ptr = (void *)((char *)heap_ptr + sizeof(heap_t));
                return (void *)ptr;
            }
            else
            {
                heap_ptr = (heap_t *)((uint64_t)heap_ptr + heap_ptr->size);
            }
        }
        else
        {
            printf("\nheap corruption");
            __asm__("cli;hlt;");
            return (void *)ptr;
        }
    }
    return (void *)ptr;
}

/*
 * Allocate memory in kernel heap.
 */
void *_malloc(uint32_t length)
{
    return _aligned_malloc(length, 4);
}

void _free(void *ptr)
{
#pragma message("compact heap.")
    if (!ptr)
        return;
    heap_t *heap_ptr = (heap_t *)((uint64_t)ptr - sizeof(heap_t));
    if (heap_ptr->flags == HEAP_EMPTY)
    {
        printf("Double free\n");
        __asm__("cli;hlt;");
    }
    memset(ptr, 0xcc, heap_ptr->size);
    if (checksum((uint8_t *)heap_ptr, sizeof(heap_t)) != 0)
    {
        printf("heap corruption\n");
        __asm__("cli;hlt;");
    }
    heap_ptr->flags = HEAP_EMPTY;
    heap_ptr->checksum = 0;
    heap_ptr->checksum = getsum((uint8_t *)heap_ptr, sizeof(heap_t));
}
