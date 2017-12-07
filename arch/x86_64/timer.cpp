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

#include <stdint.h>
#include "string.h"
#include "stddef.h"
#include "timer.h"
#include "global.h"
#include "multitask.h"
#include "apic.h"
#include "../kernel/lists.h"
#include "../../libc/threads.h"
#include "interrupts.h"

static volatile uint64_t tick = 0;

static inline void push(uint32_t **ptr, uint32_t value)
{
	(*ptr)--;
	*ptr[0] = value;
}

static void print_gdt(void)
{
	gdt_ptr_t gptr;
	__asm__("sgdt [%0]" ::"r"(&gptr));
	printf("\nGDT Base:     %x", gptr.base);
	printf("\nGDT Limit:    %x", gptr.limit);
	printf("\nGDT pointer %x", sys_info.gdt_ptr->gp.base);
	uint16_t no = (uint16_t)(gptr.limit + 1);
	no = (uint16_t)(no / sizeof(gdt_entry_t));
	printf("\n*****************************************************************");
	for (uint16_t i = 0; i < no; i++)
	{
		gdt_entry_t *gent = (gdt_entry_t *)gptr.base;
		printf("\nGDT base:%x", (uint32_t)(gent[i].base_high << 16) + (uint32_t)(gent[i].base_middle << 8) + (uint32_t)(gent[i].base_low));
		printf("\nGDT limit:%x", (uint32_t)(gent[i].limit_low));
		printf("\nGDT gran:%x", (uint32_t)(gent[i].granularity));
		printf("\nGDT accessed:%x", (uint32_t)(gent[i].access));
		printf("\n*****************************************************************");
	}
}

void apic_timer_callback(retStack_t *stack, general_registers_t *regs)
{
	tick++;
	const thread_info &next_thread = multitask::getInstance()->getNextThread(stack, regs);
	eoi();
	change_thread(next_thread);
}

void sleep(int64_t delay)
{
	for (int64_t start_tick = tick; (tick - start_tick) < delay;)
	{
		__asm__("hlt");
	}
}

uint64_t getSystemTime()
{
	return tick;
}