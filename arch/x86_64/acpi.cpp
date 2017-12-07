/*
 * Copyright 2009-2017 Srijan Kumar Sharma
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

#include "acpi.h"
#include "apic.h"
#include "paging.h"
#include "global.h"
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#define IA32_APIC_BASE_MSR 0x1B
#define IA32_APIC_BASE_MSR_BSP 0x100 // Processor is a BSP
#define IA32_APIC_BASE_MSR_ENABLE 0x800

 /*
 void* find_rspd(void);
 void * ioapic_base;
  */

  /*
   * Fix memory referances.kernel is located in 2gb+ space. unreff lower mem stuff.
   */

void fix_refferances()
{
	//identity_map_4mb((uint64_t)sys_info.local_apic);
	//lapic = _apic_base;
	//rsdp = _rsdp;
}

/*
 * Configure APIC to the default settings. Initial settings are OS specific.
 */
static void default_apic_mapping(void)
{
	uint64_t msr_apicbase = 0;
	__asm("rdmsr":"=a"(msr_apicbase) : "c"((uint64_t)IA32_APIC_BASE_MSR));
	sys_info.local_apic = (local_apic_structure*)(msr_apicbase&APICBASE_ADDRESS);
	printf("local apic:[0x%x], enabled [%d], BSP [%d]\n",
		sys_info.local_apic,
		(msr_apicbase&APICBASE_ENABLED) == APICBASE_ENABLED,
		(msr_apicbase&APICBASE_BSP) == APICBASE_BSP);
	PageManager::getInstance()->IdentityMap2MBPages(((uint64_t)sys_info.local_apic) & 0xFFFFFFFFFFE00000);
	sys_info.local_apic->lvt_timer = LVT_MASKED | 32;
	sys_info.local_apic->lvt_thermal = LVT_MASKED | 33;
	sys_info.local_apic->lvt_perf = LVT_MASKED | 34;
	sys_info.local_apic->lvt_lint0 = LVT_MASKED | 35;
	sys_info.local_apic->lvt_lint1 = LVT_MASKED | 36;
	sys_info.local_apic->lvt_err = LVT_MASKED | 37;

	sys_info.local_apic->svr |= 0x100;

	printf("svr=%x\n", sys_info.local_apic->tpr);

	sys_info.local_apic->tpr = 0;

	printf("svr=%x\n", sys_info.local_apic->tpr);

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

/*
 * Configure IO-APIC to the default settings.
 * Default settings are taken from MultiProcessor(MP) spec.
 */
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

/*
 * Override the default interrupt pin.Configure a IO-APIC pin to a IRQ number.
 */
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

/*
 * Initilizes APIC to dafault value.
 * Finds ACPI rsdp pointer.
 * Very basic ACPI phrasing of ACPI tables.
 * Initilizes IO-APIC.
 */
uint8_t get_acpi_tables()
{

	default_apic_mapping();

	default_ioapic_mapping();
	for (uint64_t rsdptr = 0xE0000; rsdptr < 0xFFFFF; rsdptr += 2)
	{
		if (memcmp((void*)rsdptr, ACPI_RSDP_SIGNATURE, 8))
		{
			sys_info.rsdp = (acpi_rsdp*)rsdptr;
			break;
		}
	}
	printf("RSD PTR [0x%x]\n", sys_info.rsdp);

	acpi_rsdt *rsdt = (acpi_rsdt*)(uint64_t)(sys_info.rsdp->RsdtAddress);
	printf("rsdt [0x%x]\n", rsdt);
	if (checksum((uint8_t*)rsdt, rsdt->header.Length))
	{
		printf("Non zero checksum.");
		__asm__("cli;hlt;");
	}

	uint32_t no_of_descriptors = ((rsdt->header.Length - (uint32_t)sizeof(acpi_description_header)) / 4);
	printf("No of ACPI Descriptors 0x%x\n", no_of_descriptors);

	uint32_t *adhtPtrTbl = (uint32_t*)((sys_info.rsdp->RsdtAddress) + sizeof(acpi_description_header));
	madt_structure *madt = nullptr;
	for (uint32_t i = 0; i < no_of_descriptors; i++)
	{
		if (((acpi_description_header *)(uint64_t)adhtPtrTbl[i])->Signature == ACPI_DESCRIPTION_HEADER_MADT)
		{
			madt = ((madt_structure *)(uint64_t)adhtPtrTbl[i]);
			break;
		}
	}

	madt_entry_structure *madt_entry = &(madt->apic_entries);
	uint32_t len = (madt->header.Length - (uint32_t)offsetof(madt_structure, apic_entries));
	printf("MADT [0x%lx], madt_entry [0x%lx], len [0x%x], madt_structure [0x%x]\n", madt, madt_entry, madt->header.Length, sizeof(madt_structure));
	while (len > 0)
	{
		if (madt_entry->length == 0)
		{
			printf("MADT entry length zero.");
			__asm__("cli;hlt;");
		}
		switch (madt_entry->type)
		{
		case INTERRUPT_OVERRIDE:
		{
			interrupt_override * intovr = (interrupt_override*)madt_entry;
			override_interrupt(intovr->source, intovr->global_system_interrupt, intovr->flags);
			break;
		}
		}
		len -= madt_entry->length;
		madt_entry = (madt_entry_structure*)((char*)madt_entry + (uint32_t)(madt_entry->length));
	}
	return 1;
}
