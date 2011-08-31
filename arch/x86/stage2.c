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
    init_multitask();
    // clrscr();
    __asm__("sti");
    thread_t tid;
    CreateThread(&tid, 0, &t1, (void*) 0xBADA55);
    while (1)
    {
        __asm__("pause");
    }
}

void* t1(void* arg)
{
    while (1)
    {
        __asm__("pause");
    }
}