/*
 * Copyright 2009-2017 Srijan Kumar Sharma
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

#include "multitask.h"
#include "descriptor_tables.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "mm.h"
#include "paging.h"
#include "global.h"
#include "../kernel/lists.h"
#include "timer.h"

uint64_t thread_id_counter = 0;

//	TODO: remove hardcoding
const retStack_t defaultThreadContext = {
	0,	//interruptNumber
	0,	// err;
	0,	//rip
	0x08, // cs
	0,	// rflags
	0,	// rsp
	0x10  // ss
};

static uint32_t getrandom(void)
{
	static uint32_t local_random_generator = 0;
	local_random_generator++;
	if (local_random_generator == 0)
		local_random_generator++;
	return local_random_generator;
}

void init_multitask()
{
	multitask::getInstance()->initilize();
}

/*
 * Changes current thread to new thread with context reg.
 * also enables interrupts (clears NT bit in 'rflags' set by 'cli')
 */
void change_thread(const thread_info &thread, bool enable_interrupts)
{
	retStack_t context = thread.context;
	uint64_t rsp = context.rsp;
	//set maskable interrupt flag.
	if (enable_interrupts)
	{
		context.rflags |= 0x200;
	}
	if (((context.cs) >> 3) == 0)
	{
		printf("\nInvalid Code section.Halting...");
		__asm__("cli;"
				"hlt;");
	}
	rsp -= sizeof(retStack_t);
	memcpy((char *)rsp, (char *)&(context), sizeof(retStack_t));
	rsp -= sizeof(general_registers_t);
	memcpy((char *)rsp, (char *)&(thread.regs), sizeof(general_registers_t));
	//printf("(%lx,%lx)>", thread.context.rip, thread.context.rsp);
	switch_context(rsp);
}

void thread_end()
{
	while (1)
	{
		__asm__(
			"sti;"
			"hlt;");
	}
}

thread_info *getNextThreadInQueue()
{
	/*linked_list_t *cursor = sys_info.thread_list;
	while (get_async_spin_lock(&(((thread_info*) cursor->pointer)->flags)))
	{
		cursor = cursor->next;
		if (sys_info.thread_list == cursor)
			return null_thread;
	}
	return ((thread_info*) cursor->pointer);*/
	return 0;
}

int CreateNullProcess()
{
	return 0;
}

multitask::multitask() : multitaskMutex(0), uiThreadIterator(0)
{
	printf("NOT MORE THAN ONCE\n");
}

/*
allocates stack of stackSize size.
returns stack pointer and size.
*/
int multitask::allocateStack(uint64_t &stackSize, uint64_t &stackPtr)
{
	stackSize = PageManager::roundToPageSize(stackSize);
	//	Find free space after 4GB mark.
	PageManager::getInstance()->findFreeVirtualMemory(stackPtr, stackSize, 0x100000000);
	PageManager::getInstance()->setPageAllocation(stackPtr, stackSize);
	stackPtr += stackSize;
	return 0;
}

const thread_info &multitask::getKernelThread()
{
	return threadList[0];
}

multitask::~multitask()
{
}

multitask *multitask::getInstance()
{
	static multitask instance;
	return &instance;
}

int multitask::initilize()
{
	int errCode = 0;
	printf("Creating kernel thread\n");
	thread_info kernel_thread(0, KERNEL_THREAD_NAME);
	printf("Creating kernel ProcessID [%d]\n", kernel_thread.ProcessID);
	printf("Creating kernel ThreadID [%d]\n", kernel_thread.getThreadID());
	kernel_thread.context.ss = 0x10;
	kernel_thread.stackSize = KERNEL_STACK_SZ;
	errCode = allocateStack(kernel_thread.stackSize, kernel_thread.context.rsp);
	kernel_thread.regs.rbp = kernel_thread.context.rsp;
	if (errCode)
	{
		printf("Error allocating stack for kernel thread\n");
		return errCode;
	}
	kernel_thread.context.rip = (uint64_t)state_c0;
	kernel_thread.context.rflags = get_rflags();
	kernel_thread.context.cs = 0x08;

	threadList.push_back(kernel_thread);
	uiCurrentThreadIndex = 0;
	return 0;
}

int multitask::createThread(thread_t *thd, const char *threadName, void *(*start_routine)(void *), void *arg)
{
	//	freeze multitasking
	sync lock(multitaskMutex);
	int ret = 0;
	threadList.push_back(thread_info(0, threadName));
	*thd = &threadList.back();
	(*thd)->flags = THREAD_BUSY;
	(*thd)->context = defaultThreadContext;
	(*thd)->pfnStartRoutine = start_routine;
	(*thd)->arg = arg;
	(*thd)->stackSize = KERNEL_STACK_SZ;
	ret = allocateStack((*thd)->stackSize, (*thd)->context.rsp);
	if (ret)
	{
		printf("Error allocating stack for thread\n");
		return ret;
	}
	(*thd)->context.rflags = get_rflags();
	(*thd)->context.rip = (uint64_t)&thread_info::thread_start_point;
	(*thd)->regs.rdi = (uint64_t)(*thd);
	printf("Creating P:%x,T:%x\n", (uint32_t)(*thd)->ProcessID, (uint32_t)(*thd)->getThreadID());
	return 0;
}

const thread_info &multitask::getNextThread(retStack_t *stack, general_registers_t *regs)
{
	threadList[uiCurrentThreadIndex].context = *stack;
	threadList[uiCurrentThreadIndex].regs = *regs;
	//sync lock(multitaskMutex);
	if (mtx_trylock(&multitaskMutex) == thrd_success)
	{
		mtx_unlock(&threadList[uiCurrentThreadIndex].mtx);
		const size_t uiThreadCount = threadList.size();
		do
		{
			uiThreadIterator = (uiThreadIterator + 1) % uiThreadCount;
		} while ((mtx_trylock(&threadList[uiThreadIterator].mtx) != thrd_success) && uiCurrentThreadIndex != uiThreadIterator);

		uiCurrentThreadIndex = uiThreadIterator;
		mtx_unlock(&multitaskMutex);
	}
	return threadList[uiCurrentThreadIndex];
}

thread_info::thread_info(uint64_t processID, const char *threadName) : mtx(0),
																	   flags(0),
																	   isactive(0),
																	   stackSize(0),
																	   context({}),
																	   regs({}),
																	   uiThreadId(0),
																	   ProcessID(processID)
{
	uiThreadId = thread_id_counter++;
	strcpy(p_sThreadName, threadName);
}

void thread_info::operator=(const thread_info &t)
{
	printf("stack %x,%x\n", stackSize, t.stackSize);
	stackSize = t.stackSize;
	printf("stack %x,%x\n", stackSize, t.stackSize);
	//memcpy(this, &t, sizeof(t));
}

thread_info::~thread_info()
{
}

uint64_t thread_info::getProcessID()
{
	return ProcessID;
}

uint64_t thread_info::getThreadID() const
{
	return uiThreadId;
}

void thread_info::thread_start_point(thread_info *thread)
{
	void *ret = thread->pfnStartRoutine(thread->arg);
	printf("Thread returned %x\n", (uint64_t)ret);
	while (true)
	{
		__asm__("pause");
	}
}