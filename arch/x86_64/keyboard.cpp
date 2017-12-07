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

#include "keyboard.h"
#include "apic.h"
#include "interrupts.h"
#include "global.h"

#define max_key (256)
uint8_t key_grid[max_key] = {};
const int key_buffer_size = 1024;
uint8_t key_buffer[key_buffer_size];
volatile uint64_t g_ullKeyBufferIndex = 0;

static void keyboard_handler(retStack_t *regs, general_registers_t *context)
{
    bool keyProcessed = false;
    uint8_t scan_code = inb(0x60);
    bool IsKeyUp = scan_code & 0x80;
    uint8_t keyCode = scan_code & 0x7F;
    char ch = 0;
    
    if (IsKeyUp)
        key_grid[keyCode] = 0;
    else
        key_grid[keyCode] = 1;
    
    if (keyCode==SC_RSHIFT||keyCode==SC_LSHIFT)
    {
        keyProcessed=true;
    }
    else if (keyCode >= 0xF && keyCode <= 0x39)
    {
        const char rowDOWN[] = "\tqwertyuiop[]\n asdfghjkl;'  \\zxcvbnm,./    ";
        const char rowUP[] = "\tQWERTYUIOP{}\n ASDFGHJKL:\"  |ZXCVBNM<>?    ";
        if (key_grid[SC_RSHIFT] || key_grid[SC_LSHIFT])
            ch = rowUP[keyCode - 0xF];
        else
            ch = rowDOWN[keyCode - 0xF];
        keyProcessed = true;
    }

    if (IsKeyUp && ch)
    {
        key_buffer[g_ullKeyBufferIndex % key_buffer_size] = ch;
        g_ullKeyBufferIndex = ((g_ullKeyBufferIndex + 1) % key_buffer_size);
    }
    else if(!keyProcessed)
    {
        printf("^%x", keyCode);
    }
}

void init_keyboard()
{
    DBG_OUTPUT
    register_interrupt_handler(IRQ(1), keyboard_handler);
    DBG_OUTPUT
    apic_pin_enable(1);
    DBG_OUTPUT
    uint8_t temp = inb(0x61);
    DBG_OUTPUT
    outb(0x61, temp | 0x80); /* Disable */
    DBG_OUTPUT
    outb(0x61, temp & 0x7F); /* Re-enable */
}

char getchar()
{
    char c = 0;
    uint64_t localIndex = g_ullKeyBufferIndex;
    while (localIndex == g_ullKeyBufferIndex)
    {
        __asm__("hlt;");
    }
    c = key_buffer[localIndex % key_buffer_size];
    putchar(c);
    return c;
}