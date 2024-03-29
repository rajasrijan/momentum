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

#ifndef STAT_H
#define STAT_H

#include <sys/types.h>
#include <sys/time.h>
#include <stddef.h>

//  file permissions
#define S_IXOTH (1 << 0)
#define S_IWOTH (1 << 1)
#define S_IROTH (1 << 2)
#define S_IRWXO (S_IXOTH | S_IWOTH | S_IROTH)

#define S_IXGRP (1 << 3)
#define S_IWGRP (1 << 4)
#define S_IRGRP (1 << 5)
#define S_IRWXG (S_IXGRP | S_IWGRP | S_IRGRP)

#define S_IXUSR (1 << 6)
#define S_IWUSR (1 << 7)
#define S_IRUSR (1 << 8)
#define S_IRWXU (S_IXUSR | S_IWUSR | S_IRUSR)

#define S_ISVTX (1 << 9)
#define S_ISGID (1 << 10)
#define S_ISUID (1 << 11)

//	regular
#define S_IFREG (0100000)
#define S_IFSOCK (0140000)
//	symbolic link
#define S_IFLNK (0120000)
//	block special
#define S_IFBLK (0060000)
//	directory
#define S_IFDIR (0040000)
//	character special
#define S_IFCHR (0020000)
//	FIFO special
#define S_IFIFO (0010000)
//	type of file
#define S_IFMT (1 << 22)

#define S_ISDIR(mode) (((mode)&S_IFDIR) == S_IFDIR)

struct stat
{
    uint64_t st_nlink;
    uint64_t st_uid;
    uint64_t st_gid;
    size_t st_dev;
    size_t st_ino;
    size_t st_size;
    dev_t st_rdev; /* Device ID (if special file) */
    uint64_t st_mode;
    time_t st_atime; // last access time
    time_t st_mtime; // last modification time
    time_t st_ctime; // last sttus change type
    blkcnt_t st_blocks;
};

int fstat(int fd, struct stat *buf);
int lstat(const char *path, struct stat *buf);
int stat(const char *path, struct stat *buf);
#endif
