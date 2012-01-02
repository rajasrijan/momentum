/*
 * Copyright 2009-2011 Srijan Kumar Sharma
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

#ifndef _GLOBAL_H_
#define _GLOBAL_H_


#define KERNEL_CODE_PTR ((void*)0xC0000000)
#define KERNEL_STACK_PTR ((void*)0xCC000000)
#define KERNEL_HEAP_PTR ((void*)0xD0000000)
#define KERNEL_DRIVER_PTR ((void*)0xE0000000)
#define KERNEL_PROCESS_PTR ((void*)0xF0000000)

#define DBG_OUTPUT printf("\nLine %d, function %s(), file %s\n",__LINE__,__FUNCTION__,__FILE__);

#include <stdint.h>
#include "descriptor_tables.h"
#include "paging.h"
#include "acpi.h"
#include "apic.h"
#include "../../libc/stdio.h"
#include "../../kernel/lists.h"

extern void* _kalloc(uint32_t size, uint32_t align);
extern void* memory_map;
extern uint32_t memory_map_len;

typedef struct system_info
{
    uint32_t no_of_cpus;
    gdt_t* gdt_ptr;
    paging_structure_t* pst;
    void* memory_map;
    uint32_t memory_map_len;
    uint32_t kernel_end;
    lapic_t *local_apic;
    idt_t *idt_ptr;
    acpi_rsdp_t* rsdp;
    linked_list_t *thread_list;
    uint32_t task_list_mutex;
} __attribute__((packed)) system_info_t;

system_info_t sys_info;

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

void* __attribute__((stdcall)) alined_alloc(uint32_t size, uint32_t alignment);
uint32_t get_eflags(void);
/*
 * gets the current previlage leval.
 */
uint32_t get_cpl(void);
void switch_context(uint32_t esp);
void get_spin_lock(void* lock_ptr);
void release_spin_lock(void* lock_ptr);
void state_c0(void);
#endif
