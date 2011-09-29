/*
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
uint32_t x = 0, y = 0;

void clrscr()
{
    x = 0;
    y = 0;
    for (int i = 0; i < 80 * 25; i++)
        printf(" ");
    x = 0;
    y = 0;
}

static void print_hex(uint32_t arg)
{
    for (int j = 0; j < 8; j++)
    {
        uint32_t no = (arg >> ((7 - j)*4))&0x0F;
        switch (no)
        {
        case 0:
            putcharacter('0', x, y);
            x++;
            y += (x / 80);
            x %= 80;
            break;
        case 1:
            putcharacter('1', x, y);
            x++;
            y += (x / 80);
            x %= 80;
            break;
        case 2:
            putcharacter('2', x, y);
            x++;
            y += (x / 80);
            x %= 80;
            break;
        case 3:
            putcharacter('3', x, y);
            x++;
            y += (x / 80);
            x %= 80;
            break;
        case 4:
            putcharacter('4', x, y);
            x++;
            y += (x / 80);
            x %= 80;
            break;
        case 5:
            putcharacter('5', x, y);
            x++;
            y += (x / 80);
            x %= 80;
            break;
        case 6:
            putcharacter('6', x, y);
            x++;
            y += (x / 80);
            x %= 80;
            break;
        case 7:
            putcharacter('7', x, y);
            x++;
            y += (x / 80);
            x %= 80;
            break;
        case 8:
            putcharacter('8', x, y);
            x++;
            y += (x / 80);
            x %= 80;
            break;
        case 9:
            putcharacter('9', x, y);
            x++;
            y += (x / 80);
            x %= 80;
            break;
        case 0xa:
            putcharacter('a', x, y);
            x++;
            y += (x / 80);
            x %= 80;
            break;
        case 0xb:
            putcharacter('b', x, y);
            x++;
            y += (x / 80);
            x %= 80;
            break;
        case 0xc:
            putcharacter('c', x, y);
            x++;
            y += (x / 80);
            x %= 80;
            break;
        case 0xd:
            putcharacter('d', x, y);
            x++;
            y += (x / 80);
            x %= 80;
            break;
        case 0xe:
            putcharacter('e', x, y);
            x++;
            y += (x / 80);
            x %= 80;
            break;
        case 0xf:
            putcharacter('f', x, y);
            x++;
            y += (x / 80);
            x %= 80;
            break;

        }
    }
}

static void print_dec(uint32_t arg)
{
    uint32_t no, no1;
    no = 0;
    no1 = arg;
    while (no1)
    {
        no += no1 % 10;
        no1 /= 10;
        no *= 10;
    }
    no1 = 0;
    while (no)
    {
        no1 = no % 10;
        no /= 10;
        switch (no1)
        {
        case 0:
            putcharacter('0', x, y);
            x++;
            y += (x / 80);
            x %= 80;
            break;
        case 1:
            putcharacter('1', x, y);
            x++;
            y += (x / 80);
            x %= 80;
            break;
        case 2:
            putcharacter('2', x, y);
            x++;
            y += (x / 80);
            x %= 80;
            break;
        case 3:
            putcharacter('3', x, y);
            x++;
            y += (x / 80);
            x %= 80;
            break;
        case 4:
            putcharacter('4', x, y);
            x++;
            y += (x / 80);
            x %= 80;
            break;
        case 5:
            putcharacter('5', x, y);
            x++;
            y += (x / 80);
            x %= 80;
            break;
        case 6:
            putcharacter('6', x, y);
            x++;
            y += (x / 80);
            x %= 80;
            break;
        case 7:
            putcharacter('7', x, y);
            x++;
            y += (x / 80);
            x %= 80;
            break;
        case 8:
            putcharacter('8', x, y);
            x++;
            y += (x / 80);
            x %= 80;
            break;
        case 9:
            putcharacter('9', x, y);
            x++;
            y += (x / 80);
            x %= 80;
            break;
        }
    }

}

void printf(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);

    for (uint32_t i = 0; format[i] != 0; i++)
    {
        switch (format[i])
        {
        case '%':
            i++;
            switch (format[i])
            {
            case 'x':
            {
                uint32_t arg = va_arg(ap, uint32_t);
                print_hex(arg);
                break;
            }
            case 'd':
            {
                uint32_t arg = va_arg(ap, uint32_t);
                print_dec(arg);
                break;
            }
            case 's':
            {
                char* str = va_arg(ap, char*);
                while (str[0] != 0)
                {
                    
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
        if (y > 24)
        {
            y = 24;
            x = 0;
            scroll();
        }
    }
    va_end(ap);
}
