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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    char cmdline[4096] = {0};
    char path[256] = {0};
    while (1)
    {
        getcwd(path, sizeof(path));
        printf("%s>", path);
        gets_s(cmdline, sizeof(cmdline));
        char *cmd = cmdline;
        char *args = NULL;
        for (size_t i = 0; cmdline[i] != 0; i++)
        {
            if (cmdline[i] == ' ')
            {
                cmdline[i] = 0;
                args = &cmdline[i + 1];
                break;
            }
        }
        if (!strcmp("cd", cmd))
        {
            chdir(args);
        }
        else if (!strcmp("exit", cmd))
        {
            exit(0);
        }
    }
    return 0;
}
