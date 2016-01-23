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

#include "string.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

void * memset(void * ptr, int value, size_t num)
{
    for (int i = 0; i < num; i++)
    {
        ((char*) ptr)[i] = (char) (value);
    }
    return ptr;
}

char *strdup(const char *s1)
{
    char* ret = (char*) malloc(strlen(s1)+1);
    memcpy(ret, s1, strlen(s1) + 1);
    return ret;
}

size_t strlen(const char* str)
{
    size_t i;
    for (i = 0; str[i] != 0; i++);
    return i;
}

char strcmp(char* s1, char* s2)
{
    for (size_t i = 0; (s1[i] * s2[i]) != 0; i++)
        if ((s1[i] - s2[i]) != 0)
            return (char) (s1[i] - s2[i]);
    return 0;
}

int memcmp(const void * ptr1, const void * ptr2, size_t num)
{
    for (size_t i = 0; i < num; i++)
    {
        if (((char*) ptr1)[i] != ((char*) ptr2)[i])
        {
            return false;
        }
    }
    return true;
}

void strcpy(char *dst, const char *src)
{
    int i = 0;
    for (i = 0; src[i] != 0; i++)
    {
        dst[i] = src[i];
    }
    dst[i]=0;
}