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
#include <arch/efi/efi.h>
#include <map>
#include "paging.h"
#include <errno.h>
#include <kernel/logging.h>

extern EFI_SYSTEM_TABLE *st;
uint64_t memoryMapKey = 0;
uint64_t memoryMapSize = 0;
uint64_t descriptorSize = 0;
EFI_MEMORY_DESCRIPTOR *memoryMap = 0;

int init_paging(void)
{
    int ret = 0;
    ret = PageManager::initialize();
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
    auto pgno = size / EFI_PAGE_SIZE;
    EFI_PHYSICAL_ADDRESS memory = (EFI_PHYSICAL_ADDRESS)vaddr;

    ret = st->BootServices->AllocatePages(AllocateAddress, EfiLoaderData, pgno, &memory);
    if (ret)
    {
        log_error("Failed to allocate pages! error %d\n", ret);
        return ret;
    }
    vaddr = memory;
    return ret;
}

int PageManager::findFreeVirtualMemory(uint64_t &vaddr, uint64_t sz, uint64_t offset)
{
    int ret = 0;
    log_info("%s:%d %s\n", __FILE__, __LINE__, __FUNCTION__);
    auto pgno = sz / EFI_PAGE_SIZE;
    log_info("vaddr=%llx, size=%llx\n", vaddr, pgno);
    EFI_PHYSICAL_ADDRESS memory = (EFI_PHYSICAL_ADDRESS)vaddr;

    ret = st->BootServices->AllocatePages(AllocateAnyPages, EfiLoaderData, pgno, &memory);
    if (ret)
    {
        log_error("Failed to allocate pages! error %d\n", ret);
        return ret;
    }
    vaddr = memory;
    return ret;
}

int PageManager::getPhysicalAddress(uint64_t vaddr, uint64_t &paddr)
{
    log_info("%s:%d %s\n", __FILE__, __LINE__, __FUNCTION__);
    return -ENOSYS;
}

int PageManager::freeVirtualMemory(uint64_t vaddr, uint64_t size)
{
    log_info("%s:%d %s\n", __FILE__, __LINE__, __FUNCTION__);
    return -ENOSYS;
}

int PageManager::setVirtualToPhysicalMemory(uint64_t vaddr, uint64_t paddr, uint64_t size, Privilege privilege, PageType pageType)
{
    log_info("%s:%d %s\n", __FILE__, __LINE__, __FUNCTION__);
    // int ret = 0;
    // if (!memoryMap) {
    //     free(memoryMap);
    //     memoryMap = nullptr;
    // }

    // ret = st->BootServices->GetMemoryMap(&memoryMapSize, nullptr, &memoryMapKey, &descriptorSize,
    // &descriptorVersion); if (ret < 0) {
    //     log_error("Failed to get size of memory map. error %d.\n", ret);
    // }

    // log_error("memoryMapSize=%d, memoryMapKey=%d, descriptorSize=%d, descriptorVersion=%d\n", memoryMapSize,
    // memoryMapKey, descriptorSize, descriptorVersion);

    // memoryMap = (EFI_MEMORY_DESCRIPTOR *)malloc(memoryMapSize);

    // ret = st->BootServices->GetMemoryMap(&memoryMapSize, memoryMap, &memoryMapKey, &descriptorSize,
    // &descriptorVersion); if (ret < 0) {
    //     log_error("Failed to get memory map. error %d.\n", ret);
    // }
    // log_error("memoryMapSize=%d, memoryMapKey=%d, descriptorSize=%d, descriptorVersion=%d\n", memoryMapSize,
    // memoryMapKey, descriptorSize, descriptorVersion);

    return -ENOSYS;
}

int PageManager::initialize()
{
    int ret = 0;
    return ret;
}

int PageManager::getMemoryMap(uint64_t &memoryMapKey, uint64_t &memoryMapSize, uint64_t &descriptorSize, EFI_MEMORY_DESCRIPTOR **memoryMap)
{
    int ret = 0;
    uint32_t descriptorVersion = 0;
    ret = st->BootServices->GetMemoryMap(&memoryMapSize, nullptr, &memoryMapKey, &descriptorSize, &descriptorVersion);
    if (ret != 0)
    {
        log_error("Failed to get size of memory map. error %d.\n", ret);
    }
    log_error("memoryMapSize=%d, memoryMapKey=%d, descriptorSize=%d, descriptorVersion=%d\n", memoryMapSize, memoryMapKey, descriptorSize, descriptorVersion);
    *memoryMap = (EFI_MEMORY_DESCRIPTOR *)malloc(memoryMapSize);
    ret = st->BootServices->GetMemoryMap(&memoryMapSize, *memoryMap, &memoryMapKey, &descriptorSize, &descriptorVersion);
    if (ret != 0)
    {
        log_error("Failed to get memory map. error %d.\n", ret);
    }
    log_error("memoryMapSize=%d, memoryMapKey=%d, descriptorSize=%d, descriptorVersion=%d\n", memoryMapSize, memoryMapKey, descriptorSize, descriptorVersion);
    return ret;
}
