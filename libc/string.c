/*
 * Copyright 2009-2021 Srijan Kumar Sharma
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

void *memset(void *ptr, int value, size_t num)
{
    for (size_t i = 0; i < num; i++)
    {
        ((char *)ptr)[i] = (char)(value);
    }
    return ptr;
}

void *memcpy(void *dst, const void *src, size_t len)
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

size_t strlen(const char *str)
{
    size_t i;
    for (i = 0; str[i] != 0; i++)
        ;
    return i;
}

char stricmp(const char *s1, const char *s2)
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

int strcmp(const char *s1, const char *s2)
{
    size_t i = 0;
    do
    {
        if ((s1[i] - s2[i]) != 0)
            return (char)(s1[i] - s2[i]);
    } while ((s1[i] != 0) && (s2[i] != 0) && ++i);
    return 0;
}

char *strstr(const char *str1, const char *str2)
{
    for (int i = 0; str1[i] != 0; i++)
    {
        if (strcmp(&str1[i], str2) == 0)
        {
            return (char *)&str1[i];
        }
    }
    return NULL;
}

int memcmp(const void *ptr1, const void *ptr2, size_t num)
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

void *mempcpy(void *dest, const void *src, size_t n)
{
    return memcpy(dest, src, n) + 1;
}

char *strcpy(char *dst, const char *src)
{
    int i = 0;
    for (i = 0; src[i] != 0; i++)
    {
        dst[i] = src[i];
    }
    dst[i] = 0;
    return dst;
}

char *strcat(char *dst, const char *src)
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

const char *strrchar(const char *s1, int character)
{
    for (int64_t len = strlen(s1); len >= 0; len--)
    {
        if (s1[len] == character)
        {
            return &s1[len];
        }
    }
    return NULL;
}

const char *strchar(const char *s1, int character)
{
    for (size_t len = 0; s1[len]; len++)
    {
        if (s1[len] == character)
        {
            return &s1[len];
        }
    }
    return NULL;
}

char *strncpy(char *dst, const char *src, size_t count)
{
    int srcEnded = 0;
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
                srcEnded = 1;
            else
                src++;
        }
        dst++;
    }
    *dst = 0;
    return dst;
}

int strncmp(const char *str1, const char *str2, size_t num)
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

char *strncat(char *destination, const char *source, size_t num)
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

char *strtok(char *str, const char *delimiters)
{
    static char *ptr = NULL;
    if (str != NULL)
    {
        ptr = str;
    }
    if (!ptr || *ptr == 0)
    {
        return NULL;
    }
    char *result = ptr;
    int delimFound = 0;
    while (*ptr != 0 && !delimFound)
    {
        for (size_t i = 0; delimiters[i] != 0; i++)
        {
            if (*ptr == delimiters[i])
            {
                *ptr = 0;
                delimFound = 1;
                break;
            }
        }
        ++ptr;
    }

    if (!delimFound && *ptr != 0)
    {
        return NULL;
    }

    return result;
}

char *strchrnul(const char *s, int c)
{
    const char *tmp = s;
    while (*tmp != 0)
    {
        if (*tmp == c)
            return tmp;
    }
    return tmp;
}

void *memccpy(void *pVoid, const void *pVoid1, int i1, size_t st1)
{
    __asm__ __volatile__("cli;hlt");
    return NULL;
}
void *memchr(const void *pVoid, int i1, size_t s1)
{
    __asm__ __volatile__("cli;hlt");
    return NULL;
}
void *memmove(void *pVoid, const void *pVoid1, size_t s1)
{
    __asm__ __volatile__("cli;hlt");
    return NULL;
}
char *strchr(const char *c1, int i1)
{
    __asm__ __volatile__("cli;hlt");
    return NULL;
}
int strcoll(const char *s1, const char *s2)
{
    __asm__ __volatile__("cli;hlt");
    return 0;
}
size_t strcspn(const char *s1, const char *s2)
{
    __asm__ __volatile__("cli;hlt");
    return 0;
}
char *strpbrk(const char *s1, const char *s2)
{
    __asm__ __volatile__("cli;hlt");
    return NULL;
}
char *strrchr(const char *src, int c)
{
    char *latest_found = NULL;
    for (size_t i = 0; src[i] != 0; i++)
    {
        if (src[i] == c)
            latest_found = &src[i];
    }
    return latest_found;
}
size_t strspn(const char *s1, const char *s2)
{
    __asm__ __volatile__("cli;hlt");
    return 0;
}
char *strtok_r(char *s1, const char *s2, char **ss3)
{
    __asm__ __volatile__("cli;hlt");
    return NULL;
}
size_t strxfrm(char *s1, const char *s2, size_t s3)
{
    __asm__ __volatile__("cli;hlt");
    return 0;
}
