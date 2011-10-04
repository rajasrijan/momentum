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
static void state_c0(void);
extern uint32_t stack;

void stage2(void)
{
    /*
     * Fix allocation address to higher half.
     */
    _kalloc(0xC0000000, 1);
    init_paging();
    initilize_memorymanager();

    create_kernel_heap();

    fix_refferances();
    if (!get_acpi_tables())
        __asm__("cli;hlt;");
    init_apic_timer(0x0FFFFFF);
    //init_keyboard();
    printf("\n%x", (& stack));
    uint32_t *esp = 0;
    __asm__("mov %%esp,%0" : "=r"(esp) : :);
    esp = (void*) ((uint32_t) CreateStack()+(uint32_t) esp - ((uint32_t) & stack) + 0xC0000000);
    __asm__("mov %0,%%esp" : : "r"(esp) :);
    init_multitask();

    // clrscr();
    __asm__("sti");
    thread_t tid;
    CreateThread(&tid, 0, &t1, (void*) 0xBADA55);
    printf("some string");
    while (1)
    {
        __asm__("pause");
    }
}

void* t1(void* arg)
{
    __asm__ volatile("xchg %%bx,%%bx;" :);
    printf("some string.");
    while (1)
    {
        __asm__("pause");
    }
}

static void state_c0()
{
    while (1)
    {
        __asm__("pause");
    }
}