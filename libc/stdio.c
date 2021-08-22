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

#include "stdio.h"
#include <stdarg.h>
#include <ctype.h>
#include "string.h"
#include <kernel/syscall.h>
#include <errno.h>
#if __STDC_HOSTED__ == 0
#include <native_sync.h>
#include <stddef.h>
#include <arch/video.h>
#else
#include <fcntl.h>
#include <unistd.h>
#endif

#if __STDC_HOSTED__ == 0
int putchar(int c)
{
    char ch[2] = {};
    ch[0] = c;
    printString(ch);
    return c;
}
#if _arch_efi_
char getchar()
{
    uint64_t ch = 0;
    return ch;
}
#endif
#else
struct FILE_IO __stds[3] = {{STDIN_FILENO, NULL, NULL}, {STDOUT_FILENO, NULL, NULL}, {STDERR_FILENO, NULL, NULL}};
FILE *stdin = &__stds[STDIN_FILENO], *stdout = &__stds[STDOUT_FILENO], *stderr = &__stds[STDERR_FILENO];
int putchar(int c)
{
    __asm__ volatile("syscall" ::"D"(SYSCALL_PUTCHAR), "S"(c), "d"(0) : "rcx", "r11");
    return c;
}
int putchar_unlocked(int c)
{
    __asm__ volatile("syscall" ::"D"(SYSCALL_PUTCHAR), "S"(c), "d"(0) : "rcx", "r11");
    return c;
}
char getchar()
{
    uint64_t ch = 0;
    __asm__ volatile("syscall" : "=a"(ch) : "D"(SYSCALL_GETCHAR), "S"(NULL), "d"(0) : "rcx", "r11");
    return ch;
}
void printString(const char *ch)
{
    while (*ch++)
    {
        putchar(*ch);
    }
}
struct file_descriptor
{
    int fd;
};

FILE *fopen(const char *filename, const char *mode)
{
    int oflag = 0;
    if (mode[0] == 'w')
    {
        oflag |= O_WRONLY | O_CREAT;
    }

    int fd = open(filename, oflag);
    if (fd < 0)
    {
        asm("cli;hlt");
        errno = ENOSYS;
        return NULL;
    }
    return (FILE *)fd;
}
int ferror(FILE *file)
{
    asm("cli;hlt");
    return -ENOSYS;
}
int fclose(FILE *file)
{
    asm("cli;hlt");
    return -ENOSYS;
}

int fprintf(FILE *file, const char *format, ...)
{
    asm("cli;hlt");
    return -ENOSYS;
}
int fflush(FILE *stream)
{
    //   caching not implemented yet. nothing to flush.
    return 0;
}
int vfprintf(FILE *stream, const char *format, va_list arg)
{
    asm("cli;hlt");
    return -ENOSYS;
}

size_t fwrite(const void *ptr, size_t size, size_t count, FILE *stream)
{
    int ret = 0;
    ret = write(stream->_fileno, ptr, size * count);
    return ret;
}
int fputs_unlocked(const char *str, FILE *stream)
{
    return fwrite(str, 1, strlen(str), stream);
}

int fputs(const char *str, FILE *stream)
{
    return fwrite(str, 1, strlen(str), stream);
}
int puts(const char *str)
{
    printString(str);
    return 0;
}
#endif

#if __STDC_HOSTED__ == 0
mtx_t printf_lock = 0;
#endif
int printf(const char *format, ...)
{
#if __STDC_HOSTED__ == 0
    mtx_lock(&printf_lock);
#endif

    int ret = 0;
    va_list arg;
    char buffer[1024];
    va_start(arg, format);
    ret = vsnprintf(buffer, sizeof(buffer) / sizeof(buffer[0]), format, arg);
    printString(buffer);
    va_end(arg);
#if __STDC_HOSTED__ == 0
    mtx_unlock(&printf_lock);
#endif
    return 0;
}

int vprintf(const char *format, va_list arg)
{
#if __STDC_HOSTED__ == 0
    mtx_lock(&printf_lock);
#endif

    int ret = 0;
    char buffer[1024];
    ret = vsnprintf(buffer, sizeof(buffer) / sizeof(buffer[0]), format, arg);
    buffer[ret] = 0;
    printString(buffer);
#if __STDC_HOSTED__ == 0
    mtx_unlock(&printf_lock);
#endif
    return 0;
}
int vasprintf(char **s, const char *format, va_list arg)
{
#if __STDC_HOSTED__ == 0
    mtx_lock(&printf_lock);
#endif
    int ret = 0;
    *s = (char *)malloc(1024);
    ret = vsnprintf(*s, 1024, format, arg);
#if __STDC_HOSTED__ == 0
    mtx_unlock(&printf_lock);
#endif
    return 0;
}

char *gets_s(char *str, size_t sz)
{
    size_t it = 0;
    char ch = 0;
    do
    {
        ch = getchar();
        switch (ch)
        {
        case '\b':
            it--;
            str[it] = 0;
            break;
        default:
            str[it] = ch;
            it++;
            break;
        }
    } while (it < sz && ch != '\n');
    if (it >= 1)
        str[it - 1] = 0;
    return str;
}

int kisnum(const char ch)
{
    return (ch >= '0' && ch <= '9');
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}
