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

#include "global.h"
#include "timer.h"
#include "mm.h"
#include "paging.h"
#include "acpi.h"
#include "apic.h"
#include "stdlib.h"
#include "cpu.h"
#include "keyboard.h"
#include "multitask.h"
#include <stdio.h>

void stage2(void);
void* t1(void* arg);
extern uint32_t stack;

void stage2(void)
{
    init_idt();
    init_paging();
    initilize_memorymanager();
    create_kernel_heap();
    fix_refferances();

    if (!get_acpi_tables())
        __asm__("cli;hlt;");
    init_apic_timer(0x0FFFFFF);
    //init_keyboard();
    init_multitask();

    // clrscr();

    //thread_t tid;
    change_thread(&(((thread_info_t*) (sys_info.thread_list->pointer))->context));
}

void* t1(void* arg)
{
    __asm__("xchg %%bx,%%bx"::);
    printf("\nArgument is %x", arg);
}

void state_c0()
{
    printf("some string");
    uint32_t tid;
    CreateThread(&tid, 0, &t1, (void*) 0xBADA55);
    while (1)
    {
        __asm__("pause");
    }
}

