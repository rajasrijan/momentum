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

#include "stdio.h"
#include <stdarg.h>
#include <ctype.h>
#include "string.h"
#include <kernel/syscall.h>
#if __STDC_HOSTED__ == 0
#include <native_sync.h>
#include <arch/x86_64/video.h>
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
int putchar(int c)
{
	__asm__ volatile("syscall" ::"D"(SYSCALL_PUTCHAR), "S"(c));
	return c;
}
char getchar()
{
	return 0;
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
	va_list ap;
	va_start(ap, format);
	for (uint64_t i = 0; format[i] != 0; i++)
	{
		switch (format[i])
		{
		case '%':
			i++;
			switch (format[i])
			{
			case 'l':
			{
				i++;
				if (format[i] == 'x')
				{
					uint64_t arg = va_arg(ap, uint64_t);
					print_num64(arg, 16);
				}
				break;
			}
			case 'x':
			{
				uint64_t arg = va_arg(ap, uint64_t);
				print_num32(arg, 16);
				break;
			}
			case 'd':
			{
				uint64_t arg = va_arg(ap, uint64_t);
				print_num32(arg, 10);
				break;
			}
			case 's':
			{
				char *str = va_arg(ap, char *);
				while (str[0] != 0)
				{
					putchar(*str++);
				}
				break;
			}
			}
			break;
		case '\n':
		case '\r':
		case '\t':
		default:
			putchar(format[i]);
			break;
		}
	}
	va_end(ap);
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

enum FormatFlag
{
	FormatFlag_None,
	FormatFlag_LeftJustify,
	FormatFlag_PrependSign,
	FormatFlag_BlankIfNoSign,
	FormatFlag_PrependBaseNotifier,
	FormatFlag_PadZeros
};
enum WidthFlags
{
	WidthFlags_None,
	WidthFlags_hh,
	WidthFlags_h,
	WidthFlags_l,
	WidthFlags_ll,
	WidthFlags_j,
	WidthFlags_z,
	WidthFlags_t,
	WidthFlags_L
};
int max(int a, int b) { return (a > b) ? a : b; }
int decode_format(const char *buffer, enum FormatFlag *flags, int *width, int *precision, enum WidthFlags *length, int *specifier)
{
	//%[flags][width][.precision][length]specifier
	int buffer_index = 1;
	*flags = FormatFlag_None;
	width[0] = 0;
	precision[0] = -1;
	specifier[0] = 0;
	if (buffer[buffer_index] == '-')
	{
		*flags = FormatFlag_LeftJustify;
		buffer_index++;
	}
	else if (buffer[buffer_index] == '+')
	{
		*flags = FormatFlag_PrependSign;
		buffer_index++;
	}
	else if (buffer[buffer_index] == ' ')
	{
		*flags = FormatFlag_BlankIfNoSign;
		buffer_index++;
	}
	else if (buffer[buffer_index] == '#')
	{
		*flags = FormatFlag_PrependBaseNotifier;
		buffer_index++;
	}
	else if (buffer[buffer_index] == '0')
	{
		*flags = FormatFlag_PadZeros;
		buffer_index++;
	}
	while (isdigit(buffer[buffer_index]))
	{
		width[0] = (width[0] * 10) + (buffer[buffer_index] - '0');
		buffer_index++;
	}
	if (buffer[buffer_index] == '.')
	{
		precision[0] = 0;
		buffer_index++;
		while (isdigit(buffer[buffer_index]))
		{
			precision[0] = (precision[0] * 10) + (buffer[buffer_index] - '0');
			buffer_index++;
		}
	}
	if (buffer[buffer_index] == 'h')
	{
		if (buffer[buffer_index + 1] == 'h')
		{
			length[0] = WidthFlags_hh;
			buffer_index++;
		}
		else
			length[0] = WidthFlags_h;
		buffer_index++;
	}
	else if (buffer[buffer_index] == 'l')
	{
		if (buffer[buffer_index + 1] == 'l')
		{
			length[0] = WidthFlags_ll;
			buffer_index++;
		}
		else
			length[0] = WidthFlags_l;
		buffer_index++;
	}
	else if (buffer[buffer_index] == 'j')
	{
		length[0] = WidthFlags_j;
		buffer_index++;
	}
	else if (buffer[buffer_index] == 'z')
	{
		length[0] = WidthFlags_z;
		buffer_index++;
	}
	else if (buffer[buffer_index] == 't')
	{
		length[0] = WidthFlags_t;
		buffer_index++;
	}
	else if (buffer[buffer_index] == 'L')
	{
		length[0] = WidthFlags_L;
		buffer_index++;
	}
	else
	{
		length[0] = WidthFlags_None;
	}
	specifier[0] = buffer[buffer_index];
	buffer_index++;
	return buffer_index;
}
int print_number(int64_t number, int base, char *buffer, enum FormatFlag flags, int width, int precision, enum WidthFlags length, int specifier)
{
	char symbols[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
	char digits[256] = {0};
	int digits_index = 0, buffer_index = 0;
	int isNeg = 0;
	if (number < 0)
	{
		isNeg = 1;
		number *= -1;
	}
	if (flags == FormatFlag_PadZeros && precision != -1)
	{
		flags = FormatFlag_None;
	}
	if (precision == -1)
		precision = 0;
	do
	{
		digits[digits_index] = symbols[number % base];
		number /= base;
		digits_index++;
	} while (number > 0);
	//  add leading zeros
	precision -= digits_index;
	for (int i = 0; i < precision; i++)
	{
		digits[digits_index++] = symbols[0];
	}
	int conditional_space = 0;
	if (isNeg || (specifier != 'x' && flags == FormatFlag_BlankIfNoSign) || (specifier != 'x' && flags == FormatFlag_PrependSign))
	{
		conditional_space = 1;
	}
	if (specifier == 'x' && flags == FormatFlag_PrependBaseNotifier)
	{
		conditional_space = 2;
	}

	if ((flags != FormatFlag_PadZeros) && flags != FormatFlag_LeftJustify)
		for (int i = 0; i < max(0, width - digits_index - conditional_space); i++)
			buffer[buffer_index++] = ' ';
	if (isNeg)
	{
		buffer[buffer_index] = '-';
		buffer_index++;
	}
	else if (flags == FormatFlag_PrependSign && specifier != 'x')
	{
		buffer[buffer_index] = '+';
		buffer_index++;
	}
	else if (flags == FormatFlag_BlankIfNoSign && specifier != 'x')
	{
		buffer[buffer_index] = ' ';
		buffer_index++;
	}
	else if (specifier == 'x' && flags == FormatFlag_PrependBaseNotifier)
	{
		buffer[buffer_index++] = '0';
		buffer[buffer_index++] = 'x';
	}
	if (flags == FormatFlag_PadZeros && precision == 0)
		for (int i = 0; i < max(0, width - digits_index - conditional_space); i++)
			buffer[buffer_index++] = '0';
	for (int i = digits_index - 1; i >= 0; i--)
	{
		buffer[buffer_index] = digits[i];
		buffer_index++;
	}
	if (flags == FormatFlag_LeftJustify)
		for (int i = 0; i < max(0, width - digits_index - conditional_space); i++)
			buffer[buffer_index++] = ' ';
	return buffer_index;
}
int vsprintf(char *buffer, const char *format, va_list arg)
{
	int format_index = 0, buffer_index = 0;
	enum FormatFlag flags;
	enum WidthFlags length;
	int width, precision, specifier;
	while (format[format_index] != 0)
	{
		if (format[format_index] == '%')
		{
			format_index += decode_format(format, &flags, &width, &precision, &length, &specifier);
			if (specifier == 'd' || specifier == 'i')
			{
				if (length == WidthFlags_None)
				{
					int number = va_arg(arg, int);
					buffer_index += print_number(number, 10, buffer, flags, width, precision, length, specifier);
				}
				else if (length == WidthFlags_l)
				{
					long int number = va_arg(arg, long int);
					buffer_index += print_number(number, 10, buffer, flags, width, precision, length, specifier);
				}
				else if (length == WidthFlags_ll)
				{
					long long int number = va_arg(arg, long long int);
					buffer_index += print_number(number, 10, buffer, flags, width, precision, length, specifier);
				}
			}
			else if (specifier == 'x')
			{
				if (length == WidthFlags_None)
				{
					unsigned int number = va_arg(arg, unsigned int);
					buffer_index += print_number(number, 16, buffer, flags, width, precision, length, specifier);
				}
				else if (length == WidthFlags_l)
				{
					unsigned long int number = va_arg(arg, unsigned long int);
					buffer_index += print_number(number, 16, buffer, flags, width, precision, length, specifier);
				}
				else if (length == WidthFlags_ll)
				{
					unsigned long long int number = va_arg(arg, unsigned long long int);
					buffer_index += print_number(number, 16, buffer, flags, width, precision, length, specifier);
				}
			}
			else if (specifier == 's')
			{
				char *str = va_arg(arg, char *);
				int ch_len = 0;
				if (precision == -1)
					ch_len = strlen(str);
				else
					for (; ch_len < precision && str[ch_len] != 0; ch_len++)
						;
				for (int i = 0; i < width - ch_len; i++)
				{
					buffer[buffer_index++] = ' ';
				}
				for (int i = 0; i < ch_len; i++)
				{
					buffer[buffer_index++] = str[i];
				}
			}
			else
				goto safe_exit;
		}
		else
		{
			buffer[buffer_index] = format[format_index];
			buffer_index++;
			format_index++;
		}
	}
safe_exit:
	buffer[buffer_index] = 0;
	return buffer_index;
}
// int sprintf(char *buffer, const char *format, ...)
// {
// 	va_list ap;
// 	va_start(ap, format);
// 	int ret = vsprintf(buffer, format, ap);
// 	va_end(ap);
// 	return ret;
// }