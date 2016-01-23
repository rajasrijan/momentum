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

#include "keyboard.h"
#include "apic.h"
#include "interrupts.h"
#include "global.h"

#define  max_key (110)
uint8_t key_grid[max_key] = {SC_ESC, SC_1, SC_2, SC_3, SC_4, 0};
uint8_t key_buffer[1024];
uint32_t index = 0;

static void keyboard_handler(registers_t *regs)
{
    uint8_t scan_code = inb(0x60);
    if (index > 1024)
        return;
/*
    if (scan_code <= max_key)
        printf("keycode %d", key_grid[scan_code - 1]);
*/
}

void init_keyboard()
{
    register_interrupt_handler(IRQ(1), keyboard_handler);
    apic_pin_enable(1);
    uint8_t temp = inb(0x61);
    outb(0x61, temp | 0x80); /* Disable */
    outb(0x61, temp & 0x7F); /* Re-enable */
}
