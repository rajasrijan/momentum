/*
 * Copyright 2009-2019 Srijan Kumar Sharma
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
#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>

#define DBG_OUTPUT printf("%s:%s():%d\n", __FILE__, __FUNCTION__, __LINE__);
#ifdef __cplusplus
#define EXT_TYP "C"
#else
#define EXT_TYP
#endif
extern EXT_TYP char getchar(void);
extern EXT_TYP int printf(const char *format, ...);
extern EXT_TYP int vsnprintf(char *buffer, size_t n, const char *format, va_list arg);
extern EXT_TYP int snprintf(char *str, size_t n, const char *format, ...);

extern EXT_TYP char *gets_s(char *str, size_t sz);
extern EXT_TYP int putchar(int c);
extern EXT_TYP void clrscr(void);

#define assert(x)                                                                \
    if (x)                                                                       \
    {                                                                            \
        printf("\nAssert at [%s],File [%s], Line [%d]", #x, __FILE__, __LINE__); \
        __asm__("cli;hlt;");                                                     \
    }

#endif /* STDIO_H */
