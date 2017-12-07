/*
 * Copyright 2009-2012 Srijan Kumar Sharma
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
#define KERNEL_DRV_HEAP_PTR ((void*)0xF0000000)

#define DBG_OUTPUT printf("\nLine %d, function %s(), file %s",__LINE__,__FUNCTION__,__FILE__);

#include <stdint.h>
#include <stddef.h>
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
    llnode* current_thread;
} __attribute__((packed)) system_info_t;

extern volatile system_info_t sys_info;

static inline void outb(unsigned short port, unsigned char val)
{
    __asm__ volatile( "outb %0, %1"
            : : "a"(val), "Nd"(port));
}

static inline void outl(unsigned short port, uint32_t val)
{
    __asm__ volatile( "outl %0, %1"
            : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile( "inb %1, %0"
            : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline uint16_t inw(uint16_t port)
{
    uint16_t ret;
    __asm__ volatile( "inw %1, %0"
            : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline uint32_t inl(uint16_t port)
{
    uint32_t ret;
    __asm__ volatile( "inl %1, %0"
            : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void insl(uint16_t port, void* addr, uint32_t count)
{
    __asm__ __volatile__("rep ; insl" : "=D"(addr), "=c"(count) : "d"(port), "0"(addr), "1"(count));
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

/*Functions defined in asm*/
extern "C"
{
uint32_t get_spin_lock(void* lock_ptr);
uint32_t get_async_spin_lock(void* lock_ptr);
uint32_t release_spin_lock(void* lock_ptr);

void stage2(void);
void switch_context(uint32_t esp);
/*
 * gets the current previlage leval.
 */
uint32_t get_cpl(void);
uint32_t* get_cr3(void);
uint32_t get_eflags(void);
}
void state_c0(void);
#define LOGHEX(x) printf("\n\"" #x "\" :[0x%x]",x);
#endif
