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

#include <stddef.h>
#include <limits.h>
#include <dirent.h>
#include <sys/types.h>

#define AT_FDCWD -100

#define F_GETFD (0)
#define F_SETFD (1)
#define F_DUPFD (2)

#define F_GETFL (0)
#define F_SETFL (1)

#define O_RDONLY (1 << 0)   //    Open for reading only.
#define O_WRONLY (1 << 1)   //    Open for writing only.
#define O_RDWR (1 << 2)     //    Open for reading and writing. The result is undefined \
                             //    if this flag is applied to a FIFO.
#define O_NONBLOCK (1 << 3) //    When opening a FIFO with O_RDONLY or O_WRONLY set: If     \
                             //    O_NONBLOCK is set: An open() for reading only will return \
                             //    without delay. An open() for writing only will return an  \
                             //    error if no process currently has the file open for reading.
#define O_DIRECTORY (1 << 4)
#define O_CLOEXEC (1 << 5)
#define O_TRUNC (1 << 6)
#define O_CREAT (1 << 7)
#define O_APPEND (1 << 8)
#define O_EXCL (1 << 9)

#define R_OK 0
#define W_OK 1
#define X_OK 2
#define F_OK 3

#if __STDC_HOSTED__ == 1
int open(const char *path, int oflag, ...);
#endif
int openat(int dirfd, const char *pathname, int flags);
int getdents64(int fd, struct dirent *dirp, size_t count);

#endif