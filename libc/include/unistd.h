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


#ifndef _UNISTD_H_
#define _UNISTD_H_

#include <stddef.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <errno.h>

#ifndef _POSIX_VERSION
#define _POSIX_VERSION 200809L
#endif //_POSIX_VERSION

#define STDERR_FILENO 2
#define STDIN_FILENO 0
#define STDOUT_FILENO 1

int chdir(const char *path);
char *getcwd(char *buf, size_t size);
int close(int fd);
int fsync(int fd);
int fdatasync(int fd);
gid_t getgroups(int n, gid_t *groups);

int pipe(int fd[2]);

#endif