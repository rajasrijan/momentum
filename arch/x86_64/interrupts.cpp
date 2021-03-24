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

#include "interrupts.h"
#include "apic.h"
#include "global.h"
#include <kernel/sys_info.h>

isr_t interrupt_handlers[256] = {0};

/* This gets called from our ASM interrupt handler stub.*/

void isr_handler(retStack_t *stack, general_registers_t *regs)
{
    if (stack->interruptNumber < 256 && interrupt_handlers[stack->interruptNumber] != 0) {
        isr_t handler = interrupt_handlers[stack->interruptNumber];
        handler(stack, regs);
        if (stack->interruptNumber >= 32)
            eoi();
    } else {

        // uint64_t rsp[10], rbp[10], rip[10];
        // size_t history_index = 0;
        // rsp[history_index] = regs->rsp;
        // rbp[history_index] = regs->rbp;
        // rip[history_index] = stack->rip;
        // for (size_t i = 0; i < 5; i++) {
        //     history_index++;
        //     rsp[history_index] = rbp[history_index - 1];
        //     rbp[history_index] = ((uint64_t *)rsp[history_index])[0];
        //     rip[history_index] = ((uint64_t *)rsp[history_index])[1];
        //     if (!rip[history_index] || rip[history_index] == 0xDEADBEEFDEADBEEF || rbp[history_index] == 0xDEADBEEFDEADBEEF) {
        //         break;
        //     }
        // }
        printf("RSP [%#llx]\n", stack);
        printf("Intr.No [%#llx], RSP [%#llx], RIP [%#llx]\n", stack->interruptNumber, regs->rsp, stack->rip);
        if (stack->interruptNumber == 6) {
            printf("Invalid Opcode\n");
        } else if (stack->interruptNumber == 14) {
            printf("PF addr [0x%02x:%#llx], Error code [0x%llx]\n", stack->cs, get_cr2(), stack->err);
        } else if (stack->interruptNumber == 13) {
            printf("GP Error code [0x%llx]\n", stack->err);
        }
        //  print context
        printf("RAX%016llX,RBX%016llX,RCX%016llX,RDX%016llX", regs->rax, regs->rbx, regs->rcx, regs->rdx);
        printf("RDI%016llX,RSI%016llX,RBP%016llX,RSP%016llX", regs->rdi, regs->rsi, regs->rbp, regs->rsp);
        printf("R08%016llX,R09%016llX,R10%016llX,R11%016llX", regs->R8, regs->R9, regs->R10, regs->R11);
        printf("R12%016llX,R13%016llX,R14%016llX,R15%016llX", regs->R12, regs->R13, regs->R14, regs->R15);
        printf("RIP%016llX,RFLAGS%016llX,CS%08X,SS%08X", stack->rip, stack->rflags, stack->cs, stack->ss);
        //  print call stack.
        //  printf("Call stack:\n");
        __asm__("cli;hlt");
    }
}

void register_interrupt_handler(uint8_t n, isr_t handler)
{
    interrupt_handlers[n] = handler;
}

void eoi()
{
    sys_info.local_apic->eoi = 0;
}
