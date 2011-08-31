/* 
 * File:   cpu.h
 * Author: Srijan
 *
 * Created on July 9, 2011, 8:55 AM
 */

#ifndef CPU_H
#define	CPU_H

#ifdef	__cplusplus
extern "C"
{
#endif

#define cpuid(in, a, b, c, d) __asm__ volatile ("cpuid": "=a" (a), "=b" (b), "=c" (c), "=d" (d) : "a" (in));

#ifdef	__cplusplus
}
#endif

#endif	/* CPU_H */

