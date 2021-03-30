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

#ifndef DIRENT_H
#define DIRENT_H
#include <errno.h>
#include <stddef.h>
#include <limits.h>
#include <sys/types.h>

#define MAXNAMLEN NAME_MAX

struct dirent {
    char d_name[NAME_MAX];
    unsigned char d_namlen;
};

typedef struct _dir {
    int fd;
    struct dirent *dirent_list;
    size_t dirent_count;
    size_t dirent_index;
} DIR;
#if __STDC_HOSTED__ == 1
DIR *opendir(const char *dir_name);
DIR *fdopendir(int fd);
struct dirent *readdir(DIR *dirp);
void rewinddir(DIR *__dirp);
void seekdir(DIR *__dirp, long int __pos);
long int telldir(DIR *__dirp);
int dirfd(DIR *__dirp);
int scandir(const char *__dir, struct dirent ***__namelist, int (*__selector)(const struct dirent *), int (*__cmp)(const struct dirent **, const struct dirent **));
int scandirat(int __dfd, const char *__dir, struct dirent ***__namelist, int (*__selector)(const struct dirent *), int (*__cmp)(const struct dirent **, const struct dirent **));
int alphasort(const struct dirent **__e1, const struct dirent **__e2);
ssize_t getdirentries(int __fd, char *__buf, size_t __nbytes, off_t *__basep);
int versionsort(const struct dirent **__e1, const struct dirent **__e2);
#endif //__STDC_HOSTED__
int closedir(DIR *dirp);
#endif //   DIRENT_H