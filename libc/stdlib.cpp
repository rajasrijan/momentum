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

#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"

extern void *_malloc(uint32_t length);
extern void _free(void *ptr);
extern void *_aligned_malloc(uint32_t len, int n);
size_t mem_used = 0;

extern "C" void *aligned_malloc(size_t size, int n)
{
    volatile void *ptr = _aligned_malloc(size, n);
    if (!ptr)
    {
        printf("\nmm error.");
        __asm__("cli;hlt");
        return nullptr;
    }
    mem_used += size;
    memset((void *)ptr, 0xcc, size);
    return (void *)ptr;
}

extern "C" void *malloc(size_t size)
{
    volatile void *ptr = _malloc(size);
    if (!ptr)
    {
        printf("\nmm error.");
        __asm__("cli;hlt");
        return nullptr;
    }
    mem_used += size;
    memset((void *)ptr, 0xcc, size);
    return (void *)ptr;
}

extern "C" void free(void *ptr)
{
    _free(ptr);
}

extern "C" void *realloc(void *ptr, size_t size)
{
    _free(ptr);
    return _malloc(size);
}

extern "C" void *calloc(size_t blocks, size_t size)
{
    void *ptr = _malloc(size * blocks);
    if (!ptr)
    {
        printf("\nmm error.");
        __asm__("cli;hlt");
        return nullptr;
    }
    memset(ptr, 0, size * blocks);
    mem_used += (size * blocks);
    return ptr;
}