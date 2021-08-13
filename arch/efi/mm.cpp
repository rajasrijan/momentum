#include "efi.h"
#include <arch/mm.h>
#include <errno.h>
#include <stddef.h>
#include <kernel/heap.h>
#include <kernel/logging.h>

static uint8_t getsum(uint8_t *ptr, uint32_t len)
{
    uint8_t res = 0;
    for (uint32_t i = 0; i < len; i++)
        res = (uint8_t)(res + ptr[i]);
    return (uint8_t)(-res);
}

int initilize_memorymanager(EFI_SYSTEM_TABLE *table)
{
    int ret = 0;
    //  8mb heap size
    const size_t heap_size = 8 * 1024 * 1024;
    EFI_PHYSICAL_ADDRESS address = 0;
    ret = table->BootServices->AllocatePages(AllocateAnyPages, EfiLoaderData, heap_size / EFI_PAGE_SIZE, &address);
    log_info("memory manager base [%llx]\n", address);
    if (ret != 0)
    {
        log_error("Failed to allocate pages!\n");
        return -ENOMEM;
    }
    pHeap = (heap_t *)address;
    pHeap->flags = HEAP_EMPTY;
    pHeap->size = heap_size;
    pHeap->next = nullptr;
    pHeap->checksum = 0;
    pHeap->checksum = getsum((uint8_t *)pHeap, sizeof(heap_t));
    return 0;
}

int destroy_memorymanager()
{
    return 0;
}