/*
 * Copyright 2009-2021 Srijan Kumar Sharma
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

#include "apic.h"
#include "acpi.h"
#include "interrupts.h"
#include "timer.h"
#include <arch/x86_64/paging.h>
#include <kernel/sys_info.h>
ioapic_t volatile *ioapic;

/*
 * Initilize IO-APIC.
 */
void init_ioapic()
{
    /*
     * If there is a better way let me know.
     */
    int ret = 0;
    ioapic = (ioapic_t *)0xFEC00000;
    ret = PageManager::IdentityMap2MBPages(0xFEC00000);
    if (ret < 0) {
        printf("Failed to identity map pages\n");
        asm("cli;hlt");
    }
}

/*
 * Write to IO-APIC
 */
void write_ioapic(uint32_t offset, uint32_t value)
{
    ioapic->ioregsel = offset;
    ioapic->iowin = value;
}

/*
 * Read from IO-APIC.
 */
uint32_t read_ioapic(uint32_t offset)
{
    ioapic->ioregsel = offset;
    return ioapic->iowin;
}

/*
 * Enable an IO-APIC pin.
 */
void apic_pin_enable(uint32_t pin)
{
    write_ioapic(0x10 + (2 * pin), (read_ioapic(0x10 + (2 * pin)) & 0xFFFEFFFF));
}

static uint64_t tick = 0;
void simple_tick_counter(retStack_t *stack, general_registers_t *regs)
{
    tick++;
    eoi();
}
/*
 * Initilize APIC timer.
 */
void init_apic_timer(uint32_t frequency)
{
    outb(0x21, 0xFF);
    outb(0xA1, 0xFF);
    register_interrupt_handler(32, simple_tick_counter);
    sys_info.local_apic->lvt_timer = 0x00020020;
    sys_info.local_apic->div_conf = 0x02;
    sys_info.local_apic->init_count = frequency;
    register_interrupt_handler(32, apic_timer_callback);
}

/*
 * Send End of interrupt to APIC. INCOMPLETE.
 */
void send_eoi()
{
    sys_info.local_apic->eoi = 0;
}

void apic_interrupt_command_init(bool isShorthand, uint64_t dest)
{
    if (isShorthand) {
        sys_info.local_apic->icr_hi = 0;
        sys_info.local_apic->icr_lo = APIC_ICR_DELIVERY_INIT | APIC_ICR_DESTMODE_PHYSICAL | APIC_ICR_LEVEL_ASSERT | APIC_ICR_TRIGGER_EDGE | dest;
    } else {
        sys_info.local_apic->icr_hi = dest & 0xFF000000;
        sys_info.local_apic->icr_lo = APIC_ICR_DELIVERY_INIT | APIC_ICR_DESTMODE_PHYSICAL | APIC_ICR_LEVEL_ASSERT | APIC_ICR_TRIGGER_EDGE | APIC_ICR_DEST_NONE;
    }
    //  wait for it to complete
    while (sys_info.local_apic->icr_lo & APIC_ICR_DELIVERY_STATUS) {
        asm("pause");
    }
}

void apic_interrupt_command_sipi(bool isShorthand, uint64_t dest, uint8_t vector)
{
    if (isShorthand) {
        sys_info.local_apic->icr_hi = 0;
        sys_info.local_apic->icr_lo = APIC_ICR_DELIVERY_STARTUP | APIC_ICR_DESTMODE_PHYSICAL | APIC_ICR_LEVEL_ASSERT | APIC_ICR_TRIGGER_EDGE | dest | vector;
    } else {
        sys_info.local_apic->icr_hi = dest & 0xFF000000;
        sys_info.local_apic->icr_lo = APIC_ICR_DELIVERY_STARTUP | APIC_ICR_DESTMODE_PHYSICAL | APIC_ICR_LEVEL_ASSERT | APIC_ICR_TRIGGER_EDGE | APIC_ICR_DEST_NONE | vector;
    }
    //  wait for it to complete
    while (sys_info.local_apic->icr_lo & APIC_ICR_DELIVERY_STATUS) {
        asm("pause");
    }
}

int init_symmetric_multi_processor()
{
    int ret = 0;
    extern unsigned char trampolin[];
    extern unsigned int trampolin_len;

    if ((ret = PageManager::IdentityMapPages(0x8000, trampolin_len)) < 0) {
        printf("Failed to map pages\n");
        return ret;
    }
    trampolin_t *tramp = (trampolin_t *)0x8000;

    //  copy trampolin code to 0x8000
    memcpy((void *)0x8000, trampolin, trampolin_len);

    apic_interrupt_command_init(true, APIC_ICR_DEST_ALL_NOSELF);
    //  start at 0x8000
    apic_interrupt_command_sipi(true, APIC_ICR_DEST_ALL_NOSELF, 0x8);

    while (tramp->proc_count == 0) {
        printf("Waiting for processor...\n");
    }
    printf("Processor found\n");

    //  unmap memory
    if ((ret = PageManager::freeVirtualMemory(0x8000, trampolin_len)) < 0) {
        printf("Failed to freeVirtualMemory()\n");
        return ret;
    }

    asm("cli;hlt");

    return 0;
}