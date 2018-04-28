/*
 * Copyright 2009-2017 Srijan Kumar Sharma
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

#define KERNEL_CODE_PTR ((void *)0x00000000C0000000)
#define KERNEL_STACK_SZ (0x0000000000200000)
#define KERNEL_HEAP_PTR ((void *)0x00000000D0000000)
#define KERNEL_DRIVER_PTR ((void *)0x00000000E0000000)
#define KERNEL_DRV_HEAP_PTR ((void *)0x00000000F0000000)

#include <stdint.h>
#include <stddef.h>
#include "descriptor_tables.h"
#include "paging.h"
#include "acpi.h"
#include "apic.h"
#include <stdio.h>
#include "../../kernel/lists.h"
#include "multiboot.h"
#include "multitask.h"
#include <list>

extern void *memory_map;
extern uint32_t memory_map_len;

typedef struct system_info
{
	uint32_t no_of_cpus;
	gdt_t *gdt_ptr;
	void *memory_map;
	uint32_t memory_map_len;
	uint32_t kernel_end;
	local_apic_structure *local_apic;
	idt_t idt;
	acpi_rsdp *rsdp;
	uint64_t root_drive_uuid;
} __attribute__((packed)) system_info_t;

extern system_info_t sys_info;

static inline void outb(unsigned short port, unsigned char val)
{
    __asm__ volatile( "out %1, %0"
            : : "a"(val), "Nd"(port));
}

static inline void outl(unsigned short port, uint32_t val)
{
	__asm__ volatile("out %1, %0"
					 :
					 : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port)
{
	uint8_t ret;
	__asm__ volatile("in %0, %1"
					 : "=a"(ret)
					 : "Nd"(port));
	return ret;
}

static inline uint16_t inw(uint16_t port)
{
	uint16_t ret;
	__asm__ volatile("in %0, %1"
					 : "=a"(ret)
					 : "Nd"(port));
	return ret;
}

static inline uint32_t inl(uint16_t port)
{
	uint32_t ret;
	__asm__ volatile("in %0, %1"
					 : "=a"(ret)
					 : "Nd"(port));
	return ret;
}

static inline void insl(uint16_t port, void *addr, uint32_t count)
{
	__asm__ __volatile__("rep ; insl"
						 : "=D"(addr), "=c"(count)
						 : "d"(port), "0"(addr), "1"(count));
}

static inline uint8_t checksum(uint8_t *ptr, uint32_t len)
{
	uint8_t res = 0;
	for (int i = 0; i < len; i++)
		res = (uint8_t)(res + ptr[i]);
	return res;
}

static inline uint8_t getsum(uint8_t *ptr, uint32_t len)
{
	uint8_t res = 0;
	for (int i = 0; i < len; i++)
		res = (uint8_t)(res + ptr[i]);
	return (uint8_t)(-res);
}

void *alined_alloc(uint32_t size, uint32_t alignment);

/*Functions defined in asm*/
extern "C" {
uint64_t get_spin_lock(void *lock_ptr);
uint32_t get_async_spin_lock(void *lock_ptr);
uint32_t release_spin_lock(void *lock_ptr);

void stage2(multiboot_info *mbi);
void switch_context(uint64_t esp);
/*
	 * gets the current previlage leval.
	 */
uint32_t get_cpl(void);
uint32_t *get_cr3(void);
uint32_t *get_cr2(void);
uint64_t get_rflags(void);
}
void state_c0(void);
#define LOGHEX(x) printf("\n\"" #x "\" :[0x%x]", x);
#define LOGSTR(x) printf("\n\"" #x "\" :[%s]", x);
#define ROOT_UUID_NAME	"ROOT_UUID"
#endif
