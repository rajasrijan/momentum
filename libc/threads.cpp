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
#if __STDC_HOSTED__ == 0
#include "threads.h"
#include <arch/time.h>
#include <arch/synchronization.h>

int mtx_init(mtx_t *mutex, int type)
{
    *mutex = 0;
    return 0;
}

int mtx_lock(mtx_t *mutex)
{
    if (!get_spin_lock(mutex))
        return thrd_success;
    return thrd_error;
}

int mtx_trylock_notimeout(mtx_t *mutex)
{
    if (!get_async_spin_lock(mutex))
        return thrd_success;
    return thrd_busy;
}

int mtx_trylock(mtx_t *mutex, uint64_t timeout)
{
    auto start_time = getSystemTime();
    do
    {
        if (!get_async_spin_lock(mutex))
            return thrd_success;
        //  system time is not going to change till next interrupt. hence halt.
        asm("hlt");
    } while (getSystemTime() - start_time >= timeout);
    return thrd_timedout;
}

int mtx_unlock(mtx_t *mutex)
{
    release_spin_lock(mutex);
    return thrd_success;
}

void mtx_destroy(mtx_t *mutex)
{
}

int sem_init(sem_t *semaphore, int64_t max_count, int64_t count)
{
    mtx_init(&(semaphore->lock), 0);
    sync(semaphore->lock);
    semaphore->count = count;
    semaphore->max_count = max_count;
    return 0;
}
#endif