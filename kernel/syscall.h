/*
 * Copyright 2009-2020 Srijan Kumar Sharma
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
#pragma once
#define SYSCALL_EXIT (0)
#define SYSCALL_PUTCHAR (1)
#define SYSCALL_GETCHAR (2)
#define SYSCALL_GETCWD (3)
#define SYSCALL_CHDIR (4)
#define SYSCALL_CLOSE (5)
#define SYSCALL_OPENAT (6)
#define SYSCALL_GETDENTS (7)

#pragma pack(push, 8)
struct openat_args
{
    int dirfd;
    const char *pathname;
    int flags;
};
struct getdents_args
{
    int fd;
    struct dirent *dirp;
    size_t count;
};
#pragma pack(pop)

#if __STDC_HOSTED__ == 1
int _syscall(int64_t callid, int64_t arg0, int64_t arg1);
#endif