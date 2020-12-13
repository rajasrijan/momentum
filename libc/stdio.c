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

#include "stdio.h"
#include <stdarg.h>
#include <ctype.h>
#include "string.h"
#include <kernel/syscall.h>
#include <errno.h>
#if __STDC_HOSTED__ == 0
#include <native_sync.h>
#include <stddef.h>
#include <arch/x86_64/video.h>
#else
#include <fcntl.h>
#endif

#if __STDC_HOSTED__ == 0
uint32_t x = 0, y = 0;
void clrscr()
{
    x = 0;
    y = 0;
    for (int i = 0; i < text_width * text_height; i++)
        printf(" ");
    x = 0;
    y = 0;
}

int putchar(int c)
{
    if (!putcharacter || !scroll)
        return 0;

    if (c == '\n')
    {
        y += 1;
        x = 0;
    }
    else if (c == '\t')
    {
        for (int i = 0; i < 8; i++)
        {
            putcharacter(' ', x, y);
            x++;
            y += (x / text_width);
            x %= text_width;
        }
    }
    else if (c == '\b')
    {
        if (x > 0)
        {
            x--;
            putcharacter(' ', x, y);
        }
    }
    else
    {
        putcharacter(c, x, y);
        x++;
        y += (x / text_width);
        x %= text_width;
    }
    if (y > (text_height - 1))
    {
        y = (text_height - 1);
        x = 0;
        scroll();
    }
    return c;
}
#else
FILE *stdin = NULL, *stdout = NULL, *stderr = NULL;
int putchar(int c)
{
    __asm__ volatile("syscall" ::"D"(SYSCALL_PUTCHAR), "S"(c), "d"(0)
                     : "rcx", "r11");
    return c;
}
char getchar()
{
    uint64_t ch = 0;
    __asm__ volatile("syscall"
                     : "=a"(ch)
                     : "D"(SYSCALL_GETCHAR), "S"(NULL), "d"(0)
                     : "rcx", "r11");
    return ch;
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
        errno = ENOSYS;
        return NULL;
    }
    return (FILE *)fd;
}
#endif
static void print_num32(uint32_t arg, uint32_t base)
{
    char str[12] = {0};
    str[11] = '0';
    char symbol[] = {"0123456789ABCDEFG"};
    for (uint32_t i = arg, no = 11; (i > 0) && (no >= 0); i /= base, no--)
    {
        str[no] = symbol[(i % base)];
    }

    for (int i = 0; i < 12; ++i)
    {
        if (str[i] != 0)
        {
            putchar(str[i]);
        }
    }
}
static void print_num64(uint64_t arg, uint32_t base)
{
    char str[24] = {0};
    str[23] = '0';
    char symbol[] = {"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
    for (uint64_t i = arg, no = 23; (i > 0) && (no >= 0); i /= base, no--)
    {
        str[no] = symbol[(i % base)];
    }

    for (int i = 0; i < 24; ++i)
    {
        if (str[i] != 0)
        {
            putchar(str[i]);
        }
    }
}

int printf(const char *format, ...)
{
#if __STDC_HOSTED__ == 0
    static mtx_t lock = 0;
    mtx_lock(&lock);
#endif

    int ret = 0;
    va_list arg;
    char buffer[1024];
    va_start(arg, format);
    ret = vsnprintf(buffer, sizeof(buffer) / sizeof(buffer[0]), format, arg);
    for (int i = 0; i < ret; i++)
    {
        putchar(buffer[i]);
    }
    va_end(arg);
#if __STDC_HOSTED__ == 0
    mtx_unlock(&lock);
#endif
    return 0;
}

int vprintf(const char *format, va_list arg)
{
#if __STDC_HOSTED__ == 0
    static mtx_t lock = 0;
    mtx_lock(&lock);
#endif

    int ret = 0;
    char buffer[1024];
    ret = vsnprintf(buffer, sizeof(buffer) / sizeof(buffer[0]), format, arg);
    for (int i = 0; i < ret; i++)
    {
        putchar(buffer[i]);
    }
#if __STDC_HOSTED__ == 0
    mtx_unlock(&lock);
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
