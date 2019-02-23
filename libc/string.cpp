/*
 * Copyright 2009-2018 Srijan Kumar Sharma
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

#include "string.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

extern "C" void *memset(void *ptr, int value, size_t num)
{
    for (size_t i = 0; i < num; i++)
    {
        ((char *)ptr)[i] = (char)(value);
    }
    return ptr;
}

extern "C" void *memcpy(void *dst, const void *src, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        ((char *)dst)[i] = ((char *)src)[i];
    }
    return dst;
}

char *strdup(const char *s1)
{
    char *ret = (char *)malloc(strlen(s1) + 1);
    memcpy(ret, s1, strlen(s1) + 1);
    return ret;
}

extern "C" size_t strlen(const char *str)
{
    size_t i;
    for (i = 0; str[i] != 0; i++)
        ;
    return i;
}

extern "C" char stricmp(const char *s1, const char *s2)
{
    char diff = 0;
    size_t i = 0;
    do
    {
        diff = tolower(s1[i]) - tolower(s2[i]);
        if (diff)
            break;
    } while (s1[i] != 0 && s2[i++]);
    return diff;
}

char strcmp(const char *s1, const char *s2)
{
    size_t i = 0;
    do
    {
        if ((s1[i] - s2[i]) != 0)
            return (char)(s1[i] - s2[i]);
    } while ((s1[i] != 0) && (s2[i] != 0) && ++i);
    return 0;
}

extern "C" const char *strstr(const char *str1, const char *str2)
{
    for (int i = 0; str1[i] != 0; i++)
    {
        if (strcmp(&str1[i], str2) == 0)
        {
            return &str1[i];
        }
    }
    return nullptr;
}

extern "C" int memcmp(const void *ptr1, const void *ptr2, size_t num)
{
    int ret = 0;
    for (size_t i = 0; i < num; i++)
    {
        ret = ((char *)ptr1)[i] != ((char *)ptr2)[i];
        if (ret != 0)
        {
            break;
        }
    }
    return ret;
}

extern "C" char *strcpy(char *dst, const char *src)
{
    int i = 0;
    for (i = 0; src[i] != 0; i++)
    {
        dst[i] = src[i];
    }
    dst[i] = 0;
    return &dst[i];
}

extern "C" char *strcat(char *dst, const char *src)
{
    for (; dst[0] != 0; dst++)
        ;
    int i = 0;
    for (i = 0; src[i] != 0; i++)
    {
        dst[i] = src[i];
    }
    dst[i] = 0;
    return &dst[i];
}

extern "C" const char *strrchar(const char *s1, int character)
{
    for (size_t len = strlen(s1); len >= 0; len--)
    {
        if (s1[len] == character)
        {
            return &s1[len];
        }
    }
    return nullptr;
}

extern "C" const char *strchar(const char *s1, int character)
{
    for (size_t len = 0; s1[len]; len++)
    {
        if (s1[len] == character)
        {
            return &s1[len];
        }
    }
    return nullptr;
}

extern "C" char *strncpy(char *dst, const char *src, size_t count)
{
    bool srcEnded = false;
    for (size_t i = 0; i < count; i++)
    {
        if (srcEnded)
        {
            *dst = 0;
        }
        else
        {
            *dst = *src;
            if (*src == 0)
                srcEnded = true;
            else
                src++;
        }
        dst++;
    }
    return dst;
}

extern "C" int strncmp(const char *str1, const char *str2, size_t num)
{
    for (size_t i = 0; i < num; i++)
    {
        if ((str1[i] == 0) || (str2[i] == 0) || ((str1[i] - str2[i]) != 0))
        {
            return (str1[i] - str2[i]);
        }
    }
    return 0;
}

extern "C" char *strncat(char *destination, const char *source, size_t num)
{
    for (; *destination; destination++)
        ;
    for (size_t i = 0; i < num && source[i] != 0; i++)
    {
        *destination++ = source[i];
    }
    *destination = 0;
    return destination;
}

extern "C" char *strtok(char *str, const char *delimiters)
{
    static char *ptr = nullptr;
    if (str != nullptr)
    {
        ptr = str;
    }
    if (!ptr || *ptr == 0)
    {
        return nullptr;
    }
    auto result = ptr;
    bool delimFound = false;
    while (*ptr != 0 && !delimFound)
    {
        for (size_t i = 0; delimiters[i] != 0; i++)
        {
            if (*ptr == delimiters[i])
            {
                *ptr = 0;
                delimFound = true;
                break;
            }
        }
        ++ptr;
    }

    if (!delimFound && *ptr != 0)
    {
        return nullptr;
    }

    return result;
}

extern "C" unsigned long long to_uuid(const char *str)
{
    unsigned long long result = 0;
    auto char_to_hex = [](char c) -> int {
        return (c >= '0' && c <= '9') ? (int)(c - '0') : (c >= 'a' && c <= 'f') ? (int)(c - 'a' + 10) : -1;
    };

    if (strlen(str) == 9)
    {
        /* fat32 uuid format */
        for (size_t i = 0; str[i] != 0; i++)
        {
            if (str[i] == '-')
                continue;
            int hex = char_to_hex(tolower(str[i]));
            if (hex == -1)
            {
                return 0;
            }
            result = (result << 4) | hex;
        }
        result <<= 48;
    }
    else if (strlen(str) == 36)
    {
        /* standard uuid format */
        asm("cli;hlt;");
    }
    return result;
}