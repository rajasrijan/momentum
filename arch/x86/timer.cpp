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
#include "../../libc/threads.h"

static volatile uint32_t tick = 0;
static volatile mtx_t threadEnable;

static inline void push(uint32_t** ptr, uint32_t value)
{
    (*ptr)--;
    *ptr[0] = value;
}

void print_thread_list()
{
    llnode* i = sys_info.thread_list->first_node;
    printf("\n");
    do
    {
        thread_info_t *thread = (thread_info_t*) (i->ptr);
        printf("(%x,%x,%x)>", thread->ThreadID, thread->context.iret_stack.same.cs, (uint32_t) (thread->context.ds)&0xFF);
        i = (llnode*) i->next;
    }
    while (i != NULL);
}

static void print_gdt(void)
{
    gdt_ptr_t gptr;
    __asm__("sgdt (%0)"::"r"(&gptr));
    printf("\nGDT Base:     %x", gptr.base);
    printf("\nGDT Limit:    %x", gptr.limit);
    printf("\nGDT pointer %x", sys_info.gdt_ptr->gp.base);
    uint16_t no = (uint16_t) (gptr.limit + 1);
    no = (uint16_t)(no / sizeof (gdt_entry_t));
    printf("\n*****************************************************************");
    for (uint16_t i = 0; i < no; i++)
    {
        gdt_entry_t* gent = (gdt_entry_t*) gptr.base;
        printf("\nGDT base:%x", (uint32_t) (gent[i].base_high << 16) + (uint32_t) (gent[i].base_middle << 8) + (uint32_t) (gent[i].base_low));
        printf("\nGDT limit:%x", (uint32_t) (gent[i].limit_low));
        printf("\nGDT gran:%x", (uint32_t) (gent[i].granularity));
        printf("\nGDT accessed:%x", (uint32_t) (gent[i].access));
        printf("\n*****************************************************************");
    }
}

void apic_timer_callback(registers_t* reg)
{
    tick++;

    if (mtx_trylock((mtx_t*) & threadEnable) == thrd_success)
    {
        if (sys_info.current_thread == 0)
        {
            setColor(0x14);
            printf("\nInvalid entry. Halting...");
            __asm__("cli;hlt;");
        }

        ((thread_info_t*) (sys_info.current_thread->ptr))->context = *reg;
        thread_info_t *last_thread = ((thread_info_t*) sys_info.current_thread->ptr);
        sys_info.current_thread = getNextThreadListInQueue();
        if (last_thread->flags & THREAD_STOP)
        {

            removefrm_linked_list(sys_info.thread_list, last_thread);

            /*setColor(0x14);
            printf("\nStopping. Halting...");
            __asm__("cli;hlt;");*/
        }

        send_eoi();
        thread_info_t *next_thread = ((thread_info_t*) (sys_info.current_thread->ptr));
        //printf("\nSwitching P:%x,T:%x", next_thread->ProcessID, next_thread->ThreadID);
        //print_thread_list();
        mtx_unlock((mtx_t*) & threadEnable);
        change_thread((thread_info_t*) (sys_info.current_thread->ptr));
    }
}

void init_timer()
{
    mtx_init((mtx_t*) & threadEnable, 0);
}

void sleep(uint32_t delay)
{
    for (uint32_t start_tick = tick; (tick - start_tick) < delay;);
}

uint32_t getSystemTime()
{
    return tick;
}

void disableThreadSwitching()
{
    mtx_lock((mtx_t*) & threadEnable);
}

void enableThreadSwitching()
{
    mtx_unlock((mtx_t*) & threadEnable);
}