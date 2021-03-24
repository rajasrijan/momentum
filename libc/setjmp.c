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

#include <setjmp.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

int setjmp(jmp_buf env)
{
    env.opaque = malloc(512);
    extern int _setjmp(void *env);
    return _setjmp(env.opaque);
}

_Noreturn void longjmp(jmp_buf env, int val)
{
    extern void _longjmp(void *env, int val);
    _longjmp(env.opaque, val);
}