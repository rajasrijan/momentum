#ifndef _STDDEF_H_
#define _STDDEF_H_

#define size_t unsigned long
#define ssize_t long

#ifndef __cplusplus
#define NULL ((void *)0)
#else
#define NULL 0
#endif //__cplusplus

#define offsetof(S, d) ((size_t)(&(((S *)0)->d)))

#endif //_STDDEF_H_