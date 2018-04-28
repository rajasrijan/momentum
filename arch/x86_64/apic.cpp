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

#include "apic.h"
#include "acpi.h"
#include "timer.h"
#include "interrupts.h"
ioapic_t* ioapic;
static void* find_rspd(void);

/*
 * Very basic and file private memory compare(memcmp).
 */
static uint8_t kmemcmp(const char* src, const char* dst, uint32_t size)
{
	for (int i = 0; i < size; i++)
		if ((src[i] - dst[i]) != 0)
			return (uint8_t)(src[i] - dst[i]);
	return 0;
}

/*
 * Initilize IO-APIC.
 */
void init_ioapic()
{
	/*
	 * If there is a better way let me know.
	 */
	ioapic = (ioapic_t*)0xFEC00000;
	PageManager::getInstance()->IdentityMap2MBPages(0xFEC00000);
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

/*
 * Initilize APIC timer.
 */
void init_apic_timer(uint32_t frequency)
{
	outb(0x21, 0xFF);
	outb(0xA1, 0xFF);
	register_interrupt_handler(32, apic_timer_callback);
	sys_info.local_apic->lvt_timer = 0x00020020;
	sys_info.local_apic->div_conf = 0x02;
	sys_info.local_apic->init_count = frequency;
}

/*
 * Find ACPI rsdp.
 */
static void* find_rspd()
{
	uint8_t* ptr;
	ptr = (uint8_t*)0x40E;
	ptr = (uint8_t*)((uint32_t*)(ptr));
	for (uint32_t i = 0; i < 1024; i += 2)
	{
		if (!kmemcmp((char*)(&ptr[i]), ACPI_RSDP_SIGNATURE, 8))
		{
			return (void*)(&ptr[i]);
		}
	}
	ptr = (uint8_t*)0xE0000;
	for (uint32_t i = 0; i < 0x20000; i += 2)
	{
		if (!kmemcmp((char*)(&ptr[i]), ACPI_RSDP_SIGNATURE, 8))
		{
			return (void*)(&ptr[i]);
		}
	}
	return 0;
}

/*
 * Send End of interrupt to APIC. INCOMPLETE.
 */
void send_eoi()
{
	sys_info.local_apic->eoi = 0;
}
