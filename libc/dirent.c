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


#include <dirent.h>
#include <errno.h>
#include <stddef.h>
#if __STDC_HOSTED__ == 1
DIR *opendir(const char *dir_name)
{
asm("cli;hlt");
    errno = ENOSYS;
    return NULL;
}

DIR *fdopendir(int fd)
{
asm("cli;hlt");
    errno = ENOSYS;
    return NULL;
}

int closedir(DIR *dirp)
{
asm("cli;hlt");
    return -ENOSYS;
}

struct dirent *readdir(DIR *dirp)
{
asm("cli;hlt");
    errno = ENOSYS;
    return NULL;
}

void rewinddir(DIR *__dirp)
{
asm("cli;hlt");
    errno = ENOSYS;
}
void seekdir(DIR *__dirp, long int __pos)
{
asm("cli;hlt");
    errno = ENOSYS;
}
long int telldir(DIR *__dirp)
{
asm("cli;hlt");
    return -ENOSYS;
}
int dirfd(DIR *__dirp)
{
asm("cli;hlt");
    return -ENOSYS;
}
int scandir(const char *__dir, struct dirent ***__namelist, int (*__selector)(const struct dirent *), int (*__cmp)(const struct dirent **, const struct dirent **))
{
asm("cli;hlt");
    return -ENOSYS;
}
int scandirat(int __dfd, const char *__dir, struct dirent ***__namelist, int (*__selector)(const struct dirent *), int (*__cmp)(const struct dirent **, const struct dirent **))
{
asm("cli;hlt");
    return -ENOSYS;
}
int alphasort(const struct dirent **__e1, const struct dirent **__e2)
{
asm("cli;hlt");
    return -ENOSYS;
}
ssize_t getdirentries(int __fd, char *__buf, size_t __nbytes, off_t *__basep)
{
asm("cli;hlt");
    errno = ENOSYS;
    return 0;
}
int versionsort(const struct dirent **__e1, const struct dirent **__e2)
{
asm("cli;hlt");
    return -ENOSYS;
}
#endif //  __STDC_HOSTED__==1