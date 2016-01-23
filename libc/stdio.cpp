/*
 * Copyright 2009-2012 Srijan Kumar Sharma
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
#include <native_sync.h>
uint32_t x = 0, y = 0;

void clrscr() {
    x = 0;
    y = 0;
    for (int i = 0; i < 80 * 25; i++)
        printf(" ");
    x = 0;
    y = 0;
}

static void print_num(uint32_t arg, uint32_t base) {
    char str[12] = {0};
    str[11] = '0';
    char symbol[] = {"0123456789ABCDEFG"};
    for (uint32_t i = arg, no = 11; (i > 0)&&(no >= 0); i /= base, no--) {
        str[no] = symbol[(i % base)];
    }

    for (int i = 0; i < 12; ++i) {
        if (str[i] != 0) {
            putcharacter(str[i], x, y);
            x++;
            y += (x / 80);
            x %= 80;
        }
    }
}

mtx_t printf_mutex;

void printf(const char *format, ...) {
    sync atomic_printf(&printf_mutex);
    va_list ap;
    va_start(ap, format);

    for (uint32_t i = 0; format[i] != 0; i++) {
        switch (format[i]) {
            case '%':
                i++;
                switch (format[i]) {
                    case 'x':
                    {
                        uint32_t arg = va_arg(ap, uint32_t);
                        print_num(arg, 16);
                        break;
                    }
                    case 'd':
                    {
                        uint32_t arg = va_arg(ap, uint32_t);
                        print_num(arg, 10);
                        break;
                    }
                    case 's':
                    {
                        char* str = va_arg(ap, char*);
                        while (str[0] != 0) {

                            putcharacter(str++[0], x, y);
                            x++;
                            y += (x / 80);
                            x %= 80;
                        }
                        break;

                    }
                }
                break;
            case '\n':
                y += 1;
                x = 0;
                break;
            default:
                putcharacter(format[i], x, y);
                x++;
                y += (x / 80);
                x %= 80;
                break;
        }
        if (y > 24) {
            y = 24;
            x = 0;
            scroll();
        }
    }
    va_end(ap);
}
