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
#include <limits.h>
#include <string.h>
#include <sys/syscall.h>

extern void _exit(int status);
extern int create_heap();
extern int main(int argc, char **argv);
int argc = 0;
char *argv[256] = {};
char tmp[4096] = {};
char program_invocation_short_name[NAME_MAX];
char program_invocation_name[1024];
void _start()
{
    int ret = 0;
    ret = create_heap();
    if (ret != 0)
        goto error_exit;
    ret = _syscall(SYSCALL_GETCMDLINE, tmp, sizeof(tmp));
    char *ptr = tmp;
    argv[argc++] = ptr;
    for (size_t i = 0; (ptr[i] != 0) && (i < 4096); i++)
    {
        if (ptr[i] == ' ')
        {
            ptr[i] = 0;
            argv[argc++] = &ptr[i + 1];
        }
    }
    //  copy the program_invocation_name
    strcpy(program_invocation_name, argv[0]);
    //  call main
    ret = main(argc, argv);
error_exit:
    _exit(ret);
}
