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

#ifndef STAT_H
#define STAT_H

#include <time.h>
#include <stddef.h>

#define S_IRWXU (1 << 0)
#define S_IRUSR (1 << 1)
#define S_IWUSR (1 << 2)
#define S_IXUSR (1 << 3)
#define S_IRWXG (1 << 4)
#define S_IRGRP (1 << 5)
#define S_IWGRP (1 << 6)
#define S_IXGRP (1 << 7)
#define S_IRWXO (1 << 8)
#define S_IROTH (1 << 9)
#define S_IWOTH (1 << 10)
#define S_IXOTH (1 << 11)
#define S_ISUID (1 << 12)
#define S_ISGID (1 << 13)
#define S_ISVTX (1 << 14)
#define S_IFREG (1 << 15)
typedef uint64_t ino_t;
struct stat
{
    uint64_t st_nlink;
    uint64_t st_uid;
    uint64_t st_gid;
    size_t st_dev;
    size_t st_ino;
    size_t st_size;
    uint64_t st_mode;
    time_t st_atime; // last access time
    time_t st_mtime; // last modification time
    time_t st_ctime; // last sttus change type
};

int fstat(int fd, struct stat *buf);
#endif