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

#include "timer.h"
#include <threads.h>
#include "apic.h"
#include "global.h"
#include "interrupts.h"
#include "stddef.h"
#include "string.h"
#include <kernel/multitask.h>
#include <stdint.h>
#include <stdio.h>

static uint64_t tick = 0;

void apic_timer_callback(retStack_t *stack, general_registers_t *regs)
{
    tick++;
    const thread_t next_thread = multitask::getInstance()->getNextThread(stack, regs);
    eoi();
    change_thread(next_thread);
}

void sleep(uint64_t delay)
{
    for (uint64_t start_tick = tick; (tick - start_tick) < delay;)
    {
        __asm__("hlt");
    }
}

uint64_t getSystemTime()
{
    return tick;
}