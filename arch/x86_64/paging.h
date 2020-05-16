/*
 * Copyright 2009-2019 Srijan Kumar Sharma
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
#define PAGING_H
#include <stdint.h>
#include "interrupts.h"
#include <vector>
#include <utility>

extern "C" uint64_t PML4T[2];
extern "C" uint64_t PDPT[512];
extern "C" uint64_t PDT[512 * 512];

struct PageDirectory
{
	uint64_t PML4T[512];
	uint64_t PDPT[512];
	uint64_t PDT[512 * 512];
};

struct MemPage
{
	uint64_t vaddr, paddr, size;
	uint64_t vend() const { return vaddr + size; }
	bool vinside(uint64_t addr) const { return (addr >= vaddr) && (addr < vend()); }
};

class PageManager
{
public:
	enum Privilege
	{
		Supervisor = 0,
		User = 1
	};
	enum PageType
	{
		Read_Only = 0,
		Read_Write = 1
	};

private:
	PageManager();
	int set2MBPage(uint64_t vaddr, uint64_t paddr, Privilege privilege, PageType pageType);
	/*
		handle page faults
	*/
	static void interruptHandler(retStack_t *stack, general_registers_t *regs);

public:
	const static uint64_t PAGESIZE = 0x200000;
	const static uint64_t BIGPAGESIZE = 0x40000000;
	const static uint64_t BIGBIGPAGESIZE = 0x8000000000;
	static PageManager *getInstance();
	static uint64_t roundToPageSize(uint64_t sz);
	static uint64_t roundToPageBoundry(uint64_t addr);
	int setPageAllocation(uint64_t vaddr, uint64_t size, Privilege privilege, PageType pageType);
	int setVirtualToPhysicalMemory(uint64_t vaddr, uint64_t paddr, uint64_t size, Privilege privilege, PageType pageType);
	uint64_t getPhysicalAddress(uint64_t virtual_address);
	uint64_t getVirtualAddress(uint64_t physical_address, uint64_t length);
	int freeVirtualMemory(uint64_t vaddr, uint64_t size);
	int IdentityMap2MBPages(uint64_t paddr);
	int initialize();
	int findVirtualMemory(uint64_t paddr, uint64_t &vaddr);
	int findFreeVirtualMemory(uint64_t &vaddr, uint64_t sz, uint64_t offset = 0);
	int getMemoryMap(std::vector<MemPage> &memMap);
	~PageManager();
};

void map_4mb(uint64_t virtual_address);
typedef volatile struct paging_structure paging_structure_t;
void new_paging_structure(paging_structure_t *ps);
void init_paging(void);

#endif /* PAGING_H */
