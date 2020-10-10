#include "new.h"
#include <cstddef>
#include <stdlib.h>

void *operator new(size_t size)
{
    return malloc(size);
}

void *operator new[](size_t size)
{
    return malloc(size);
}

void *operator new([[maybe_unused]] size_t size, void *t)
{
    return t;
}

void operator delete(void *p)
{
    free(p);
}

void operator delete[](void *p)
{
    free(p);
}