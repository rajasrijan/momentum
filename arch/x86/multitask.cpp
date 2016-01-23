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
#include "timer.h"

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
    create_linked_list((linked_list_t**)&(sys_info.thread_list), sizeof (thread_info_t));
    thread_info_t kernel_thread;
    memset(&kernel_thread, 0, sizeof (kernel_thread));
    kernel_thread.ProcessID = getrandom();
    kernel_thread.ThreadID = getrandom();

    /*sys_info.thread_list = calloc(1, sizeof (linked_list_t));
    null_thread_list = sys_info.thread_list;
    sys_info.thread_list->pointer = calloc(1, sizeof (thread_info_t));
    null_thread = sys_info.thread_list->pointer;
    sys_info.thread_list->next = sys_info.thread_list;
    sys_info.thread_list->prev = sys_info.thread_list;
    null_thread->ProcessID = getrandom();
    null_thread->ThreadID = getrandom();*/
    /*     
     * FIXME: get rid of.sizeof (general_registers_t) + sizeof(uint32_t)
     */
    get_spin_lock(&(kernel_thread.flags));
    kernel_thread.context.err_esp = (uint32_t) CreateStack() + 0x100000 - sizeof (iret_stack_same_t);
    kernel_thread.context.iret_stack.same.eip = (uint32_t) state_c0;
    kernel_thread.context.iret_stack.same.eflags = get_eflags();
    kernel_thread.context.ds = 0x10;
    kernel_thread.context.iret_stack.same.cs = 0x08;
    kernel_thread.page_table = (vector_list_t*) calloc(1, sizeof (vector_list_t));
    vector_init_fn(kernel_thread.page_table, sizeof (pt_cache_unit_t));
    pt_cache_unit_t unit = {0};
    for (uint32_t i = 0; i < 1024; i++)
        if ((sys_info.pst->page_directory[i]&1) == 1)
            for (uint32_t j = 0; j < 1024; j++)
            {
                if ((unit.paddress_flags & 0xFFF) != (sys_info.pst->page_table[(i * 1024) + j]&0xFFF))
                {
                    if ((unit.paddress_flags & 0x1) == 1)
                        kernel_thread.page_table->push(kernel_thread.page_table, &unit);
                    unit.no_of_pages = 1;
                    unit.start_page = (i * 1024) + j;
                    unit.paddress_flags = sys_info.pst->page_table[(i * 1024) + j];

                }
                else
                {
                    unit.no_of_pages++;
                }
            }
    addto_linked_list(sys_info.thread_list, &kernel_thread);
    sys_info.current_thread = (llnode*) getelement_linked_list(sys_info.thread_list, 0);
    //print_thread_list();
}

int CreateThread(uint32_t *thd, const uint32_t attr, void *((*start_routine)(void*)), void *arg)
{
    disableThreadSwitching();
    //linked_list_t *cursor = sys_info.thread_list;
    //linked_list_t *newNode = calloc(1, sizeof (linked_list_t));
    //((thread_info_t*) newNode)->flags = THREAD_BUSY;
    //newNode->next = cursor->next;
    //newNode->prev = cursor;
    //cursor->next = newNode;
    //newNode->next->prev = newNode;
    //cursor = cursor->next;
    thread_info_t thread;
    memset(&thread, 0, sizeof (thread));
    //thread.flags=THREAD_BUSY;
    thread.context.ds = 0x10;
    thread.context.err_esp = (uint32_t) CreateStack() + 0x100000;
    thread.context.err_esp -= 4;
    * ((uint32_t*) (thread.context.err_esp)) = (uint32_t) arg;
    thread.context.err_esp -= 4;
    * ((uint32_t*) (thread.context.err_esp)) = (uint32_t) thread_end;
    thread.context.err_esp -= sizeof (iret_stack_same_t);
    thread.context.iret_stack.same.cs = 0x08;
    thread.context.iret_stack.same.eflags = get_eflags();
    thread.context.iret_stack.same.eip = (uint32_t) start_routine;
    thread.ProcessID = ((thread_info_t*) (sys_info.current_thread->ptr))->ProcessID;
    thread.ThreadID = getrandom();
    //printf("\nCreating P:%x,T:%x",thread->ProcessID,thread->ThreadID );
    thread.page_table = ((thread_info_t*) (sys_info.current_thread->ptr))->page_table;

    addto_linked_list(sys_info.thread_list, &thread);
    //print_thread_list();

    enableThreadSwitching();
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
    stack_map = (uint16_t) (stack_map & ((uint16_t) esp));
}

/*
 * Changes current thread to new thread with context reg.
 * also enables interrupts (clears NT bit in 'eflags' set by 'cli')
 */
void change_thread(thread_info_t* thread)
{
    for (uint32_t i = 0; i < thread->page_table->size(thread->page_table); i++)
    {
        pt_cache_unit_t *unit = (pt_cache_unit_t*) thread->page_table->at(thread->page_table, i);
        for (uint32_t j = 0; j < unit->no_of_pages; j++)
        {
            sys_info.pst->page_table[unit->start_page + j] = unit->paddress_flags + (j * 0x1000);
        }
    }
    registers_t *reg = &(thread->context);
    uint32_t esp = reg->err_esp;
    /*
     * set maskable interrupt flag.
     */
    reg->iret_stack.same.eflags |= 0x200;
    //printf("\n%x",reg->iret_stack.same.cs );
    if (((reg->iret_stack.same.cs) >> 3) == 0)
    {
        printf("\nInvalid Code section.Halting...");
        __asm__("cli;"
                "hlt;"
                );
    }
    if ((reg->iret_stack.same.cs & 3) == 0)
    {
        //DBG_OUTPUT
        memcpy((char*) esp, (char*) &(reg->iret_stack.same), sizeof (iret_stack_same_t));
    }
    else
    {
        DBG_OUTPUT
        memcpy((char*) esp, (char*) &(reg->iret_stack.different), sizeof (iret_stack_different_t));
    }
    esp -= sizeof (general_registers_t);
    memcpy((char*) esp, (char*) &(reg->greg), sizeof (general_registers_t));
    esp -= 4;
    *((uint32_t*) esp) = reg->ds;

    switch_context(esp);
}

void thread_end()
{
    //uint32_t esp = ((thread_info_t*) sys_info.current_thread->ptr)->context.greg.esp;
    //removefrm_linked_list(sys_info.thread_list, sys_info.current_thread->ptr);
    //__asm__("cli;");
    //sys_info.current_thread = 0;
    printf("\nLooping till resheduled.");
    ((thread_info_t*) sys_info.current_thread->ptr)->flags |= THREAD_STOP;
    //DestroyStack(esp);
    while (1)
    {
        __asm__(
                "sti;"
                "hlt;"
                );
    }
}

thread_info_t* getNextThreadInQueue()
{
    /*linked_list_t *cursor = sys_info.thread_list;
    while (get_async_spin_lock(&(((thread_info_t*) cursor->pointer)->flags)))
    {
        cursor = cursor->next;
        if (sys_info.thread_list == cursor)
            return null_thread;
    }
    return ((thread_info_t*) cursor->pointer);*/
    return 0;
}

llnode* getNextThreadListInQueue()
{
    llnode* node = sys_info.current_thread;
    thread_info_t *last_thread = ((thread_info_t*) sys_info.current_thread->ptr);

    while (get_async_spin_lock(&(((thread_info_t*) (node->ptr))->flags)))
    {
        if (node == sys_info.thread_list->last_node)
            node = sys_info.thread_list->first_node;
        else
            node = (llnode*) node->next;
        if (node == sys_info.current_thread)
            return node;
    }
    /*
     *  Release last thread.
     */
    release_spin_lock(&(last_thread->flags));
    return (node);
}

int CreateNullProcess()
{
    return 0;
}

int CreateProcess()
{
    return 0;
}