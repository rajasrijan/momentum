/* 
 * File:   mm.h
 * Author: Srijan
 *
 * Created on June 21, 2011, 12:29 PM
 */

#ifndef MM_H
#define	MM_H
#include "multiboot.h"
#include <stdint.h>

typedef struct _heap_
{
    uint32_t size;
#define	    HEAP_EMPTY  (0)
#define	    HEAP_FULL   (1)
    uint8_t flags;
    uint8_t checksum;
} heap_t;
extern uint64_t total_ram;
void coax_multiboot(void *mbd);
void initilize_memorymanager(void);
void create_kernel_heap(void);
uint32_t get_4mb_block(void);
int IsMemoryReserved(uint32_t mem_addr);
void* _malloc(uint32_t length);
/*
 * Free allovated memory
 * Syntax:
 * void free(void* ptr);
 * ptr == pointer to memory to be freed.
 */
void _free(void *ptr);
#endif	/* MM_H */

