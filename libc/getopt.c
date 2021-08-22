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
#include <string.h>

char *optarg = NULL;
int optind = 1, opterr, optopt;

gid_t getegid(void)
{
    asm("int3");
    return -ENOSYS;
}
uid_t geteuid(void)
{
    asm("int3");
    return -ENOSYS;
}
gid_t getgid(void)
{
    asm("int3");
    return -ENOSYS;
}
gid_t getgroups(int n, gid_t *groups)
{
    asm("int3");
    return -ENOSYS;
}
long gethostid(void)
{
    asm("int3");
    return -ENOSYS;
}
char *getlogin(void)
{
    return NULL;
}
int getlogin_r(char *, size_t)
{
    asm("int3");
    return -ENOSYS;
}

int getopt(int argc, char *const argv[], const char *optstring)
{
    int ret = -1;
    int optstrind = 0;
    int has_argument = 0;
    if (optind == 0)
        optind = 1;
    //  all the arguments have been processed.
    if (optind >= argc)
    {
        ret = -1;
        goto error_exit;
    }
    if ((argv[optind] == NULL) || (argv[optind][0] != '-') || (!strcmp(argv[optind], "-")))
    {
        ret = -1;
        goto error_exit;
    }
    if (!strcmp(argv[optind], "--"))
    {
        optind++;
        ret = -1;
        goto error_exit;
    }
    char *arg = (optarg) ? optarg : &argv[optind][1];
    for (size_t i = 0; arg[i] != 0; i++)
    {
        for (optstrind = 0; optstring[optstrind] != 0; optstrind++)
        {
            if (optstring[optstrind] == arg[i])
            {
                optarg = NULL;
                ret = optstring[optstrind];
                if (arg[i + 1] == 0)
                {
                    optind++;
                    if (optstring[optstrind + 1] == ':')
                    {
                        optarg = argv[optind];
                        optind++;
                    }
                    else
                        optarg = NULL;
                }
                else
                {
                    optarg = &arg[i + 1];
                    if (optstring[optstrind + 1] == ':')
                    {
                        optind++;
                    }
                }
                goto error_exit;
            }
        }
    }
    ret = '?';
error_exit:
    return ret;
}

int getpagesize(void)
{
    asm("int3");
    return -ENOSYS;
}
char *getpass(const char *)
{
    return NULL;
}
