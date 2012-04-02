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

#include "multitask.h"
#include "descriptor_tables.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "mm.h"
#include "paging.h"
#include "global.h"
#include "../kernel/lists.h"

static uint16_t stack_map = 0;

static uint32_t getrandom(void)
{
    static uint32_t local_random_generator = 0;
    local_random_generator++;
    if (local_random_generator == 0)
        local_random_generator++;
    return local_random_generator;
}

void init_multitask()
{
    sys_info.thread_list = calloc(1, sizeof (linked_list_t));
    sys_info.thread_list->pointer = calloc(1, sizeof (thread_info_t));
    thread_info_t *null_thread = sys_info.thread_list->pointer;
    sys_info.thread_list->next = sys_info.thread_list;
    sys_info.thread_list->prev = sys_info.thread_list;
    null_thread->ProcessID = getrandom();
    null_thread->ThreadID = getrandom();
    /*
     * FIXME: get rid of.sizeof (general_registers_t) + sizeof(uint32_t)
     */
    null_thread->context.err_esp = (uint32_t) CreateStack() + 0x100000 - sizeof (iret_stack_same_t);
    null_thread->context.iret_stack.same.eip = (uint32_t) state_c0;
    null_thread->context.iret_stack.same.eflags = get_eflags();
    null_thread->context.ds = 0x10;
    null_thread->context.iret_stack.same.cs = 0x08;
    null_thread->page_table = calloc(1, sizeof (vector_list_t));
    vector_init_fn(null_thread->page_table, sizeof (pt_cache_unit_t));
    pt_cache_unit_t unit = {0};
    for (uint32_t i = 0; i < 1024 * 1024; i++)
    {
        if ((unit.paddress_flags & 0xFFF) != (sys_info.pst->page_table[i] & 0xFFF))
        {
            if ((unit.paddress_flags & 0x1) == 1)
                printf("\n%x->%x", unit.start_page * 0x1000, (unit.start_page + unit.no_of_pages)*0x1000);
            unit.start_page = i;
            unit.no_of_pages = 1;
            unit.paddress_flags = sys_info.pst->page_table[i];
        }
        else
        {
            unit.no_of_pages++;
        }
    }
    sys_info.task_list_mutex = 0;
}

int CreateThread(uint32_t *thd, const uint32_t attr, void *((*start_routine)(void*)), void *arg)
{
    get_spin_lock(&(sys_info.task_list_mutex));
    linked_list_t *cursor = sys_info.thread_list;
    cursor->next->prev = calloc(1, sizeof (linked_list_t));
    cursor->next->prev->next = cursor->next;
    cursor->next = cursor->next->prev;
    cursor->next->prev = cursor;
    cursor = cursor->next;
    thread_info_t *thread = cursor->pointer = calloc(1, sizeof (thread_info_t));
    thread->context.ds = 0x10;
    thread->context.err_esp = (uint32_t) CreateStack() + 0x100000;
    thread->context.err_esp -= 4;
    *((uint32_t*) (thread->context.err_esp)) = (uint32_t) arg;
    thread->context.err_esp -= 4;
    *((uint32_t*) (thread->context.err_esp)) = (uint32_t) thread_end;
    thread->context.err_esp -= sizeof (iret_stack_same_t);
    thread->context.iret_stack.same.cs = 0x08;
    thread->context.iret_stack.same.eflags = get_eflags();
    thread->context.iret_stack.same.eip = (uint32_t) start_routine;
    thread->ProcessID = getrandom();
    thread->ThreadID = getrandom();
    //thread->page_table = ((thread_info_t*) cursor->pointer)->page_table;
    release_spin_lock(&(sys_info.task_list_mutex));
    return 0;
}

void* CreateStack()
{
    uint32_t* esp = 0;
    for (uint32_t i = 0; i < 16; i++)
    {
        if ((stack_map & (1 << i)) == 0)
        {
            stack_map = (uint16_t) (stack_map | (1 << i));
            break;
        }
        else
        {
            esp += 0x100000;
        }
    }
    esp = (uint32_t*) ((uint32_t) esp + (uint32_t) KERNEL_STACK_PTR);
    map_4mb((uint32_t) esp);
    return esp;
}

void DestroyStack(uint32_t esp)
{
    esp -= (uint32_t) KERNEL_STACK_PTR;
    esp /= 0x100000;
    esp = !(uint32_t) (1 << esp);
    stack_map &= (uint16_t) esp;
}

/*
 * Changes current thread to new thread with context reg.
 * also enables interrupts (clears NT bit in 'eflags' set by 'cli')
 */
void change_thread(thread_info_t* thread)
{
    /*
        for (uint32_t i = 0; i < thread->page_table->size(thread->page_table); i++)
        {
            pt_cache_unit_t *unit = thread->page_table->at(thread->page_table, i);
            for (uint32_t j = 0; j < unit->no_of_pages; j++)
            {
                sys_info.pst->page_table[unit->start_page + j] = unit->paddress_flags + (j * 0x1000);
            }
        }
     */
    registers_t *reg = &(thread->context);
    uint32_t esp = reg->err_esp;
    /*
     * set maskable interrupt flag.
     */
    reg->iret_stack.same.eflags |= 0x200;
    if ((reg->iret_stack.same.cs & 3) == 0)
    {
        memcpy((void*) esp, (void*) &(reg->iret_stack.same), sizeof (iret_stack_same_t));
    }
    else
    {
        memcpy((void*) esp, (void*) &(reg->iret_stack.different), sizeof (iret_stack_different_t));
    }
    esp -= sizeof (general_registers_t);
    memcpy((void*) esp, (void*) &(reg->greg), sizeof (general_registers_t));
    esp -= 4;
    *((uint32_t*) esp) = reg->ds;
    switch_context(esp);
}

void thread_end()
{
#warning mutex needed;
    get_spin_lock(&(sys_info.task_list_mutex));
    linked_list_t *cursor = sys_info.thread_list;
    sys_info.thread_list = cursor->next;
    cursor->prev->next = cursor->next;
    cursor->next->prev = cursor->prev;
    __asm__("cli;");
    DestroyStack(((thread_info_t*) cursor->pointer)->context.greg.esp);
    free(cursor->pointer);
    free(cursor);
    cursor = sys_info.thread_list;
    release_spin_lock(&(sys_info.task_list_mutex));
    change_thread((thread_info_t*) (cursor->pointer));
}

