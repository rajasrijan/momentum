/* 
 * File:   string.h
 * Author: Srijan
 *
 * Created on July 4, 2011, 2:34 PM
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

