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

#ifndef PAGING_H
#define	PAGING_H
#include <stdint.h>
#include "interrupts.h"
#include "../../kernel/lists.h"
#include <vector>

extern "C" uint64_t PML4T[2];
extern "C" uint64_t PDPT[512];
extern "C" uint64_t PDT[512 * 512];

class PageManager
{
private:
	PageManager();
	/*
		handle page faults
	*/
	static void interruptHandler(retStack_t *stack, general_registers_t *regs);
public:
	const static uint64_t PAGESIZE = 0x200000;
	const static uint64_t BIGPAGESIZE = 0x40000000;
	const static uint64_t BIGBIGPAGESIZE = 0x8000000000;
	static PageManager* getInstance();
	static uint64_t roundToPageSize(uint64_t sz);
	int setPageAllocation(uint64_t vaddr, uint64_t size);
	int setVirtualToPhysicalMemory(uint64_t vaddr, uint64_t paddr, uint64_t size);
	int freeVirtualMemory(uint64_t vaddr, uint64_t size);
	int set2MBPage(uint64_t vaddr, uint64_t paddr);
	int IdentityMap2MBPages(uint64_t paddr);
	int initialize();
	int findFreeVirtualMemory(uint64_t &vaddr, uint64_t sz, uint64_t offset = 0);
	~PageManager();
};

void map_4mb(uint64_t virtual_address);
typedef volatile struct paging_structure paging_structure_t;
void new_paging_structure(paging_structure_t* ps);
void init_paging(void);
uint32_t get_physical_address(uint32_t virtual_address);

#endif	/* PAGING_H */
