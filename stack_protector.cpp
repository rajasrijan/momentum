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
#include <stdint.h>
#define STACK_CHK_GUARD 0x595e9fbd94fda766
extern "C"
{
    uint64_t __stack_chk_guard = STACK_CHK_GUARD;

    __attribute__((noreturn)) void __stack_chk_fail(void)
    {
        while (true)
        {
            asm("cli;hlt;");
        }
    }
}