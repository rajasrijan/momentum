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

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#if __STDC_HOSTED__ != 0
#error "Hosted environment"
#endif


#define KERNEL_CODE_PTR ((void*)0xC0000000)
#define KERNEL_STACK_PTR ((void*)0xCC000000)
#define KERNEL_HEAP_PTR ((void*)0xD0000000)
#define KERNEL_DRIVER_PTR ((void*)0xE0000000)
#define KERNEL_PROCESS_PTR ((void*)0xF0000000)

#define DBG_OUTPUT printf("\nLine %d, function %s(), file %s\n",__LINE__,__FUNCTION__,__FILE__);

#include <stdint.h>
#include "descriptor_tables.h"
#include "../../libc/stdio.h"
extern void* _kalloc(uint32_t size, uint32_t align);
extern void* memory_map;
extern uint32_t memory_map_len;

static inline void outb(unsigned short port, unsigned char val)
{
    __asm__ volatile( "outb %0, %1"
            : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile( "inb %1, %0"
            : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline uint8_t checksum(uint8_t *ptr, uint32_t len)
{
    uint8_t res = 0;
    for (int i = 0; i < len; i++)
        res = (uint8_t) (res + ptr[i]);
    return res;
}

static inline uint8_t getsum(uint8_t *ptr, uint32_t len)
{
    uint8_t res = 0;
    for (int i = 0; i < len; i++)
        res = (uint8_t) (res + ptr[i]);
    return (uint8_t) (-res);
}

#endif
