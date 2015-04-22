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
#include "stdio.h"
#include "../../kernel/vfs.h"
#include "pci.h"

void stage2(void);
void* t1(void* arg);
extern uint32_t stack;

void stage2(void)
{
    clrscr();
    DBG_OUTPUT
    init_idt();
    DBG_OUTPUT
    init_paging();
    DBG_OUTPUT
    initilize_memorymanager();
    DBG_OUTPUT
    create_kernel_heap();
    DBG_OUTPUT
    fix_refferances();
    DBG_OUTPUT
    if (!get_acpi_tables())
        __asm__("cli;hlt;");
    DBG_OUTPUT
    init_apic_timer(0x00FFFFF);
    DBG_OUTPUT
    init_keyboard();
    DBG_OUTPUT
    init_multitask();
    DBG_OUTPUT
    init_vfs();
    DBG_OUTPUT
    init_pci_devices();
    // clrscr();
    //thread_t tid;
    /*
     * change_thread enables interrupts and changes execution to state_c0()
     */
    DBG_OUTPUT
    change_thread((thread_info_t*) (sys_info.thread_list->pointer));
}

void* t1(void* arg)
{
    printf("\nWelcome to MomentumOS (c)\nBy Srijan Kumar Sharma.");
}

/*
 * kernel thread
 * wish there were naked functions in gcc.
 */
void state_c0()
{
    printf("\nKernal thread started.");
    //init_drivers();
    uint32_t tid;
    CreateThread(&tid, 0, &t1, (void*) 0);
    __asm__("sti");
    while (1)
    {
        __asm__("pause;"
                "hlt;");
    }
}
