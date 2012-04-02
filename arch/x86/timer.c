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

#include <stdint.h>
#include "string.h"
#include "stddef.h"
#include "timer.h"
#include "global.h"
#include "multitask.h"
#include "apic.h"
#include "../kernel/lists.h"

static inline void push(uint32_t** ptr, uint32_t value)
{
    (*ptr)--;
    *ptr[0] = value;
}

static void print_gdt(void)
{
    gdt_ptr_t gptr;
    __asm__("sgdt (%0)"::"r"(&gptr));
    printf("\nGDT Base:     %x", gptr.base);
    printf("\nGDT Limit:    %x", gptr.limit);
    printf("\nGDT pointer %x", sys_info.gdt_ptr->gp.base);
    uint16_t no = (uint16_t) (gptr.limit + 1);
    no /= sizeof (gdt_entry_t);
    printf("\n*****************************************************************");
    for (uint16_t i = 0; i < no; i++)
    {
        gdt_entry_t* gent = (void*) gptr.base;
        printf("\nGDT base:%x", (uint32_t) (gent[i].base_high << 16) + (uint32_t) (gent[i].base_middle << 8) + (uint32_t) (gent[i].base_low));
        printf("\nGDT limit:%x", (uint32_t) (gent[i].limit_low));
        printf("\nGDT gran:%x", (uint32_t) (gent[i].granularity));
        printf("\nGDT accessed:%x", (uint32_t) (gent[i].access));
        printf("\n*****************************************************************");
    }
}

void apic_timer_callback(registers_t* reg)
{
    static uint32_t tick = 0;
    get_spin_lock(&(sys_info.task_list_mutex));
    ((thread_info_t*) (sys_info.thread_list->pointer))->context = *reg;
    sys_info.thread_list = sys_info.thread_list->next;
    release_spin_lock(&(sys_info.task_list_mutex));
    send_eoi();

    change_thread((thread_info_t*) (sys_info.thread_list->pointer));
}

void init_timer(uint32_t frequency)
{
    // Firstly, register our timer callback.
    //register_interrupt_handler(IRQ(0), &timer_callback);
    /*
    // The value we send to the PIT is the value to divide it's input clock
    // (1193180 Hz) by, to get our required frequency. Important to note is
    // that the divisor must be small enough to fit into 16-bits.
    uint32_t divisor = 1193180 / frequency;

    // Send the command byte.
    outb(0x43, 0x36);

    // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
    uint8_t l = (uint8_t) (divisor & 0xFF);
    uint8_t h = (uint8_t) ((divisor >> 8) & 0xFF);

    // Send the frequency divisor.
    outb(0x40, l);
    outb(0x40, h);
     */
}
