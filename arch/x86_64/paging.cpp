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

#include "paging.h"
#include "mm.h"
#include "global.h"
#include "string.h"
#include "stdlib.h"

void new_paging_structure(paging_structure_t* ps)
{

}

//void map_4mb(uint64_t virtual_address)
//{
//
//	uint64_t boundry_4mb = (virtual_address / 0x400000);
//
//	uint64_t boundry_4kb = boundry_4mb * 0x400;
//
//	uint64_t memory_slab = get_2mb_block();
//
//	if (memory_slab == 0)
//		__asm__("cli;hlt;");
//
//	__asm__("xchg %bx,%bx;");
//
//	sys_info.pst->page_directory[boundry_4mb] = (uint64_t)&(sys_info.pst->page_table[boundry_4kb]) | 3;
//
//	for (uint64_t i = 0; i < 0x400; i++)
//	{
//		sys_info.pst->page_table[boundry_4kb + i] = memory_slab | 3;
//		memory_slab += 0x1000;
//		__asm__ volatile("invlpg %0"::"m" (*(char *)(memory_slab)));
//	}
//
//}

PageManager::PageManager()
{

}

PageManager * PageManager::getInstance()
{
	static PageManager pageManager;
	return &pageManager;
}

uint64_t PageManager::roundToPageSize(uint64_t sz)
{
	return ((sz + 0x1FFFFF) & 0xFFFFFFFFFFE00000);
}

int PageManager::setPageAllocation(uint64_t vaddr, uint64_t size)
{
	if (size % PAGESIZE)
	{
		printf("Size not 2MB aligned\n");
		__asm__("cli;hlt;");
	}
	for (size_t i = 0; i < size / PAGESIZE; i++)
	{
		uint64_t memory_slab = get_2mb_block();
		printf("SLAB addr [%x]\n", memory_slab);
		setVirtualToPhysicalMemory(vaddr, memory_slab, PAGESIZE);
	}
	return 0;
}

int PageManager::setVirtualToPhysicalMemory(uint64_t vaddr, uint64_t paddr, uint64_t size)
{
	for (uint64_t i = 0; i < size; i += PAGESIZE)
	{
		set2MBPage(vaddr, paddr);
		vaddr += PAGESIZE;
		paddr += PAGESIZE;
	}
	return 0;
}

int PageManager::freeVirtualMemory(uint64_t vaddr, uint64_t size)
{
	while (size > 0)
	{
		int lvl4 = (vaddr >> 39) & 0x1FF;
		int lvl3 = (vaddr >> 30) & 0x1FF;
		int lvl2 = (vaddr >> 21) & 0x1FF;
		if (lvl4 > 0)
		{
			return 1;
		}
		if (lvl2 == 0)
		{
			if (lvl3 == 0)
			{
				if (lvl4 = 0)
				{

				}
				else if (size >= BIGBIGPAGESIZE)
				{
					PML4T[lvl4] &= 0xFFFFFFFFFFFFFFFE;
					size -= BIGBIGPAGESIZE;
					vaddr += BIGBIGPAGESIZE;
				}
				else if (size >= BIGPAGESIZE)
				{
					PDPT[lvl3] &= 0xFFFFFFFFFFFFFFFE;
					size -= BIGPAGESIZE;
					vaddr += BIGPAGESIZE;
				}
				else if (size >= PAGESIZE)
				{
					PDT[(lvl3 * 512) + lvl2] &= 0xFFFFFFFFFFFFFFFE;
					size -= PAGESIZE;
					vaddr += PAGESIZE;
				}
			}
			else if (size >= BIGPAGESIZE)
			{
				PDPT[lvl3] &= 0xFFFFFFFFFFFFFFFE;
				size -= BIGPAGESIZE;
				vaddr += BIGPAGESIZE;
			}
			else if (size >= PAGESIZE)
			{
				PDT[(lvl3 * 512) + lvl2] &= 0xFFFFFFFFFFFFFFFE;
				size -= PAGESIZE;
				vaddr += PAGESIZE;
			}
		}
		else if (size >= PAGESIZE)
		{
			PDT[(lvl3 * 512) + lvl2] &= 0xFFFFFFFFFFFFFFFE;
			size -= PAGESIZE;
			vaddr += PAGESIZE;
		}

	}
	return 0;
}

int PageManager::set2MBPage(uint64_t vaddr, uint64_t paddr)
{
	if (paddr % PAGESIZE)
	{
		printf("Physical address not correctly aligned\n");
		__asm("cli;hlt");
	}
	int lvl4 = (vaddr >> 39) & 0x1FF;
	int lvl3 = (vaddr >> 30) & 0x1FF;
	int lvl2 = (vaddr >> 21) & 0x1FF;

	if (lvl4 > 0)
	{
		printf("Not supported yet.\n");
		__asm("cli;hlt");
	}
	PML4T[lvl4] = ((uint64_t)&PDPT[0] - 0xC0000000) | 0x3;
	PDPT[lvl3] = ((uint64_t)&PDT[(lvl3 * 512)] - 0xC0000000) | 0x3;
	PDT[(lvl3 * 512) + lvl2] = paddr | 0x83;
	__asm__ volatile("invlpg %0"::"m" (*(char *)(paddr)));
	return 0;
}

int PageManager::IdentityMap2MBPages(uint64_t paddr)
{
	return set2MBPage(paddr, paddr);
}

int PageManager::initialize()
{
	//register_interrupt_handler(0x0E, interruptHandler);
	return 0;
}

int PageManager::findFreeVirtualMemory(uint64_t &vaddr, uint64_t sz, uint64_t offset)
{
	offset = roundToPageSize(offset);
	int lvl4 = (offset >> 39) & 0x1FF;
	int lvl3 = (offset >> 30) & 0x1FF;
	int lvl2 = (offset >> 21) & 0x1FF;
	uint64_t foundSize = 0;
	do
	{
		if (!(PDT[(lvl3 * 512) + lvl2] & 1))
		{
			foundSize += PAGESIZE;
		}
		else
		{
			offset += PAGESIZE;
		}
		lvl4 = (offset >> 39) & 0x1FF;
		lvl3 = (offset >> 30) & 0x1FF;
		lvl2 = (offset >> 21) & 0x1FF;
	} while ((lvl4 < 1) && foundSize < sz);
	if (foundSize >= sz)
	{
		vaddr = offset;
		return 0;
	}
	else
	{
		return 1;
	}
	return 0;
}

PageManager::~PageManager()
{
}
