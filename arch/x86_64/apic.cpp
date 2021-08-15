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
#include <errno.h>
#include <arch/paging.h>
#include <kernel/sys_info.h>
#include <kernel/logging.h>
#include <arch/mm.h>

extern "C"
{
#include <actables.h>
}
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
    ret = PageManager::IdentityMapPages(0xFEC00000, PageManager::PAGESIZE);
    if (ret < 0)
    {
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
    if (isShorthand)
    {
        sys_info.local_apic->icr_hi = 0;
        sys_info.local_apic->icr_lo = APIC_ICR_DELIVERY_INIT | APIC_ICR_DESTMODE_PHYSICAL | APIC_ICR_LEVEL_ASSERT | APIC_ICR_TRIGGER_EDGE | dest;
    }
    else
    {
        sys_info.local_apic->icr_hi = dest & 0xFF000000;
        sys_info.local_apic->icr_lo = APIC_ICR_DELIVERY_INIT | APIC_ICR_DESTMODE_PHYSICAL | APIC_ICR_LEVEL_ASSERT | APIC_ICR_TRIGGER_EDGE | APIC_ICR_DEST_NONE;
    }
    //  wait for it to complete
    while (sys_info.local_apic->icr_lo & APIC_ICR_DELIVERY_STATUS)
    {
        asm("pause");
    }
}

void apic_interrupt_command_sipi(bool isShorthand, uint64_t dest, uint8_t vector)
{
    if (isShorthand)
    {
        sys_info.local_apic->icr_hi = 0;
        sys_info.local_apic->icr_lo = APIC_ICR_DELIVERY_STARTUP | APIC_ICR_DESTMODE_PHYSICAL | APIC_ICR_LEVEL_ASSERT | APIC_ICR_TRIGGER_EDGE | dest | vector;
    }
    else
    {
        sys_info.local_apic->icr_hi = dest & 0xFF000000;
        sys_info.local_apic->icr_lo = APIC_ICR_DELIVERY_STARTUP | APIC_ICR_DESTMODE_PHYSICAL | APIC_ICR_LEVEL_ASSERT | APIC_ICR_TRIGGER_EDGE |
                                      APIC_ICR_DEST_NONE | vector;
    }
    //  wait for it to complete
    while (sys_info.local_apic->icr_lo & APIC_ICR_DELIVERY_STATUS)
    {
        asm("pause");
    }
}

int init_symmetric_multi_processor()
{
    int ret = 0;
    uint32_t tableIndex = 0;
    ACPI_TABLE_MADT *madt_table = nullptr;
    ret = AcpiTbFindTable("APIC", "", "", &tableIndex);
    if (ACPI_FAILURE(ret))
    {
        if (ret == AE_NOT_FOUND)
        {
            log_error("ACPI Table not found in RSDT/XSDT");
        }
    }

    ret = AcpiGetTableByIndex(tableIndex, (ACPI_TABLE_HEADER **)&madt_table);
    if (ACPI_FAILURE(ret))
    {
        log_error("Failed to ret table");
        return -EUNK;
    }
    log_info("MADT table length [%llx], MADT entry length [%llx]\n", madt_table->Header.Length, sizeof(ACPI_TABLE_MADT));
    log_info("APIC Address [%x], Flags [%x]\n", madt_table->Address, madt_table->Flags);

    for (ACPI_SUBTABLE_HEADER *subtable = ACPI_ADD_PTR(ACPI_SUBTABLE_HEADER, madt_table, sizeof(ACPI_TABLE_MADT));
         (uint64_t)subtable < ((uint64_t)madt_table + madt_table->Header.Length); subtable = (ACPI_SUBTABLE_HEADER *)ACPI_NEXT_RESOURCE(subtable))
    {
        switch (subtable->Type)
        {
        case ACPI_MADT_TYPE_LOCAL_APIC: {
            ACPI_MADT_LOCAL_APIC *lapic = (ACPI_MADT_LOCAL_APIC *)subtable;
            log_info("ProcessorId [%x], Id [%x], LapicFlags [%x]\n", lapic->ProcessorId, lapic->Id, lapic->LapicFlags);
            break;
        }
        case ACPI_MADT_TYPE_IO_APIC: {
            log_error("ACPI_MADT_TYPE_IO_APIC not inplemented!\n");
            break;
        }
        case ACPI_MADT_TYPE_INTERRUPT_OVERRIDE: {
            log_error("ACPI_MADT_TYPE_INTERRUPT_OVERRIDE not inplemented!\n");
            break;
        }
        case ACPI_MADT_TYPE_NMI_SOURCE: {
            log_error("ACPI_MADT_TYPE_NMI_SOURCE not inplemented!\n");
            break;
        }
        case ACPI_MADT_TYPE_LOCAL_APIC_NMI: {
            log_error("ACPI_MADT_TYPE_LOCAL_APIC_NMI not inplemented!\n");
            break;
        }
        case ACPI_MADT_TYPE_LOCAL_APIC_OVERRIDE: {
            log_error("ACPI_MADT_TYPE_LOCAL_APIC_OVERRIDE not inplemented!\n");
            break;
        }
        case ACPI_MADT_TYPE_IO_SAPIC: {
            log_error("ACPI_MADT_TYPE_IO_SAPIC not inplemented!\n");
            break;
        }
        case ACPI_MADT_TYPE_LOCAL_SAPIC: {
            log_error("ACPI_MADT_TYPE_LOCAL_SAPIC not inplemented!\n");
            break;
        }
        case ACPI_MADT_TYPE_INTERRUPT_SOURCE: {
            log_error("ACPI_MADT_TYPE_INTERRUPT_SOURCE not inplemented!\n");
            break;
        }
        case ACPI_MADT_TYPE_LOCAL_X2APIC: {
            log_error("ACPI_MADT_TYPE_LOCAL_X2APIC not inplemented!\n");
            break;
        }
        case ACPI_MADT_TYPE_LOCAL_X2APIC_NMI: {
            log_error("ACPI_MADT_TYPE_LOCAL_X2APIC_NMI not inplemented!\n");
            break;
        }
        case ACPI_MADT_TYPE_GENERIC_INTERRUPT: {
            log_error("ACPI_MADT_TYPE_GENERIC_INTERRUPT not inplemented!\n");
            break;
        }
        case ACPI_MADT_TYPE_GENERIC_DISTRIBUTOR: {
            log_error("ACPI_MADT_TYPE_GENERIC_DISTRIBUTOR not inplemented!\n");
            break;
        }
        case ACPI_MADT_TYPE_GENERIC_MSI_FRAME: {
            log_error("ACPI_MADT_TYPE_GENERIC_MSI_FRAME not inplemented!\n");
            break;
        }
        case ACPI_MADT_TYPE_GENERIC_REDISTRIBUTOR: {
            log_error("ACPI_MADT_TYPE_GENERIC_REDISTRIBUTOR not inplemented!\n");
            break;
        }
        case ACPI_MADT_TYPE_GENERIC_TRANSLATOR: {
            log_error("ACPI_MADT_TYPE_GENERIC_TRANSLATOR not inplemented!\n");
            break;
        }
        case ACPI_MADT_TYPE_MULTIPROC_WAKEUP: {
            log_error("ACPI_MADT_TYPE_MULTIPROC_WAKEUP not inplemented!\n");
            break;
        }
        default:
            log_error("ACPI_MADT_TYPE_UNKNOWN\n");
            break;
        }
    }

    // extern unsigned char trampolin[];
    // extern unsigned int trampolin_len;

    // uint64_t addr = 0;
    // ret = allocate_physical_pages(addr, 1);
    // log_info("allocated physical address [%llx] for trampolin code.\n", addr);

    // if ((ret = PageManager::setVirtualToPhysicalMemory(0x8000, addr, PageManager::round_up_to_pagesize(trampolin_len), PageManager::Privilege::Supervisor,
    //                                                    PageManager::PageType::Read_Write)) < 0)
    // {
    //     printf("Failed to map pages\n");
    //     return ret;
    // }

    // //  copy trampolin code to 0x8000
    // memcpy((void *)0x8000, trampolin, trampolin_len);

    // volatile trampolin_t *tramp = (trampolin_t *)0x8000;

    // apic_interrupt_command_init(true, APIC_ICR_DEST_ALL_NOSELF);
    // //  start at physical address
    // apic_interrupt_command_sipi(true, APIC_ICR_DEST_ALL_NOSELF, addr >> 12);

    // while (tramp->proc_count == 0)
    // {
    //     printf("Waiting for processor...\n");
    // }
    // printf("Processor found\n");

    // //  unmap memory
    // if ((ret = PageManager::freeVirtualMemory(0x8000, trampolin_len)) < 0)
    // {
    //     printf("Failed to freeVirtualMemory()\n");
    //     return ret;
    // }
    return 0;
}
