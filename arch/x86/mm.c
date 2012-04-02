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

#include "mm.h"
#include "global.h"
#include <ctype.h>

uint64_t total_ram;
static void mm_push(uint16_t val);
static uint16_t mm_pop(void);
static uint16_t *pm_stack;
static uint32_t pm_esp;
static uint32_t reserved_mem_entries;
static heap_t* pHeap;

typedef struct reserved_memory
{
    uint32_t address;
    uint32_t length;
} reserved_memory_t;
static reserved_memory_t *resv_mem;

static void* kalloc(uint32_t size, uint32_t align)
{
    sys_info.kernel_end = (((sys_info.kernel_end) / align) * align) + size;
    return (void*) (sys_info.kernel_end - size);
}

void initilize_memorymanager()
{
    multiboot_memory_map_t *mmap = sys_info.memory_map;
    uint32_t entries = sys_info.memory_map_len / (mmap->size + 4);
    total_ram = 0;
    reserved_mem_entries = 0;
    for (int i = 0; i < entries; i++)
    {
        if (mmap[i].type == MULTIBOOT_MEMORY_AVAILABLE)
        {
            total_ram += mmap[i].len;
        }
        else
        {
            reserved_mem_entries++;
        }
    }
    resv_mem = (void*) kalloc(reserved_mem_entries * sizeof (reserved_memory_t), 1);
    pm_stack = (void*) kalloc((uint32_t) (total_ram / 0x400000) * sizeof (uint16_t), 4);
    pm_esp = ((uint32_t) (total_ram / 0x400000) - 1);
    for (int i = 0; i < entries; i++)
    {
        if (mmap[i].type == MULTIBOOT_MEMORY_AVAILABLE)
        {
            if (mmap[i].len >= 0x400000)
            {
                uint32_t paddress = 0;
                for (uint32_t j = 0; j < mmap[i].len; j += 0x100000)
                {
                    paddress = (uint32_t) (mmap[i].addr) + j;
                    if (paddress >= sys_info.kernel_end - 0xc0000000)
                    {
                        if ((paddress % 0x400000) == 0)
                        {
                            mm_push((uint16_t) (paddress / 0x400000));
                        }
                    }
                }
            }
        }
        else
        {
            static uint32_t no = 0;
            if (no >= reserved_mem_entries)
            {
                __asm__("cli;hlt");
            }
            resv_mem[no].length = (uint32_t) mmap[i].len;
            resv_mem[no].address = (uint32_t) mmap[i].addr;
            no++;
        }
    }
}

static void mm_push(uint16_t val)
{
    pm_stack[pm_esp] = val;
    pm_esp--;
}

static uint16_t mm_pop(void)
{
    pm_esp++;
    return pm_stack[pm_esp];
}

/*
 * Create kernel heap. 10MB to start with.
 */
void create_kernel_heap()
{
    pHeap = KERNEL_HEAP_PTR;
    pHeap->flags = HEAP_EMPTY;
    pHeap->size = 0x1000000;
    pHeap->checksum = 0;
    pHeap->checksum = getsum((uint8_t*) pHeap, sizeof (heap_t));
}

uint32_t get_4mb_block()
{
    return ((uint32_t) mm_pop())*0x400000;
}

int IsMemoryReserved(uint32_t mem_addr)
{
    for (uint32_t i = 0; i < reserved_mem_entries; i++)
    {
        if ((mem_addr >= resv_mem[i].address) && (mem_addr <= (resv_mem[i].address + resv_mem[i].length)))
        {
            return true;
        }
    }
    return false;
}

/*
 * Allocate memory in kernel heap.
 */
void* _malloc(uint32_t length)
{
    heap_t *heap_ptr = pHeap;
    volatile void* ptr = 0;
    while (true)
    {
        if (checksum((uint8_t*) heap_ptr, sizeof (heap_t)) == 0)
            if ((heap_ptr->flags == HEAP_EMPTY) && (heap_ptr->size >= length + (2 * sizeof (heap_t))))
            {
                uint32_t size = length + sizeof (heap_t);
                heap_ptr->size -= size;
                heap_ptr->checksum = 0;
                heap_ptr->checksum = getsum((uint8_t*) heap_ptr, sizeof (heap_t));
                heap_ptr = (void*) ((char*) heap_ptr + heap_ptr->size);
                heap_ptr->flags = HEAP_FULL;
                heap_ptr->checksum = 0;
                heap_ptr->checksum = getsum((uint8_t*) heap_ptr, sizeof (heap_t));
                ptr = (void*) ((char*) heap_ptr + sizeof (heap_t));
                return (void*) ptr;
            }
            else
            {
                heap_ptr = (void*) ((char*) heap_ptr + heap_ptr->size);
            }
        else
        {
            printf("\nheap corruption");
            __asm__("cli;hlt;");
            return (void*) ptr;
        }

    }
    return (void*) ptr;
}

void _free(void *ptr)
{
    heap_t *heap_ptr = (void*) ((char*) ptr - sizeof (heap_t));
    if (checksum((uint8_t*) heap_ptr, sizeof (heap_t)) != 0)
    {
        printf("\nheap corruption");
        __asm__("cli;hlt;");
    }
    heap_ptr->flags = HEAP_EMPTY;
    heap_ptr->checksum = 0;
    heap_ptr->checksum = getsum((uint8_t*) heap_ptr, sizeof (heap_t));
}
