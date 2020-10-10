/*
 * Copyright 2009-2020 Srijan Kumar Sharma
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
#define STRING_H

#include <stddef.h>
#ifdef __cplusplus
extern "C"
{
#endif //  __cplusplus
    void *memset(void *ptr, int value, size_t num);
    void *memcpy(void *dst, const void *src, size_t len);
    char *strcat(char *dst, const char *src);
    size_t strlen(const char *str);
    int memcmp(const void *ptr1, const void *ptr2, size_t num);
    char *strdup(const char *s1);
    char *strcpy(char *dst, const char *src);

    int strcmp(const char *s1, const char *s2);
    char stricmp(const char *s1, const char *s2);
    /**
     * @brief find last character in the string s1
     * 
     * @param s1 string in which to search for
     * @param character he character to search for
     * @return const char* character position or NULL on no match
     */
    const char *strrchar(const char *s1, int character);
    const char *strchar(const char *s1, int character);
    char *strstr(const char *str1, const char *str2);
    char *strncpy(char *dst, const char *src, size_t count);
    int strncmp(const char *str1, const char *str2, size_t num);
    char *strncat(char *destination, const char *source, size_t num);
    char *strtok(char *str, const char *delimiters);
#ifdef __cplusplus
}
#endif //  __cplusplus
#endif /* STRING_H */
