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

#include <stddef.h>
#include <unistd.h>
#include <stdint.h>
#include <kernel/syscall.h>

#if __STDC_HOSTED__ == 1
int chdir(const char *path)
{
    int ret = 0;
    __asm__ volatile("syscall"
                     : "=a"(ret)
                     : "D"(SYSCALL_CHDIR), "S"(path), "d"(0)
                     : "rcx", "r11");
    return ret;
}
char *getcwd(char *buf, size_t size)
{
    char *ret = 0;
    __asm__ volatile("syscall"
                     : "=a"(ret)
                     : "D"(SYSCALL_GETCWD), "S"(buf), "d"(size)
                     : "rcx", "r11");
    return ret;
}
int close(int fd)
{
    int ret = _syscall(SYSCALL_CLOSE, fd, 0);
    return ret;
}

pid_t getpid(void)
{
    pid_t pid;
    int ret = _syscall(SYSCALL_GETPID, (uint64_t)&pid, 0);
    return pid;
}
ssize_t read(int fd, void *buf, size_t count)
{
    int ret = 0;
    struct read_args args = {fd, buf, count};
    ret = _syscall(SYSCALL_READ, (uint64_t)&args, 0);
    return ret;
}
ssize_t write(int fd, const void *buf, size_t count)
{
    int ret = 0;
    struct write_args args = {fd, buf, count};
    ret = _syscall(SYSCALL_WRITE, (uint64_t)&args, 0);
    return ret;
}
int isatty(int fd)
{
    int ret = 0;
    ret = _syscall(SYSCALL_ISATTY, (uint64_t)fd, 0);
    if (!ret)
        return 1;
    else {
        errno = -ret;
        return 0;
    }
}
#endif