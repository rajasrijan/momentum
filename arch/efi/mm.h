#ifndef MM_H
#define MM_H

#include <stdint.h>
#include <arch/efi/efi.h>

int initilize_memorymanager(EFI_SYSTEM_TABLE *table);
int destroy_memorymanager();

#endif /* MM_H */