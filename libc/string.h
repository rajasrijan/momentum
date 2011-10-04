/*
 * Copyright 2009-2011 Srijan Kumar Sharma
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

#ifndef STRING_H
#define	STRING_H

#ifdef	__cplusplus
extern "C"
{
#endif

#include <stddef.h>

void * memset(void * ptr, int value, size_t num);
char *strdup(const char *s1);
size_t strlen(const char* str);

static inline void* memcpy(char *dst, const char *src, size_t len)
{

    for (size_t i = 0; i < len; i++)
    {
        dst[i] = src[i];
    }
}
int memcmp(const void * ptr1, const void * ptr2, size_t num);
char strcmp(char* s1, char *s2);

#ifdef	__cplusplus
}
#endif

#endif	/* STRING_H */

