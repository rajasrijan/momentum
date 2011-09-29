/*
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

#include "acpi.h"
#include "apic.h"
#include "paging.h"
#include "global.h"
#include <stdint.h>
#include <string.h>
#include <ctype.h>

extern uint8_t kmemcmp(const char* src, const char* dst, uint32_t size);
void* find_rspd(void);
void * ioapic_base;
__attribute__((section(".sdata"))) uint32_t _cpu_count;
__attribute__((section(".sdata"))) uint32_t _ioapic_count;
__attribute__((section(".sdata"))) uint32_t tmp;
__attribute__((section(".sdata"))) void* _apic_base;
__attribute__((section(".sdata"))) acpi_rsdp_t *_rsdp;
acpi_rsdp_t *rsdp;

__attribute__((section(".stext"))) void parse_acpi()
{
    _cpu_count = 0;
    _rsdp = find_rspd();
    if (_rsdp == 0)
        __asm__("cli;hlt;");
    acpi_rsdt_t *rsdt = (void*) (_rsdp->RsdtAddress);
    uint32_t no_of_descriptors = ((rsdt->header.Length
            - sizeof (acpi_description_header_t)) / 4);
    acpi_description_header_t **adht = (void*) ((_rsdp->RsdtAddress)
            + sizeof (acpi_description_header_t));

    for (uint32_t i = 0; i < no_of_descriptors; i++)
    {
        if (adht[i]->Signature == ACPI_DESCRIPTION_HEADER_MADT)
        {
            adht += i;
            break;
        }
    }
    madt_structure_t *madt = (void*) *adht;
    madt_entry_structure_t *madt_entry = &(madt->apic_entries);
    uint32_t len = (madt->header.Length - offsetof(madt_structure_t, apic_entries));
    _apic_base = madt->lapic;
    while (len > 0)
    {
        if (madt_entry->length == 0)
            __asm__("cli;hlt;");
        switch (madt_entry->type)
        {
        case LOCAL_APIC:
        {
            _cpu_count++;
        }
        }
        len -= madt_entry->length;
        madt_entry = (void*) ((char*) madt_entry + (uint32_t) (madt_entry->length));
    }
    _cpu_count = 1;
    _ioapic_count = 1;
}

__attribute__((section(".stext"))) void* find_rspd()
{
    uint8_t* ptr;
    ptr = (void*) 0x40E;
    ptr = (void*) ((uint32_t*) (ptr));
    for (uint32_t i = 0; i < 1024; i += 2)
    {
        if (!kmemcmp((char*) (&ptr[i]), ACPI_RSDP_SIGNATURE, 8))
        {
            return (void*) (&ptr[i]);
        }
    }
    ptr = (void*) 0xE0000;
    for (uint32_t i = 0; i < 0x20000; i += 2)
    {
        if (!kmemcmp((char*) (&ptr[i]), ACPI_RSDP_SIGNATURE, 8))
        {
            return (void*) (&ptr[i]);
        }
    }
    return 0;
}

void fix_refferances()
{
    identity_map_4mb((uint32_t) _apic_base);
    lapic = _apic_base;
    rsdp = _rsdp;
}

static void default_apic_mapping(void)
{
    lapic->lvt_timer = LVT_MASKED | 32;
    lapic->lvt_thermal = LVT_MASKED | 33;
    lapic->lvt_perf = LVT_MASKED | 34;
    lapic->lvt_lint0 = LVT_MASKED | 35;
    lapic->lvt_lint1 = LVT_MASKED | 36;
    lapic->lvt_err = LVT_MASKED | 37;

    lapic->svr |= 0x100;
    printf("\n svr=%x", lapic->tpr);
    lapic->tpr = 0;
    printf("\n svr=%x", lapic->tpr);
    outb(0x21, 0xFF);
    outb(0xA1, 0xFF);
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, IRQ(0));
    outb(0xA1, IRQ(8));
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
    outb(0x21, 0xFF);
    outb(0xA1, 0xFF);
}

static void default_ioapic_mapping(void)
{
    init_ioapic();
    /*
     * Pin 0 - INTR - LINTN 0 - Edge triggered - Active Hi.
     */
    write_ioapic(0x11, 0x00000000);
    write_ioapic(0x10, 0x00010000 | 35);
    /*
     * Pin 1 - IRQ1 - Keyboard Controller - Edge triggered - Active Hi.
     */
    write_ioapic(0x13, 0x00000000);
    write_ioapic(0x12, 0x00010000 | 39);
    /*
     * Pin 2 - IRQ2 - 8254 Timer - Edge triggered - Active Hi.
     */
    write_ioapic(0x15, 0x00000000);
    write_ioapic(0x14, 0x00010000 | 38);
    /*
     * Pin 3 - IRQ3 - LITM3 - Edge triggered - Active Hi.
     */
    write_ioapic(0x17, 0x00000000);
    write_ioapic(0x16, 0x00010000 | 40);
    /*
     * Pin 4 - IRQ4 - LITM4 - Edge triggered - Active Hi.
     */
    write_ioapic(0x19, 0x00000000);
    write_ioapic(0x18, 0x00010000 | 41);
    /*
     * Pin 5 - IRQ5 - LITM5 - Edge triggered - Active Hi.
     */
    write_ioapic(0x1b, 0x00000000);
    write_ioapic(0x1a, 0x00010000 | 42);
    /*
     * Pin 6 - IRQ6 - LITM6 - Edge triggered - Active Hi.
     */
    write_ioapic(0x1d, 0x00000000);
    write_ioapic(0x1c, 0x00010000 | 43);
    /*
     * Pin 7 - IRQ7 - LITM7 - Edge triggered - Active Hi.
     */
    write_ioapic(0x1f, 0x00000000);
    write_ioapic(0x1e, 0x00010000 | 44);
    /*
     * Pin 8 - IRQ8 - Real time clock - Edge triggered - Active lo.
     */
    write_ioapic(0x21, 0x00000000);
    write_ioapic(0x20, 0x00012000 | 45);
    /*
     * Pin 9 - IRQ7 - LITM7 - Edge triggered - Active Hi.
     */
    write_ioapic(0x23, 0x00000000);
    write_ioapic(0x22, 0x00010000 | 46);
    /*
     * Pin 10 - IRQ7 - LITM7 - Edge triggered - Active Hi.
     */
    write_ioapic(0x25, 0x00000000);
    write_ioapic(0x24, 0x00010000 | 47);
    /*
     * Pin 11 - IRQ7 - LITM7 - Edge triggered - Active Hi.
     */
    write_ioapic(0x27, 0x00000000);
    write_ioapic(0x26, 0x00010000 | 48);
    /*
     * Pin 12 - IRQ7 - LITM7 - Edge triggered - Active Hi.
     */
    write_ioapic(0x29, 0x00000000);
    write_ioapic(0x28, 0x00010000 | 49);
    /*
     * Pin 13 - IRQ1 - DMA Chaining - Edge triggered - Active Hi.
     */
    write_ioapic(0x2b, 0x00000000);
    write_ioapic(0x2a, 0x00010000 | 50);
    /*
     * Pin 14 - IRQ1 - Keyboard Controller - Edge triggered - Active Hi.
     */
    write_ioapic(0x2d, 0x00000000);
    write_ioapic(0x2c, 0x00010000 | 51);
    /*
     * Pin 15 - IRQ1 - Keyboard Controller - Edge triggered - Active Hi.
     */
    write_ioapic(0x2f, 0x00000000);
    write_ioapic(0x2e, 0x00010000 | 52);
    outb(0x22, 0x70);
    outb(0x23, 0x1);
}

static void override_interrupt(uint8_t source, uint32_t pin, uint16_t flags)
{
    uint32_t hi_pin, lo_pin;
    lo_pin = 0x10 + (pin * 2);
    hi_pin = 0x11 + (pin * 2);
    uint32_t flags_hi = 0, flags_lo = 0;
    switch (flags & 0x3)
    {
    case 0x01:
    {
        flags_lo &= 0xFFFFDFFF;
        break;
    }
    case 0x3:
    {
        flags_lo |= 0x00002000;
        break;
    }
    }
    switch ((flags >> 2) & 0x3)
    {
    case 0x01:
    {
        flags_lo &= 0xFFFF7FFF;
        break;
    }
    case 0x3:
    {
        flags_lo |= 0x00008000;
        break;
    }
    }
    write_ioapic(hi_pin, 0x00000000 | flags_hi);
    write_ioapic(lo_pin, 0x00010000 | IRQ(source) | flags_lo);
}

uint8_t get_acpi_tables()
{
    default_apic_mapping();
    default_ioapic_mapping();

    acpi_rsdt_t *rsdt = (void*) (rsdp->RsdtAddress);
    if (checksum((void*) rsdt, rsdt->header.Length))
        __asm__("cli;hlt;");
    uint32_t no_of_descriptors = ((rsdt->header.Length - sizeof (acpi_description_header_t)) / 4);
    acpi_description_header_t **adht = (void*) ((rsdp->RsdtAddress) + sizeof (acpi_description_header_t));
    for (uint32_t i = 0; i < no_of_descriptors; i++)
    {
        if (adht[i]->Signature == ACPI_DESCRIPTION_HEADER_MADT)
        {
            adht += i;
            break;
        }
    }
    madt_structure_t *madt = (void*) *adht;
    madt_entry_structure_t *madt_entry = &(madt->apic_entries);
    uint32_t len = (madt->header.Length - offsetof(madt_structure_t, apic_entries));

    while (len > 0)
    {
        if (madt_entry->length == 0)
            __asm__("cli;hlt;");
        switch (madt_entry->type)
        {
        case INTERRUPT_OVERRIDE:
        {
            interrupt_override_t * intovr = (void*) madt_entry;
            override_interrupt(intovr->source, intovr->global_system_interrupt, intovr->flags);
            break;
        }
        }
        len -= madt_entry->length;
        madt_entry = (void*) ((char*) madt_entry + (uint32_t) (madt_entry->length));
    }
    return true;
}
