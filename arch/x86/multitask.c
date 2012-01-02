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
    null_thread->task_id = getrandom();
    null_thread->thread_id = getrandom();
    /*
     * FIXME: get rid of.sizeof (general_registers_t) + sizeof(uint32_t)
     */
    null_thread->context.err_esp = (uint32_t) CreateStack() + 0x100000 - sizeof (iret_stack_same_t);
    null_thread->context.iret_stack.same.eip = (uint32_t) state_c0;
    null_thread->context.iret_stack.same.eflags = get_eflags();
    null_thread->context.ds = 0x10;
    null_thread->context.iret_stack.same.cs = 0x08;
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
    thread->task_id = getrandom();
    thread->thread_id = getrandom();
    release_spin_lock(&(sys_info.task_list_mutex));
    return 0;
}

void* CreateStack()
{
    uint32_t* esp = 0;
    for (uint32_t i = 1; i < 16; i++)
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

/*
 * Changes current thread to new thread with context reg.
 * also enables interrupts (clears NT bit in 'eflags' set by 'cli')
 */
void change_thread(registers_t *reg)
{
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
    get_spin_lock(&(sys_info.task_list_mutex));
    linked_list_t *cursor = sys_info.thread_list;
    sys_info.thread_list = cursor->next;
    cursor->prev->next = cursor->next;
    cursor->next->prev = cursor->prev;
    free(cursor->pointer);
    free(cursor);
    cursor = sys_info.thread_list;
    __asm__("cli");
    release_spin_lock(&(sys_info.task_list_mutex));
    change_thread(&((thread_info_t*) (cursor->pointer))->context);
}

