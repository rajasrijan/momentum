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
#pragma once
#pragma pack(push, 8)

struct open_args
{
    const char *pathname;
    int oflag;
};

struct openat_args
{
    int dirfd;
    const char *pathname;
    int flags;
};

struct getdents_args
{
    int fd;
    struct dirent *dirp;
    size_t count;
};

struct mmap_args
{
    void *addr;
    size_t len;
    int prot;
    int flags;
    int fildes;
    off_t off;
};

struct read_args
{
    int fd;
    void *buf;
    size_t count;
};

struct write_args
{
    int fd;
    const void *buf;
    size_t count;
};

struct stat_args
{
    const char *path;
    struct stat *buf;
};

struct readdir_args
{
    int fd;
    struct dirent *buf;
    size_t *buf_size;
};

#pragma pack(pop)
#include <sys/syscall.h>
