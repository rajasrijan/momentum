/* 
 * File:   stdlib.h
 * Author: Srijan
 *
 * Created on July 4, 2011, 6:41 PM
 */

#ifndef STDLIB_H
#define	STDLIB_H

#ifdef	__cplusplus
extern "C"
{
#endif
#include "stddef.h"

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

void* malloc(size_t size);
void free(void* ptr);
void* realloc(void* ptr, size_t size);
void* calloc(size_t blocks, size_t size);


#ifdef	__cplusplus
}
#endif

#endif	/* STDLIB_H */

