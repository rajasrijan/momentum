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
#include <stddef.h>
#include <fcntl.h>
#include <kernel/syscall.h>
#include <errno.h>

#if __STDC_HOSTED__ == 1
int getdents64(int fd, struct dirent *dirp, size_t count)
{
    struct getdents_args args = {fd, dirp, count};
    int ret = _syscall(SYSCALL_GETDENTS, (uint64_t)&args, 0);
    return ret;
}

int open(const char *path, int oflag, ...)
{
    struct open_args args = {path, oflag};
    int ret = _syscall(SYSCALL_OPEN, (uint64_t)&args, 0);
    return ret;
}
int openat(int dirfd, const char *pathname, int flags, mode_t mode)
{
    struct openat_args args = {dirfd, pathname, flags};
    int ret = _syscall(SYSCALL_OPENAT, (uint64_t)&args, mode);
    return ret;
}
#endif