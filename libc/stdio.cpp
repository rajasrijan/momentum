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

#include "stdio.h"
#include <stdarg.h>
#include "string.h"
#include <native_sync.h>
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

static void print_num(uint32_t arg, uint32_t base)
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
			putcharacter(str[i], x, y);
			x++;
			y += (x / 80);
			x %= 80;
		}
	}
}
static void print_num(uint64_t arg, uint32_t base)
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
			putcharacter(str[i], x, y);
			x++;
			y += (x / 80);
			x %= 80;
		}
	}
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
			y += (x / 80);
			x %= 80;
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
		y += (x / 80);
		x %= 80;
	}
	if (y > 24)
	{
		y = 24;
		x = 0;
		scroll();
	}
	return c;
}

void printf(const char *format, ...)
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
					print_num(arg, 16);
				}
				break;
			}
			case 'x':
			{
				uint64_t arg = va_arg(ap, uint64_t);
				print_num(arg, 16);
				break;
			}
			case 'd':
			{
				uint64_t arg = va_arg(ap, uint64_t);
				print_num(arg, 10);
				break;
			}
			case 's':
			{
				char *str = va_arg(ap, char *);
				while (str[0] != 0)
				{

					putcharacter(str++ [0], x, y);
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
		case '\t':
			for (size_t it = 0; it < 4; it++)
			{
				putcharacter(' ', x, y);
				x++;
				y += (x / 80);
				x %= 80;
			}
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

char *gets_s(char *str, size_t sz)
{
	int it = 0;
	char ch = 0;
	do
	{
		ch = getchar();
		str[it] = ch;
		it++;
	} while (it < sz && ch != '\n');
	if (it >= 1)
		str[it - 1] = 0;
}
bool kisnum(const char ch)
{
	return (ch >= '0' && ch <= '9');
}

template <typename _type, typename fetch = _type>
void kprint_number(char *alpha_number, int alpha_number_count, int &alpha_number_it, bool &is_negative, va_list arg, const int base, const char *number_representation)
{
	_type number = va_arg(arg, fetch);
	if (number < 0)
	{
		number *= -1;
		is_negative = true;
	}
	while (alpha_number_it < alpha_number_count)
	{
		alpha_number[alpha_number_it] = number_representation[(number % base)];
		number /= base;
		alpha_number_it++;
		if (number == 0)
			break;
	}
}

int vsprintf(char *buffer, const char *format, va_list arg)
{
	int ret = 0;
	char *output_buffer = buffer;
	const char *ch_it = format;
	do
	{
		if (*ch_it == '%' && *++ch_it != 0)
		{
			if (*ch_it == '%')
			{
				*output_buffer++ = *ch_it;
			}
			else
			{
				bool insert_space_if_no_sign_is_present = (*ch_it == ' ');
				bool left_justify = (*ch_it == '-');
				bool force_sign = (*ch_it == '+');
				bool force_special_suffixes = (*ch_it == '#');
				bool left_pad_with_zeros = (*ch_it == '0');

				bool width_specified = false;
				int width = 0;
				bool precision_specified = false;
				int precision = 0;

				if (insert_space_if_no_sign_is_present || left_justify ||
					force_sign || force_special_suffixes || left_pad_with_zeros)
				{
					ch_it++;
				}

				if (*ch_it != 0)
				{
					if (kisnum(*ch_it))
					{
						if (force_sign && *ch_it == '0')
							left_pad_with_zeros = true;

						width_specified = true;
						do
						{
							int num = (int)(*ch_it - '0');
							width *= 10;
							width += num;
						} while (*++ch_it != 0 && kisnum(*ch_it));
					}
					if (*ch_it != 0 && *ch_it == '.' && *++ch_it != 0 && kisnum(*ch_it))
					{
						precision_specified = true;
						do
						{
							int num = (int)(*ch_it - '0');
							precision *= 10;
							precision += num;
						} while (*++ch_it != 0 && kisnum(*ch_it));
					}
				}
				//  length
				int length_type = 0; // 0 is nothing specified
				if (*ch_it != 0)
				{
					if (ch_it[0] == 'h' && ch_it[1] == 'h')
					{
						length_type = 1;
						ch_it += 2;
					}
					else if (ch_it[0] == 'h')
					{
						length_type = 2;
						ch_it += 1;
					}
					else if (ch_it[0] == 'l' && ch_it[1] == 'l')
					{
						length_type = 4;
						ch_it += 2;
					}
					else if (ch_it[0] == 'l')
					{
						length_type = 3;
						ch_it += 1;
					}
					else if (ch_it[0] == 'j')
					{
						length_type = 5;
						ch_it += 1;
					}
					else if (ch_it[0] == 'z')
					{
						length_type = 6;
						ch_it += 1;
					}
					else if (ch_it[0] == 't')
					{
						length_type = 7;
						ch_it += 1;
					}
					else if (ch_it[0] == 'L')
					{
						length_type = 8;
						ch_it += 1;
					}
				}
				if (*ch_it != 0 &&
					(*ch_it == 'd' || *ch_it == 'i' ||
					 *ch_it == 'u' || *ch_it == 'o' || *ch_it == 'x' || *ch_it == 'X' ||
					 *ch_it == 'c' ||
					 *ch_it == 's' ||
					 *ch_it == 'p' ||
					 *ch_it == 'n'))
				{
					char alpha_number[32] = {0};
					const char *str_to_print = nullptr;
					int alpha_number_it = 0;
					bool is_negative = false;
					char specifier = *ch_it;

					if (*ch_it == 'd' || *ch_it == 'i')
					{
						char number_representation[] = "0123456789";
						if (length_type == 0)
						{
							kprint_number<int>(alpha_number, sizeof(alpha_number) / sizeof(alpha_number[0]), alpha_number_it, is_negative, arg, 10, number_representation);
						}
						else if (length_type == 1)
						{
							kprint_number<signed char, int>(alpha_number, sizeof(alpha_number) / sizeof(alpha_number[0]), alpha_number_it, is_negative, arg, 10, number_representation);
						}
						else if (length_type == 2)
						{
							kprint_number<short int, int>(alpha_number, sizeof(alpha_number) / sizeof(alpha_number[0]), alpha_number_it, is_negative, arg, 10, number_representation);
						}
						else if (length_type == 3)
						{
							kprint_number<long int>(alpha_number, sizeof(alpha_number) / sizeof(alpha_number[0]), alpha_number_it, is_negative, arg, 10, number_representation);
						}
						else if (length_type == 4)
						{
							kprint_number<long long int>(alpha_number, sizeof(alpha_number) / sizeof(alpha_number[0]), alpha_number_it, is_negative, arg, 10, number_representation);
						}
						else if (length_type == 5)
						{
							kprint_number<intmax_t>(alpha_number, sizeof(alpha_number) / sizeof(alpha_number[0]), alpha_number_it, is_negative, arg, 10, number_representation);
						}
						else if (length_type == 6)
						{
							kprint_number<ssize_t>(alpha_number, sizeof(alpha_number) / sizeof(alpha_number[0]), alpha_number_it, is_negative, arg, 10, number_representation);
						}
						else if (length_type == 7)
						{
							kprint_number<uint64_t>(alpha_number, sizeof(alpha_number) / sizeof(alpha_number[0]), alpha_number_it, is_negative, arg, 10, number_representation);
						}
						str_to_print = alpha_number;
					}
					else if (*ch_it == 'x' || *ch_it == 'X')
					{
						char number_representation_capital[] = "0123456789ABCDEF";
						char number_representation_small[] = "0123456789abcdef";
						char *number_representation = nullptr;
						if (*ch_it == 'X')
						{
							number_representation = number_representation_capital;
						}
						else
						{
							number_representation = number_representation_small;
						}
						if (length_type == 0)
						{
							kprint_number<unsigned int>(alpha_number, sizeof(alpha_number) / sizeof(alpha_number[0]), alpha_number_it, is_negative, arg, 0x10, number_representation);
						}
						else if (length_type == 1)
						{
							kprint_number<unsigned char, int>(alpha_number, sizeof(alpha_number) / sizeof(alpha_number[0]), alpha_number_it, is_negative, arg, 0x10, number_representation);
						}
						else if (length_type == 2)
						{
							kprint_number<unsigned short int, int>(alpha_number, sizeof(alpha_number) / sizeof(alpha_number[0]), alpha_number_it, is_negative, arg, 0x10, number_representation);
						}
						else if (length_type == 3)
						{
							kprint_number<unsigned long int>(alpha_number, sizeof(alpha_number) / sizeof(alpha_number[0]), alpha_number_it, is_negative, arg, 0x10, number_representation);
						}
						else if (length_type == 4)
						{
							kprint_number<unsigned long long int>(alpha_number, sizeof(alpha_number) / sizeof(alpha_number[0]), alpha_number_it, is_negative, arg, 0x10, number_representation);
						}
						else if (length_type == 5)
						{
							kprint_number<uintmax_t>(alpha_number, sizeof(alpha_number) / sizeof(alpha_number[0]), alpha_number_it, is_negative, arg, 0x10, number_representation);
						}
						else if (length_type == 6)
						{
							kprint_number<size_t>(alpha_number, sizeof(alpha_number) / sizeof(alpha_number[0]), alpha_number_it, is_negative, arg, 0x10, number_representation);
						}
						else if (length_type == 7)
						{
							kprint_number<uint64_t>(alpha_number, sizeof(alpha_number) / sizeof(alpha_number[0]), alpha_number_it, is_negative, arg, 0x10, number_representation);
						}
						str_to_print = alpha_number;
					}
					else if (*ch_it == 's')
					{
						str_to_print = va_arg(arg, char *);
						alpha_number_it = strlen(str_to_print);
					}
					int tmp_length = alpha_number_it + (int)((is_negative || insert_space_if_no_sign_is_present || force_sign) && (specifier != 'x' && specifier != 'X') && specifier != 's');

					if (!left_justify && !left_pad_with_zeros && width_specified && width > tmp_length)
					{
						for (size_t i = 0; i < width - tmp_length; i++)
						{
							*output_buffer++ = ' ';
						}
					}
					if ((specifier != 'x' && specifier != 'X') && specifier != 's')
					{
						if (is_negative)
						{
							*output_buffer++ = '-';
						}
						else if (insert_space_if_no_sign_is_present && !is_negative)
						{
							*output_buffer++ = ' ';
						}
						else if (force_sign && !is_negative)
						{
							*output_buffer++ = '+';
						}
					}
					if (left_pad_with_zeros && width_specified && width > tmp_length)
					{
						for (size_t i = 0; i < width - tmp_length; i++)
						{
							*output_buffer++ = '0';
						}
					}
					for (size_t i = 0; i < alpha_number_it; i++)
					{
						if (specifier == 's')
						{
							*output_buffer++ = str_to_print[i];
						}
						else
						{
							*output_buffer++ = str_to_print[alpha_number_it - i - 1];
						}
					}
					if (left_justify && width_specified && width > tmp_length)
					{
						for (size_t i = 0; i < width - tmp_length; i++)
						{
							*output_buffer++ = ' ';
						}
					}
				}
			}
		}
		else
		{
			*output_buffer++ = *ch_it;
		}
		ret++;
	} while (*ch_it++ != 0);
	return ret;
}

int sprintf(char *str, const char *format, ...)
{
	int ret = 0;
	va_list arg;
	va_start(arg, format);
	ret = vsprintf(str, format, arg);
	va_end(arg);
	return ret;
}