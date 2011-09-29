/*
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

