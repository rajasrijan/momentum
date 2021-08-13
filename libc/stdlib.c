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

#include <stdlib.h>
#include <stdint.h>
#include "string.h"
#include "stdio.h"
#include <ctype.h>
#include <kernel/syscall.h>
#include <sys/mman.h>
#include <errno.h>
#include <kernel/heap.h>

heap_t *pHeap = NULL;

static uint8_t checksum(uint8_t *ptr, uint32_t len)
{
    uint8_t res = 0;
    for (uint32_t i = 0; i < len; i++)
        res = (uint8_t)(res + ptr[i]);
    return res;
}

static uint8_t getsum(uint8_t *ptr, uint32_t len)
{
    uint8_t res = 0;
    for (uint32_t i = 0; i < len; i++)
        res = (uint8_t)(res + ptr[i]);
    return (uint8_t)(-res);
}

#if __STDC_HOSTED__ == 1
int create_heap()
{
    pHeap = (heap_t *)mmap(NULL, HEAP_DEFAULT_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE, -1, 0);
    if (!pHeap)
        return -ENOMEM;
    pHeap->flags = HEAP_EMPTY;
    pHeap->next = NULL;
    pHeap->size = HEAP_DEFAULT_SIZE;
    pHeap->checksum = 0;
    pHeap->checksum = getsum((uint8_t *)pHeap, sizeof(heap_t));
    return 0;
}
#endif

void check_heap_integrity()
{
    heap_t *heap_ptr = pHeap;
    do
    {
        if (checksum((uint8_t *)heap_ptr, sizeof(heap_t)) != 0)
        {
            printf("heap corruption\n");
            __asm__("cli;hlt;");
        }
        heap_ptr = heap_ptr->next;
    } while (heap_ptr);
}

/*
 * Aligned allocate memory in kernel heap.B
 */
void *_aligned_malloc(uint32_t len, int n)
{
    check_heap_integrity();
    heap_t *heap_ptr = pHeap;
    volatile void *ptr = 0;
    uint32_t alignment = 1 << n;
    while (heap_ptr)
    {
        if (checksum((uint8_t *)heap_ptr, sizeof(heap_t)) != 0)
        {
            printf("\nheap corruption");
            __asm__("cli;hlt;");
            return (void *)ptr;
        }

        //	allocations are in reverse order
        uint32_t length = (uint32_t)(((uint64_t)heap_ptr + heap_ptr->size) - (((uint64_t)heap_ptr + heap_ptr->size - len) & (~(alignment - 1))));
        if ((heap_ptr->flags == HEAP_EMPTY) && (heap_ptr->size >= length + (2 * sizeof(heap_t))))
        {
            uint32_t size = length + sizeof(heap_t);
            heap_ptr->size -= size;
            heap_t *next_node = heap_ptr->next;
            heap_ptr->next = (heap_t *)((uint64_t)heap_ptr + heap_ptr->size);
            heap_ptr->checksum = 0;
            heap_ptr->checksum = getsum((uint8_t *)heap_ptr, sizeof(heap_t));
            heap_ptr = heap_ptr->next;
            heap_ptr->size = size;
            heap_ptr->flags = HEAP_FULL;
            heap_ptr->next = next_node;
            heap_ptr->checksum = 0;
            heap_ptr->checksum = getsum((uint8_t *)heap_ptr, sizeof(heap_t));
            ptr = (void *)((char *)heap_ptr + sizeof(heap_t));
            check_heap_integrity();
            return (void *)ptr;
        }
        else
        {
            heap_ptr = heap_ptr->next;
        }
    }
    check_heap_integrity();
    return (void *)ptr;
}

void *_malloc(uint32_t length)
{
    return _aligned_malloc(length, 4);
}

void _free(void *ptr)
{
    check_heap_integrity();
    if (!ptr)
        return;
    heap_t *heap_ptr = (heap_t *)((uint64_t)ptr - sizeof(heap_t));
    if (heap_ptr->flags == HEAP_EMPTY)
    {
        printf("Double free\n");
        __asm__("cli;hlt;");
    }
    memset(ptr, 0xcc, heap_ptr->size - sizeof(heap_t));

    if (checksum((uint8_t *)heap_ptr, sizeof(heap_t)) != 0)
    {
        printf("heap corruption\n");
        __asm__("cli;hlt;");
    }

    heap_ptr->flags = HEAP_EMPTY;
    heap_ptr->checksum = 0;
    heap_ptr->checksum = getsum((uint8_t *)heap_ptr, sizeof(heap_t));

    check_heap_integrity();
}

#if __STDC_HOSTED__ == 1
__attribute__((noreturn)) void _exit(int status)
{
    __asm__ volatile("syscall" ::"D"(SYSCALL_EXIT), "S"(status), "d"(0) : "rcx", "r11");
    while (1)
    {
        __asm__("pause");
    }
}
__attribute__((noreturn)) void exit(int status)
{
    _exit(status);
}
#endif
size_t mem_used = 0;

int atoi(const char *str)
{
    int number = 0;
    int index = 0;
    // skip till first digit
    for (index = 0; str[index] != 0 && !isdigit(str[index]); index++)
        ;
    for (; str[index] != 0 && isdigit(str[index]); index++)
    {
        number = ((number * 10) + (str[index] - '0'));
    }

    return number;
}

void *aligned_malloc(size_t size, int n)
{
    volatile void *ptr = _aligned_malloc(size, n);
    if (!ptr)
    {
        printf("\nmm error.");
        __asm__("cli;hlt");
        return NULL;
    }
    mem_used += size;
    memset((void *)ptr, 0xcc, size);
    return (void *)ptr;
}

void *malloc(size_t size)
{
    volatile void *ptr = _malloc(size);
    if (!ptr)
    {
        printf("\nmm error.");
        __asm__("cli;hlt");
        return NULL;
    }
    mem_used += size;
    memset((void *)ptr, 0xcc, size);
    return (void *)ptr;
}

void free(void *ptr)
{
    _free(ptr);
}

void *realloc(void *ptr, size_t size)
{
    _free(ptr);
    return _malloc(size);
}

void *calloc(size_t blocks, size_t size)
{
    void *ptr = _malloc(size * blocks);
    if (!ptr)
    {
        printf("\nmm error.");
        __asm__("cli;hlt");
        return NULL;
    }
    memset(ptr, 0, size * blocks);
    mem_used += (size * blocks);
    return ptr;
}

const size_t atexit_fn_max_count = 10;
size_t atexit_fn_count = 0;
void (*atexit_fn_list[atexit_fn_max_count])(void);

int atexit(void (*function)(void))
{
    if (atexit_fn_count >= atexit_fn_max_count)
        return -ENOMEM;
    atexit_fn_list[atexit_fn_count++] = function;
    return 0;
}
