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

#ifndef SYSCALL_H
#define SYSCALL_H
#include <stdint.h>

#define SYSCALL_EXIT (0)
#define SYSCALL_PUTCHAR (1)
#define SYSCALL_GETCHAR (2)
#define SYSCALL_GETCWD (3)
#define SYSCALL_CHDIR (4)
#define SYSCALL_CLOSE (5)
#define SYSCALL_OPEN (6)
#define SYSCALL_OPENAT (7)
#define SYSCALL_GETDENTS (8)
#define SYSCALL_MMAP (9)
#define SYSCALL_GETPID (10)
#define SYSCALL_GETCMDLINE (11)
#define SYSCALL_READ (12)
#define SYSCALL_WRITE (13)

#if __STDC_HOSTED__ == 1
int _syscall(int64_t callid, int64_t arg0, int64_t arg1);
#endif
#endif //SYSCALL_H