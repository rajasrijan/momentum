/*
 * Copyright 2009-2018 Srijan Kumar Sharma
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

#include "global.h"
#include "interrupts.h"
#include "apic.h"

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
		printf("RSP [0x%x]\n", stack);
		printf("Interrupt No [0x%x], RSP [0x%x], RIP [0x%x]\n", stack->interruptNumber, regs->rsp, stack->rip);
		if (stack->interruptNumber == 14)
		{
			printf("PF addr [0x%x], Error code [0x%x]\n", get_cr2(), stack->err);
		}
		else if (stack->interruptNumber == 13)
		{
			printf("GP Error code [0x%x]\n", stack->err);
		}
		//print call stack.
		printf("Call stack:\n");
		uint64_t rsp = regs->rsp, rbp = regs->rbp, rip = stack->rip;
		for (size_t i = 0; i < 3; i++)
		{
			printf("RIP [0x%lx], RSP [0x%lx], RBP [0x%lx]\n", rip, rsp, rbp);
			rsp = rbp;
			rbp = ((uint64_t *)rsp)[0];
			rip = ((uint64_t *)rsp)[1];
			if (rip == 0xDEADBEEFDEADBEEF || rbp == 0xDEADBEEFDEADBEEF)
			{
				break;
			}
		}
		__asm__("cli");
		__asm__("hlt");
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