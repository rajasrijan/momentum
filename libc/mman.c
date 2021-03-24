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

#ifndef MMAN_H
#define MMAN_H

#include <errno.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <kernel/syscall.h>

void *mmap(void *addr, size_t len, int prot, int flags, int fildes, off_t off)
{
    int ret = 0;
    void *ptr = NULL;
    struct mmap_args args = {addr, len, prot, flags, fildes, off};
    ret = _syscall(SYSCALL_MMAP, (int64_t)&args, (int64_t)&ptr);
    if (ret != 0) {
        ptr = 0;
        errno = -ret;
    }

    return ptr;
}
#endif