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

#ifndef STDIO_H
#define STDIO_H
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

#define EOF (-1)
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define DBG_OUTPUT printf("%s:%s():%d\n", __FILE__, __FUNCTION__, __LINE__);
#ifdef __cplusplus
extern "C"
{
#endif
    char getchar(void);
    int printf(const char *format, ...);
    int vprintf(const char *format, va_list arg);
    int vsnprintf(char *buffer, size_t n, const char *format, va_list arg);
    int snprintf(char *str, size_t n, const char *format, ...);

    char *gets_s(char *str, size_t sz);
    int putchar(int c);
    void clrscr(void);
#if __STDC_HOSTED__ == 1
#ifndef FILE
#define FILE void
#endif //FILE

    extern FILE *stdin, *stdout, *stderr;
    FILE *fopen(const char *filename, const char *mode);
    int fclose(FILE *file);
    int ferror(FILE *file);
#endif
#ifdef __cplusplus
}
#endif
#define assert(x)                                                                \
    if (x)                                                                       \
    {                                                                            \
        printf("\nAssert at [%s],File [%s], Line [%d]", #x, __FILE__, __LINE__); \
        __asm__("cli;hlt;");                                                     \
    }

#endif /* STDIO_H */
