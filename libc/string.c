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
    char* ret = (char*) malloc(strlen(s1));
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