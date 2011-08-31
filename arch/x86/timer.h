/* 
 * File:   timer.h
 * Author: Srijan
 *
 * Created on June 23, 2011, 12:09 PM
 */

#ifndef TIMER_H
#define	TIMER_H
#include "global.h"
#include "interrupts.h"

void apic_timer_callback(registers_t* reg);
void init_timer(uint32_t frequency);

#endif	/* TIMER_H */

