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
#include <kernel/logging.h>

isr_t interrupt_handlers[256] = {0};

/* This gets called from our ASM interrupt handler stub.*/

void isr_handler(retStack_t *stack, general_registers_t *regs)
{
    if (stack->interruptNumber < 256 && interrupt_handlers[stack->interruptNumber] != 0)
    {
        isr_t handler = interrupt_handlers[stack->interruptNumber];
        handler(stack, regs);
        if (stack->interruptNumber >= 32)
            eoi();
    }
    else
    {

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
        //     if (!rip[history_index] || rip[history_index] == 0xDEADBEEFDEADBEEF || rbp[history_index] ==
        //     0xDEADBEEFDEADBEEF) {
        //         break;
        //     }
        // }
        log_error("RSP [%#llx]\n", stack);
        log_error("Intr.No [%#llx], RSP [%#llx], RIP [%#llx]\n", stack->interruptNumber, regs->rsp, stack->rip);
        if (stack->interruptNumber == 6)
        {
            log_error("Invalid Opcode\n");
        }
        else if (stack->interruptNumber == 14)
        {
            log_error("PF addr [0x%02x:%#llx], Error code [0x%llx]\n", stack->cs, get_cr2(), stack->err);
        }
        else if (stack->interruptNumber == 13)
        {
            log_error("GP Error code [0x%llx]\n", stack->err);
        }
        //  print context
        log_error("RAX%#llx,RBX%#llx,RCX%#llx,RDX%#llx\n", regs->rax, regs->rbx, regs->rcx, regs->rdx);
        log_error("RDI%#llx,RSI%#llx,RBP%#llx,RSP%#llx\n", regs->rdi, regs->rsi, regs->rbp, regs->rsp);
        log_error("R08%#llx,R09%#llx,R10%#llx,R11%#llx\n", regs->R8, regs->R9, regs->R10, regs->R11);
        log_error("R12%#llx,R13%#llx,R14%#llx,R15%#llx\n", regs->R12, regs->R13, regs->R14, regs->R15);
        log_error("RIP%#llx,RFLAGS%#llx,CS%08X,SS%08X\n", stack->rip, stack->rflags, stack->cs, stack->ss);
        //  print call stack.
        //  log_error("Call stack:\n");
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
