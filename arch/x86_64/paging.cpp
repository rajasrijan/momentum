/*
 * Copyright 2009-2018 Srijan Kumar Sharma
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

void new_paging_structure(paging_structure_t *ps)
{
}

PageManager::PageManager()
{
}

PageManager *PageManager::getInstance()
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

uint64_t PageManager::getPhysicalAddress(uint64_t virtual_address)
{
	uint64_t paddr = 0;
	uint64_t lvl4 = (virtual_address >> 39) & 0x1FF;
	uint64_t lvl3 = (virtual_address >> 30) & 0x1FF;
	uint64_t lvl2 = (virtual_address >> 21) & 0x1FF;
	uint64_t lvl1 = virtual_address & 0x3FFFFF;
	uint64_t tmp4 = (PML4T[lvl4] & (~0xFF)) + 0xC0000000;
	uint64_t tmp3 = (((uint64_t *)tmp4)[lvl3] & (~0xFF)) + 0xC0000000;
	paddr = (((uint64_t *)tmp3)[lvl2] & (~0xFF)) + lvl1;
	return paddr;
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
				if (lvl4 == 0)
				{
					if (size >= BIGBIGPAGESIZE)
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
					else
					{
						printf("Invalid page size [%x]\n", size);
						asm("cli;hlt");
					}
				}
				else
				{
					printf("lvl4 pages shouldnt exist\n");
					asm("cli;hlt");
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
	asm volatile ( "invlpg (%0)" : : "b"(vaddr) : "memory" );
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

int PageManager::findVirtualMemory(uint64_t paddr, uint64_t &vaddr)
{
	for (uint64_t lvl4 = 0; lvl4 < 1; lvl4++)
		for (uint64_t lvl3 = 0; lvl3 < 512; lvl3++)
			for (uint64_t lvl2 = 0; lvl2 < 512; lvl2++)
			{
				if (PDT[(lvl3 * 512) + lvl2] & 1)
				{
					uint64_t phy_addr = PDT[(lvl3 * 512) + lvl2] & 0xFFFFFFFFFFFFFF00;
					if (paddr >= phy_addr && paddr < phy_addr + PAGESIZE)
					{
						vaddr = ((lvl4 << 39) | (lvl3 << 30) | (lvl2 << 21)) + (paddr % PAGESIZE);
						return 0;
					}
				}
			}
	return 1;
}

int PageManager::findFreeVirtualMemory(uint64_t &vaddr, uint64_t sz, uint64_t offset)
{
	offset = roundToPageSize(offset);
	uint64_t lvl4 = (offset >> 39) & 0x1FF;
	uint64_t lvl3 = (offset >> 30) & 0x1FF;
	uint64_t lvl2 = (offset >> 21) & 0x1FF;
	uint64_t foundSize = 0;
	for (; lvl4 < 1 && foundSize < sz; lvl4++)
	{
		for (; lvl3 < 512 && foundSize < sz; lvl3++)
		{
			//	TODO: perform lvl3 checks
			for (; lvl2 < 512 && foundSize < sz; lvl2++)
			{
				if ((PDT[(lvl3 * 512) + lvl2] & 1) == 0)
				{
					if (foundSize == 0)
					{
						vaddr = ((lvl4 << 39) | (lvl3 << 30) | (lvl2 << 21));
					}
					foundSize += PAGESIZE;
				}
				else
				{
					foundSize = 0;
					vaddr = 0;
				}
			}
		}
	}
	if (foundSize < sz)
	{
		return 1;
	}
	return 0;
}

PageManager::~PageManager()
{
}

int PageManager::getMemoryMap(std::vector<MemPage> &memMap)
{
	uint64_t vaddr = 0, paddr = 0;
	uint8_t flags = 0;
	for (uint64_t lvl4 = 0; lvl4 < 1; lvl4++)
		for (uint64_t lvl3 = 0; lvl3 < 512; lvl3++)
			for (uint64_t lvl2 = 0; lvl2 < 512; lvl2++)
			{
				uint8_t _flags = PDT[(lvl3 * 512) + lvl2] & 0x07;
				uint64_t _paddr = PDT[(lvl3 * 512) + lvl2] & 0xFFFFFFFFFFFFFF00;
				vaddr = ((lvl4 << 39) | (lvl3 << 30) | (lvl2 << 21));
				if (((_flags != flags) || (_paddr != (paddr + PAGESIZE))))
				{
					if ((_flags & 1) == 1)
					{
						MemPage page = {vaddr, _paddr, PAGESIZE};
						memMap.push_back(page);
					}
				}
				else
				{
					memMap.back().size += PAGESIZE;
				}
				paddr = _paddr;
				flags = _flags;
			}
	return 0;
}