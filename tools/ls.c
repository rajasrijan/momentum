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

#include <stdint.h>
#include <time.h>
#include <stddef.h>
#include <fcntl.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/stat.h>

struct dirent dir[INT16_MAX];

int main()
{
    int fd = 0, ret = 0;
    fd = openat(AT_FDCWD, ".", O_RDONLY | O_NONBLOCK | O_CLOEXEC | O_DIRECTORY);
    ret = getdents64(fd, dir, INT16_MAX);
    for (int i = 0; i < ret; i++)
        printf("%s\n", dir[i].d_name);
    close(fd);
}