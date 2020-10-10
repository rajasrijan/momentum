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

#ifndef PAGING_H
#define PAGING_H
#include "interrupts.h"
#include <stdint.h>
#include <utility>
#include <vector>

#define PG_LVL_IDX(vaddr, lvl) (((vaddr) >> ((9 * lvl) + 3)) & 0x1FF)
#define PG_LVL_SZ(lvl) (1ull << ((9 * lvl) + 3))
#define PG_LVL_ALIGN(vaddr, lvl) ((vaddr) & (0xFFFFFFFFFFFFFFFFull << ((9 * lvl) + 3)))
#define PG_LVL_OFFSET(vaddr, lvl) ((vaddr) & ((1ull << ((9 * lvl) + 3)) - 1))
#define PG_LVL_VADDR(l4, l3, l2, l1) (((l4 & 0x1FF) << 39) | ((l3 & 0x1FF) << 30) | ((l2 & 0x1FF) << 21) | ((l1 & 0x1FF) << 12))
#define PG_GET_ADDRESS(pg) ((pg)->paddr << 12)

#pragma pack(push, 1)
union page_struct
{
    struct
    {
        uint8_t present : 1;
        uint8_t rw : 1;
        uint8_t us : 1;
        uint8_t pwt : 1;
        uint8_t pcd : 1;
        uint8_t a : 1;
        uint8_t d : 1;
        uint8_t ps : 1;
        uint8_t ign1 : 4;
        uint64_t paddr : 51;
        uint8_t xd : 1;
    };
    uint64_t addr;
};
#pragma pack(pop)

struct MemPage
{
    uint64_t vaddr, paddr, size;
    uint64_t vend() const
    {
        return vaddr + size;
    }
    bool vinside(uint64_t addr) const
    {
        return (addr >= vaddr) && (addr < vend());
    }
};

namespace PageManager
{
const uint64_t SMALLPAGESIZE = 0x1000;
const uint64_t PAGESIZE = 0x200000;
const uint64_t BIGPAGESIZE = 0x40000000;
const uint64_t BIGBIGPAGESIZE = 0x8000000000;

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

int set2MBPage(uint64_t vaddr, uint64_t paddr, Privilege privilege, PageType pageType);
/*
    handle page faults
*/
void interruptHandler(retStack_t *stack, general_registers_t *regs);

uint64_t roundToPageSize(uint64_t sz);
uint64_t roundToPageBoundry(uint64_t addr);
int setPageAllocation(uint64_t vaddr, uint64_t size, Privilege privilege, PageType pageType);
int setVirtualToPhysicalMemory(uint64_t vaddr, uint64_t paddr, uint64_t size, Privilege privilege, PageType pageType);
int getPhysicalAddress(uint64_t vaddr, uint64_t &paddr);
int getVirtualAddress(uint64_t paddr, uint64_t length, uint64_t &vaddr);
int freeVirtualMemory(uint64_t vaddr, uint64_t size);
int IdentityMap2MBPages(uint64_t paddr);
int IdentityMapPages(uint64_t paddr, uint64_t size);
int initialize();
int findVirtualMemory(uint64_t paddr, uint64_t &vaddr);
int findFreeVirtualMemory(uint64_t &vaddr, uint64_t sz, uint64_t offset = 0x0);
int getMemoryMap(std::vector<MemPage> &memMap);
int applyMemoryMap(const std::vector<MemPage> &memMap, Privilege privilege, PageType pageType);
int removeMemoryMap(const std::vector<MemPage> &memMap);
int add_early_kernel_mapping(uint64_t vaddr, uint64_t paddr, uint64_t size);
}; // namespace PageManager

typedef volatile struct paging_structure paging_structure_t;
void new_paging_structure(paging_structure_t *ps);
void init_paging(void);

#endif /* PAGING_H */
