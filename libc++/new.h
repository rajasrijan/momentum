#include "stddef.h"
#include "stdlib.h"

inline void *operator new(size_t size)
{
    return malloc(size);
}

inline void *operator new[](size_t size)
{
    return malloc(size);
}

inline void operator delete(void *p)
{
    free(p);
}

inline void operator delete[](void *p)
{
    free(p);
}