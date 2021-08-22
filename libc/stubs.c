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

int raise(int i1)
{
    asm("int3");
    return -ENOSYS;
}
int sigfillset(sigset_t *)
{
    asm("int3");
    return -ENOSYS;
}
void (*signal(int, void (*)(int)))(int)
{
    asm("int3");
}
putc_unlocked()
{
    asm("int3");
    return -ENOSYS;
}

fcntl()
{
    asm("int3");
    return -ENOSYS;
}

bsearch()
{
    asm("int3");
    return -ENOSYS;
}
umask()
{
    asm("int3");
    return -ENOSYS;
}

times()
{
    asm("int3");
    return -ENOSYS;
}
qsort()
{
    asm("int3");
    return -ENOSYS;
}
int execve(const char *, char *const[], char *const[])
{
    asm("int3");
    return -ENOSYS;
}
int fnmatch(const char *, const char *, int)
{
    asm("int3");
    return -ENOSYS;
}
int dup2(int, int)
{
    asm("int3");
    return -ENOSYS;
}
pid_t waitpid(pid_t, int *, int)
{
    asm("int3");
    return -ENOSYS;
}
int sigsuspend(const sigset_t *)
{
    asm("int3");
    return -ENOSYS;
}
int sigprocmask(int, const sigset_t *, sigset_t *)
{
    asm("int3");
    return -ENOSYS;
}
strsignal()
{
    asm("int3");
    return -ENOSYS;
}
WCOREDUMP()
{
    asm("int3");
    return -ENOSYS;
}
sprintf()
{
    asm("int3");
    return -ENOSYS;
}
pid_t fork(void)
{
    asm("int3");
    return -ENOSYS;
}
environ()
{
    asm("int3");
    return -ENOSYS;
}
pid_t getppid(void)
{
    asm("int3");
    return -ENOSYS;
}
getpwnam()
{
    asm("int3");
    return -ENOSYS;
}
int pipe(int[2])
{
    asm("int3");
    return -ENOSYS;
}
S_ISREG()
{
    asm("int3");
    return -ENOSYS;
}
int fstat(int fd, struct stat *buf)
{
    asm("int3");
    return -ENOSYS;
}
ferror_unlocked()
{
    asm("int3");
    return -ENOSYS;
}
clearerr()
{
    asm("int3");
    return -ENOSYS;
}
dprintf()
{
    asm("int3");
    return -ENOSYS;
}

uid_t getuid(void)
{
    asm("int3");
    return -ENOSYS;
}

poll()
{
    asm("int3");
    return -ENOSYS;
}

strtoull()
{
    asm("int3");
    return -ENOSYS;
}
strtoul()
{
    asm("int3");
    return -ENOSYS;
}

long int sysconf(int)
{
    asm("int3");
    return -ENOSYS;
}
strncasecmp()
{
    asm("int3");
    return -ENOSYS;
}
strcasecmp()
{
    asm("int3");
    return -ENOSYS;
}

S_ISLNK()
{
    asm("int3");
    return -ENOSYS;
}
S_ISBLK()
{
    asm("int3");
    return -ENOSYS;
}
S_ISCHR()
{
    asm("int3");
    return -ENOSYS;
}
readlink()
{
    asm("int3");
    return -ENOSYS;
}

ioctl()
{
    asm("int3");
    return -ENOSYS;
}
getenv()
{
    asm("int3");
    return -ENOSYS;
}

int sigemptyset(sigset_t *)
{
    asm("int3");
    return 0;
}

void sscanf()
{
    asm("int3");
}
void localtime()
{
    asm("int3");
}
void mktime()
{
    asm("int3");
}
void time()
{
    asm("int3");
}
void strndup()
{
    asm("int3");
}
void unlink()
{
    asm("int3");
}
void rename()
{
    asm("int3");
}
void lseek()
{
    asm("int3");
}
void mkstemp()
{
    asm("int3");
}
void fileno_unlocked()
{
    asm("int3");
}
void setenv()
{
    asm("int3");
}
void setgid()
{
    asm("int3");
}
void setuid()
{
    asm("int3");
}
void setegid()
{
    asm("int3");
}
void seteuid()
{
    asm("int3");
}
void fchdir()
{
    asm("int3");
}
void chroot()
{
    asm("int3");
}
void socket()
{
    asm("int3");
}
void bind()
{
    asm("int3");
}
void listen()
{
    asm("int3");
}
void sendto()
{
    asm("int3");
}
void ttyname_r()
{
    asm("int3");
}
void srand()
{
    asm("int3");
}
void vfork()
{
    asm("int3");
}
void realpath()
{
    asm("int3");
}

int sigaddset(sigset_t *, int)
{
    asm("int3");
}

void strftime()
{
    asm("int3");
}

void unsetenv()
{
    asm("int3");
}

void rand()
{
    asm("int3");
}
