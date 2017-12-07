/*
 * Copyright 2009-2017 Srijan Kumar Sharma
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

extern void putcharacter(const char ch, uint32_t x, uint32_t y);
extern void scroll(void);
extern void setColor(uint8_t c);
extern char getchar(void);
extern "C" void printf(const char *format, ...);
extern "C" int vsprintf(char *buffer, const char *format, va_list arg);
extern "C" int sprintf ( char * str, const char * format, ... );
char *gets_s(char *str, size_t sz);
int putchar(int c);
void clrscr(void);

#define assert(x)                                                                \
    if (x != 0)                                                                  \
    {                                                                            \
        printf("\nAssert at [%s],File [%s], Line [%d]", #x, __FILE__, __LINE__); \
        __asm__("cli;hlt;");                                                     \
    }

#endif /* STDIO_H */
