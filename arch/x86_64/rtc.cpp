/*
 * Copyright 2009-2020 Srijan Kumar Sharma
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

#include <stdint.h>
#include <arch/x86_64/rtc.h>
#include "global.h"

void init_rtc()
{
    asm("cli");              // disable interrupts
    outb(0x70, 0x8B);        // select register B, and disable NMI
    char prev = inb(0x71);   // read the current value of register B
    outb(0x70, 0x8B);        // set the index again (a read will reset the index to register D)
    outb(0x71, prev | 0x40); // write the previous value ORed with 0x40. This turns on bit 6 of register B
    asm("sti");
    set_rtc_interrupt_rate(15);
}

void set_rtc_interrupt_rate(uint8_t rate)
{
    rate &= 0x0F; // rate must be above 2 and not over 15
    asm("cli");
    outb(0x70, 0x8A);                 // set index to register A, disable NMI
    char prev = inb(0x71);            // get initial value of register A
    outb(0x70, 0x8A);                 // reset index to A
    outb(0x71, (prev & 0xF0) | rate); // write only our rate to A. Note, rate is the bottom 4 bits.
    asm("sti");
}