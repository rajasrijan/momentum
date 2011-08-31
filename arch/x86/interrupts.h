/* 
 * File:   interrupts.h
 * Author: Srijan
 *
 * Created on June 19, 2011, 8:01 PM
 */

#ifndef INTERRUPTS_H
#define	INTERRUPTS_H

#include <stdint.h>

typedef struct registers
{
    uint32_t ds; // Data segment selector
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
    uint32_t int_no, err_code; // Interrupt number and error code (if applicable)
    uint32_t eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
} registers_t;

typedef void (*isr_t)(registers_t*);
void register_interrupt_handler(uint8_t n, isr_t handler);
void isr_handler(registers_t regs);

#endif	/* INTERRUPTS_H */

