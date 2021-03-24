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

#include <unistd.h>
#include <getopt.h>
#include <errno.h>

gid_t getegid(void)
{
asm("cli;hlt");
    return -ENOSYS;
}
uid_t geteuid(void)
{
asm("cli;hlt");
    return -ENOSYS;
}
gid_t getgid(void)
{
asm("cli;hlt");
    return -ENOSYS;
}
gid_t getgroups(int n, gid_t *groups)
{
asm("cli;hlt");
    return -ENOSYS;
}
long gethostid(void)
{
asm("cli;hlt");
    return -ENOSYS;
}
char *getlogin(void)
{
    return NULL;
}
int getlogin_r(char *, size_t)
{
asm("cli;hlt");
    return -ENOSYS;
}
int getopt(int, char *const[], const char *)
{
asm("cli;hlt");
    return -ENOSYS;
}
int getpagesize(void)
{
asm("cli;hlt");
    return -ENOSYS;
}
char *getpass(const char *)
{
    return NULL;
}