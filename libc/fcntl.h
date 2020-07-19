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

#ifndef _FCNTL_H_
#define _FCNTL_H_

#define AT_FDCWD -100

#define O_RDONLY (1 << 0)
#define O_NONBLOCK (1 << 1)
#define O_CLOEXEC (1 << 2)
#define O_DIRECTORY (1 << 3)

#define NAME_MAX 256

struct dirent
{
    char d_name[NAME_MAX];
};

int openat(int dirfd, const char *pathname, int flags);
int getdents64(int fd, struct dirent *dirp, size_t count);

#endif