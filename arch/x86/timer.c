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

#include <stdint.h>
#include "string.h"
#include "stddef.h"
#include "timer.h"
#include "global.h"
#include "multitask.h"
#include "apic.h"

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
    tick++;
    thread_info_t *thread = 0;
    task_info_t *task = 0;
    /*
     * Save context.
     */
    __asm__ volatile("xchg %%bx,%%bx;"::);
    __asm__ volatile("mov %%fs:(0x0),%0;"
            : "=r"(thread)
            );
    __asm__ volatile("mov %%fs:(0x4),%0;"
            : "=r"(task)
            );
    //print_gdt();
    printf("\nThread address(%x)", thread);
    printf("\nNext thread (%x)", thread->thread_id);
    printf("\nNext thread (%x)", thread->next_thread);
    thread->context = *reg;
    /*
     * Get next thread.
     */

    if (thread->next_thread != 0)
    {
        thread = thread->next_thread;
    }
    else
    {
        task = task->next_task;
        thread = task->threads;
    }
    printf("\nThread address(%x)", thread);
    printf("\nNext thread (%x)", thread->thread_id);
    printf("\nNext thread (%x)", thread->next_thread);
    DBG_OUTPUT
    __asm__ volatile("mov %0,%%fs:(0x0);" ::"r"(thread));
    DBG_OUTPUT
    __asm__ volatile("mov %0,%%fs:(0x4);" ::"r"(task));
    DBG_OUTPUT

            /*
             * Setup proper stack.
             * current privilege level is ring 0.
             */
    switch ((reg->cs) & 0x3)
    {
    case 0:
    {
        /*
         * Interrupt to same privilege.
         * 'ESP' is not saved. We have to change it manually.
         */
        DBG_OUTPUT
        thread->context.eflags |= 0x200;
        DBG_OUTPUT
        uint32_t *esp = (uint32_t*) thread->context.esp;
        DBG_OUTPUT
        esp -= 9;
        DBG_OUTPUT
        memcpy((void*) esp, (void*) &(thread->context), offsetof(registers_t, useresp));
        DBG_OUTPUT
        __asm__("xchg %%bx,%%bx;"
                "mov %0,%%esp;"
                "pop %%eax;"
                "mov %%ax,%%ds;"
                "mov %%ax,%%es;"
                "mov %%ax,%%gs;"
                "movl $0x0,(%1);"// Send EOI to Local APIC eoi register
                "popa;"
                "add $8,%%esp;"
                "sti;" //Not executed till iret.
                "iret;"
                :
                : "r"(esp), "r"(&(sys_info.local_apic->eoi))
                );
        /*
        
                push(&esp, thread->context.eflags);
                push(&esp, thread->context.cs);
                push(&esp, thread->context.eip);

        
                push(&esp, thread->context.eax);
                push(&esp, thread->context.ecx);
                push(&esp, thread->context.edx);
                push(&esp, thread->context.ebx);
                push(&esp, thread->context.esp);
                push(&esp, thread->context.ebp);
                push(&esp, thread->context.esi);
                push(&esp, thread->context.edi);
                __asm__ volatile("xchg %%bx,%%bx;"
                        "mov %0,%%esp;"
                        "popa;"
                        "iret;"
                        : "=r"(esp)
                        );
         */
        break;
    }
    }
    DBG_OUTPUT
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
