#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>

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

int vsnprintf(char *buffer, size_t n, const char *format, va_list arg)
{
    int format_index = 0, buffer_index = 0;
    enum FormatFlag flags;
    enum WidthFlags length;
    int width, precision, specifier;
    while (format[format_index] != 0 && buffer_index < n - 1)
    {
        if (format[format_index] == '%')
        {
            format_index += decode_format(&format[format_index], &flags, &width, &precision, &length, &specifier);
            if (specifier == 'd' || specifier == 'i')
            {
                if (length == WidthFlags_None)
                {
                    int number = va_arg(arg, int);
                    buffer_index += print_number(number, 10, &buffer[buffer_index], flags, width, precision, length, specifier);
                }
                else if (length == WidthFlags_l)
                {
                    long int number = va_arg(arg, long int);
                    buffer_index += print_number(number, 10, &buffer[buffer_index], flags, width, precision, length, specifier);
                }
                else if (length == WidthFlags_ll)
                {
                    long long int number = va_arg(arg, long long int);
                    buffer_index += print_number(number, 10, &buffer[buffer_index], flags, width, precision, length, specifier);
                }
            }
            else if (specifier == 'x')
            {
                if (length == WidthFlags_None)
                {
                    unsigned int number = va_arg(arg, unsigned int);
                    buffer_index += print_number(number, 16, &buffer[buffer_index], flags, width, precision, length, specifier);
                }
                else if (length == WidthFlags_l)
                {
                    unsigned long int number = va_arg(arg, unsigned long int);
                    buffer_index += print_number(number, 16, &buffer[buffer_index], flags, width, precision, length, specifier);
                }
                else if (length == WidthFlags_ll)
                {
                    unsigned long long int number = va_arg(arg, unsigned long long int);
                    buffer_index += print_number(number, 16, &buffer[buffer_index], flags, width, precision, length, specifier);
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

int snprintf(char *str, size_t n, const char *format, ...)
{
    va_list arg;
    va_start(arg, format);
    int ret = vsnprintf(str, n, format, arg);
    va_end(arg);
    return ret;
}
