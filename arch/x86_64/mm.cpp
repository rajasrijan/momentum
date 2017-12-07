/*
 * Copyright 2009-2017 Srijan Kumar Sharma
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

uint64_t total_ram;
uint32_t ram_end;
static uint64_t mm_pop(void);
uint64_t *pm_stack;
uint32_t pm_esp = 0;
uint32_t mStackSize = 0;
uint32_t reserved_mem_entries;
heap_t* pHeap;
struct MemMap_t
{
	uint64_t addr, len;
}g_pMemAvailable[64];
typedef struct reserved_memory
{
	uint32_t address;
	uint32_t length;
} reserved_memory_t;
static reserved_memory_t *resv_mem;

void initilize_memorymanager(multiboot_info* mbi)
{
	memset((void*)g_pMemAvailable, 0, sizeof(g_pMemAvailable));
	multiboot_memory_map_t* mem_map = (multiboot_memory_map_t*)((uint64_t)mbi->mmap_addr);
	uint32_t length = mbi->mmap_length;
	uint64_t mAvailableMemory = 0;
	const char *memType[] = { "AVAILABLE","RESERVED","ACPI RECLAIMABLE","NVS","BADRAM" };
	int index = 0;
	for (int i = 0; (i < 64) && (length > 0); i++)
	{
		printf("addr [0x%x], len [0x%x], type [%s]\n", mem_map->addr, mem_map->len, memType[mem_map->type - 1]);
		if (mem_map->type == MULTIBOOT_MEMORY_AVAILABLE)
		{
			g_pMemAvailable[index].addr = mem_map->addr;
			g_pMemAvailable[index].len = mem_map->len;
			mAvailableMemory += g_pMemAvailable[index].len;
			mStackSize += (g_pMemAvailable[index].len / PageManager::PAGESIZE);
			index++;
		}
		else if ((mem_map->type == MULTIBOOT_MEMORY_RESERVED) || (mem_map->type == MULTIBOOT_MEMORY_ACPI_RECLAIMABLE))
		{
			for (uint64_t resvSz = 0; resvSz < mem_map->len; resvSz += 0x200000)
			{
				PageManager::getInstance()->IdentityMap2MBPages((mem_map->addr + resvSz) & 0xFFFFFFFFFFE00000);
			}
		}
		length -= (mem_map->size + sizeof(multiboot_memory_map_t::size));
		mem_map = (multiboot_memory_map_t*)((uint64_t)mem_map + mem_map->size + sizeof(multiboot_memory_map_t::size));
	}
	printf("%d Bytes usable memory found.\n", mAvailableMemory);
	pm_stack = new uint64_t[mStackSize];
	mStackSize = 0;
	for (size_t i = 0; i < sizeof(g_pMemAvailable) / sizeof(g_pMemAvailable[0]); i++)
	{
		if (g_pMemAvailable[i].addr == 0 && g_pMemAvailable[i].len == 0)
			break;
		uint64_t addr = PageManager::roundToPageSize(g_pMemAvailable[i].addr);
		for (; addr < g_pMemAvailable[i].addr + g_pMemAvailable[i].len; addr += PageManager::PAGESIZE)
		{
			if (addr < (uint64_t)g_qKernelEnd)
			{
				continue;
			}
			pm_stack[mStackSize] = (addr);
			mStackSize++;
			addr += PageManager::PAGESIZE;

		}
	}
}

static uint64_t mm_pop(void)
{
	pm_esp++;
	return pm_stack[pm_esp];
}

/*
 * Create kernel heap. 128MB to start with.
 */
void create_kernel_heap()
{
	g_qKernelEnd = (void*)((((uint64_t)g_qKernelEnd + 0x200000 - 1) / 0x200000) * 0x200000);
	uint64_t heapSize = 0x8000000;
	PageManager::getInstance()->setVirtualToPhysicalMemory((uint64_t)KERNEL_HEAP_PTR, (uint64_t)g_qKernelEnd, 0x8000000);
	g_qKernelEnd = (char*)g_qKernelEnd + 0x8000000;
	pHeap = (heap_t*)KERNEL_HEAP_PTR;
	pHeap->flags = HEAP_EMPTY;
	pHeap->size = heapSize;
	pHeap->checksum = 0;
	pHeap->checksum = getsum((uint8_t*)pHeap, sizeof(heap_t));
}

uint64_t get_2mb_block()
{
	return mm_pop();
}

int IsMemoryReserved(uint32_t mem_addr)
{
	for (uint32_t i = 0; i < reserved_mem_entries; i++)
	{
		if ((mem_addr >= resv_mem[i].address)
			&& (mem_addr <= (resv_mem[i].address + resv_mem[i].length)))
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
		if (checksum((uint8_t*)heap_ptr, sizeof(heap_t)) == 0)
			if ((heap_ptr->flags == HEAP_EMPTY)
				&& (heap_ptr->size >= length + (2 * sizeof(heap_t))))
			{
				uint32_t size = length + sizeof(heap_t);
				heap_ptr->size -= size;
				heap_ptr->checksum = 0;
				heap_ptr->checksum = getsum((uint8_t*)heap_ptr, sizeof(heap_t));
				heap_ptr = (heap_t*)((char*)heap_ptr + heap_ptr->size);
				heap_ptr->flags = HEAP_FULL;
				heap_ptr->checksum = 0;
				heap_ptr->checksum = getsum((uint8_t*)heap_ptr, sizeof(heap_t));
				ptr = (void*)((char*)heap_ptr + sizeof(heap_t));
				return (void*)ptr;
			}
			else
			{
				heap_ptr = (heap_t*)((char*)heap_ptr + heap_ptr->size);
			}
		else
		{
			printf("\nheap corruption");
			__asm__("cli;hlt;");
			return (void*)ptr;
		}

	}
	return (void*)ptr;
}

void _free(void *ptr)
{
	#warning compact heap.
		heap_t *heap_ptr = (heap_t*)((char*)ptr - sizeof(heap_t));
	if (checksum((uint8_t*)heap_ptr, sizeof(heap_t)) != 0)
	{
		printf("\nheap corruption");
		__asm__("cli;hlt;");
	}
	heap_ptr->flags = HEAP_EMPTY;
	heap_ptr->checksum = 0;
	heap_ptr->checksum = getsum((uint8_t*)heap_ptr, sizeof(heap_t));
}
