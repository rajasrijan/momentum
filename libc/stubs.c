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

#include <errno.h>
#include <signal.h>
#include <setjmp.h>

void longjmp(jmp_buf env, int val)
{
    asm("cli;hlt");
}

int raise(int i1)
{
    return -ENOSYS;
}
int sigfillset(sigset_t *)
{
    return -ENOSYS;
}
void (*signal(int, void (*)(int)))(int)
{
    asm("cli;hlt");
}
putc_unlocked()
{
    return -ENOSYS;
}
fflush()
{
    return -ENOSYS;
}
fcntl()
{
    return -ENOSYS;
}
fputs_unlocked()
{
    return -ENOSYS;
}
bsearch()
{
    return -ENOSYS;
}
umask()
{
    return -ENOSYS;
}
int puts(const char *str)
{
    return -ENOSYS;
}
char *strerror(int errnum)
{
    return -ENOSYS;
}
times()
{
    return -ENOSYS;
}
qsort()
{
    return -ENOSYS;
}
int execve(const char *, char *const[], char *const[])
{
    return -ENOSYS;
}
int fnmatch(const char *, const char *, int)
{
    return -ENOSYS;
}
int dup2(int, int)
{
    return -ENOSYS;
}
pid_t waitpid(pid_t, int *, int)
{
    return -ENOSYS;
}
int sigsuspend(const sigset_t *)
{
    return -ENOSYS;
}
int sigprocmask(int, const sigset_t *, sigset_t *)
{
    return -ENOSYS;
}
strsignal()
{
    return -ENOSYS;
}
WCOREDUMP()
{
    return -ENOSYS;
}
sprintf()
{
    return -ENOSYS;
}
pid_t fork(void)
{
    return -ENOSYS;
}
environ()
{
    return -ENOSYS;
}
pid_t getppid(void)
{
    return -ENOSYS;
}
int stat(const char *restrict, struct stat *restrict)
{
    return -ENOSYS;
}
S_ISDIR()
{
    return -ENOSYS;
}
getpwnam()
{
    return -ENOSYS;
}
int pipe(int[2])
{
    return -ENOSYS;
}
S_ISREG()
{
    return -ENOSYS;
}
int fstat(int fd, struct stat *buf)
{
    return -ENOSYS;
}
int setjmp(jmp_buf env)
{
    return -ENOSYS;
}
ferror_unlocked()
{
    return -ENOSYS;
}
clearerr()
{
    return -ENOSYS;
}
dprintf()
{
    return -ENOSYS;
}
pid_t getpid(void)
{
    return -ENOSYS;
}
uid_t getuid(void)
{
    return -ENOSYS;
}
int isatty(int)
{
    return -ENOSYS;
}
poll()
{
    return -ENOSYS;
}
ssize_t read(int, void *, size_t)
{
    return -ENOSYS;
}
char *optarg = NULL;
int optind = 0, opterr = 0, optopt = 0;
strtoull()
{
    return -ENOSYS;
}
strtoul()
{
    return -ENOSYS;
}
ssize_t write(int, const void *, size_t)
{
    return -ENOSYS;
}
long int sysconf(int)
{
    return -ENOSYS;
}
strncasecmp()
{
    return -ENOSYS;
}
strcasecmp()
{
    return -ENOSYS;
}
vasprintf()
{
    return -ENOSYS;
}