/* 
 * File:   stdio.h
 * Author: Srijan
 *
 * Created on June 22, 2011, 9:28 PM
 */

#ifndef STDIO_H
#define	STDIO_H
#include <stdint.h>
extern void putcharacter(const char ch, uint32_t x, uint32_t y);
extern void scroll(void);
void printf(const char *format, ...);
void clrscr(void);
#endif	/* STDIO_H */

