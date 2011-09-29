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

#include "interrupts.h"
#include "global.h"
#include "apic.h"
static void eoi(void);
isr_t interrupt_handlers[256] = {0};

/* This gets called from our ASM interrupt handler stub.*/

void isr_handler(registers_t regs)
{
    if (interrupt_handlers[regs.int_no] != 0)
    {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(&regs);
        if (regs.int_no >= 32)
            eoi();
    }
    else
    {
        printf("regs.int_no=%x", regs.int_no);
        __asm__("cli");
        __asm__("hlt");
    }
}

void register_interrupt_handler(uint8_t n, isr_t handler)
{
    interrupt_handlers[n] = handler;
}

static void eoi()
{
    lapic->eoi = 0;
}