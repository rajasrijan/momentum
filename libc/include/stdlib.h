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

#ifndef STDLIB_H
#define STDLIB_H

#include "stddef.h"
#include <sys/types.h>

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0
#ifdef __cplusplus
extern "C"
{
#endif
    long long int strtoll(const char *str, char **endptr, int base);
    long int strtol(const char *str, char **endptr, int base);
    int atoi(const char *str);
    void *aligned_malloc(size_t size, int n);
    void *malloc(size_t size);
    void free(void *ptr);
    void *realloc(void *ptr, size_t size);
    void *calloc(size_t blocks, size_t size);
    void exit(int status);
    __attribute__((noreturn)) void abort(void);
    int atexit(void (*function)(void));

#ifndef __cplusplus
#define min(a, b) ((a < b) ? a : b)
#define max(a, b) ((a > b) ? a : b)
#endif

#ifdef __cplusplus
}
#endif

#endif /* STDLIB_H */
