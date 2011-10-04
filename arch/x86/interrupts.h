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

#ifndef INTERRUPTS_H
#define	INTERRUPTS_H

#include <stdint.h>

typedef struct registers
{
    uint32_t ds; // Data segment selector
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
    uint32_t int_no, err_code; // Interrupt number and error code (if applicable)

    /*
     * Stack structure differs depending on difference in privilege level.
     */
    uint32_t eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
} __attribute__((packed)) registers_t;

typedef void (*isr_t)(registers_t*);
void register_interrupt_handler(uint8_t n, isr_t handler);
void isr_handler(registers_t regs);

#endif	/* INTERRUPTS_H */

