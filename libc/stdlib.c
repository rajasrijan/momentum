#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"

extern void* _malloc(uint32_t length);
extern void _free(void *ptr);
uint32_t mem_used = 0;

void* malloc(size_t size)
{
    volatile void* ptr = _malloc(size);
    if (ptr == 0)
    {
        printf("\nmm error.");
        __asm__("cli;hlt");
    }
    mem_used += size;
    return (void*)ptr;
}

void free(void* ptr)
{
    _free(ptr);
}

void* realloc(void* ptr, size_t size)
{
    _free(ptr);
    return _malloc(size);
}

void* calloc(size_t blocks, size_t size)
{
    void* ptr = _malloc(size * blocks);
    if (ptr == 0)
    {
        printf("\nmm error.%x,%x",mem_used,(size * blocks));
        __asm__("cli;hlt");
    }
    memset(ptr, 0, size * blocks);
    mem_used += (size * blocks);
    return ptr;
}