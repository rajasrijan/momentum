/*
 * Copyright 2009-2021 Srijan Kumar Sharma
 *
 * This file is part of Momentum.
 *
 * Momentum is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Momentum is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Momentum.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>

#pragma pack(push, 1)
struct general_registers_t {
    char xmm[512];
    uint64_t R15, R14, R13, R12, R11, R10, R9, R8, rdi, rsi, rbp, rsp, rbx, rdx, rcx, rax; // Pushed by pusha.
};

struct retStack_t {
    uint64_t padding;                  // make struct multiple of 16
    uint64_t interruptNumber, err;     // Interrupt number and error code (if applicable)
    uint64_t rip, cs, rflags, rsp, ss; // Pushed by the processor automatically.
};
#pragma pack(pop)

typedef void (*isr_t)(retStack_t *, general_registers_t *);
void register_interrupt_handler(uint8_t n, isr_t handler);
/*This thing is called from asm code. So extern is needed*/
extern "C" {
/*called for every interrupt*/
void isr_handler(retStack_t *stack, general_registers_t *regs);
}
void eoi(void);
#endif /* INTERRUPTS_H */
