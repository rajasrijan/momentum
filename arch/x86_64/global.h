/*
 * Copyright 2009-2019 Srijan Kumar Sharma
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
#include <list>

extern void *memory_map;
extern uint32_t memory_map_len;

static inline void insl(uint16_t port, void *addr, uint32_t count)
{
	__asm__ __volatile__("rep ; insl"
						 : "=D"(addr), "=c"(count)
						 : "d"(port), "0"(addr), "1"(count));
}

static inline uint8_t checksum(uint8_t *ptr, uint32_t len)
{
	uint8_t res = 0;
	for (uint32_t i = 0; i < len; i++)
		res = (uint8_t)(res + ptr[i]);
	return res;
}

static inline uint8_t getsum(uint8_t *ptr, uint32_t len)
{
	uint8_t res = 0;
	for (uint32_t i = 0; i < len; i++)
		res = (uint8_t)(res + ptr[i]);
	return (uint8_t)(-res);
}
static inline void rdmsr(uint32_t msr, uint32_t *lo, uint32_t *hi)
{
	asm volatile("rdmsr"
				 : "=a"(*lo), "=d"(*hi)
				 : "c"(msr));
}

static inline void wrmsr(uint32_t msr, uint64_t val)
{
	asm volatile("wrmsr"
				 :
				 : "a"(val & 0xFFFFFFFF), "d"((val >> 32) & 0xFFFFFFFF), "c"(msr));
}
/*Functions defined in asm*/
extern "C"
{
	uint64_t get_spin_lock(void *lock_ptr);
	uint32_t get_async_spin_lock(void *lock_ptr);
	uint32_t release_spin_lock(void *lock_ptr);

	void stage2(struct multiboot_information *mbi);
	void switch_context(uint64_t rsp, uint64_t ss);
	/*
	 * gets the current previlage leval.
	 */
	uint32_t get_cpl(void);
	uint32_t *get_cr3(void);
	uint32_t *get_cr2(void);
	uint64_t get_rflags(void);
	void get_gdt(void *ptr);
	void load_interrupt_descriptor_table(void *ptr);
	void outb(unsigned short port, unsigned char val);
	void outw(unsigned short port, uint16_t val);
	void outl(unsigned short port, uint32_t val);
	uint8_t inb(uint16_t port);
	uint32_t inl(uint16_t port);
	uint16_t inw(uint16_t port);
}
void state_c0(void);
#define LOGHEX(x) printf("\n\"" #x "\" :[0x%x]", x);
#define LOGSTR(x) printf("\n\"" #x "\" :[%s]", x);
#define ROOT_UUID_NAME "ROOT_UUID"
#endif
