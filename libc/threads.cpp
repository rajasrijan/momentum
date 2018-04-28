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
#include "threads.h"
#include "../arch/x86_64/global.h"

int mtx_init(mtx_t* mutex, int type)
{
    *mutex = 0;
    return 0;
}

int mtx_lock(mtx_t* mutex)
{
    if (!get_spin_lock(mutex))
        return thrd_success;
    return thrd_error;
}

int mtx_trylock(mtx_t *mutex)
{
    if (!get_async_spin_lock(mutex))
        return thrd_success;
    else
        return thrd_busy;

    return thrd_busy;
}

int mtx_unlock(mtx_t *mutex)
{
    release_spin_lock(mutex);
    return thrd_success;
}

void mtx_destroy(mtx_t *mutex)
{
}
