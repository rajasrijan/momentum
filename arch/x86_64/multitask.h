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

#ifndef MULTITASK_H
#define MULTITASK_H

#include "multitask.h"
#include "interrupts.h"
#include "paging.h"
#include "../kernel/lists.h"
#include <vector>
#include <threads.h>
#include <errno.h>

/*
  *	Thread flags.
  */
#define THREAD_BUSY (1 << 0)
#define THREAD_STOP (1 << 1)

#define KERNEL_THREAD_NAME "kernel_thread"

/*
   *	Thread info structure.
   */

class thread_info
{
public:
	mtx_t mtx;
	uint64_t flags;
	uint64_t isactive;
	uint64_t stackSize;
	retStack_t context;
	general_registers_t regs;
	//std::vector<pt_cache_unit_t> page_table;
private:
	uint64_t uiThreadId;
	uint64_t ProcessID;
	char p_sThreadName[256];
	void *(*pfnStartRoutine)(void *);

public:
	void operator=(const thread_info &t);
	thread_info(uint64_t processID, const char *threadName);
	~thread_info();
	uint64_t getProcessID();
	uint64_t getThreadID() const;

private:
	friend class multitask;
	static void thread_start_point(thread_info *thread);
	void *arg;
};

typedef thread_info *thread_t;

typedef struct core_info
{
	thread_info *threads;
} __attribute__((packed)) core_info_t;

void init_multitask(void);
void DestroyStack(uint32_t esp);
void init_kernel_stack(void);
void change_thread(const thread_info &thread, bool enable_interrupts = true);
void thread_end(void);
int CreateNullProcess(void);
thread_info *getNextThreadInQueue(void);

class multitask
{
public:
	~multitask();
	static multitask *getInstance();
	int initilize();
	int allocateStack(uint64_t &stackSize, uint64_t &stackPtr);
	const thread_info &getKernelThread();
	int createThread(thread_t *thd, const char *threadName, void *(*start_routine)(void *), void *arg);
	std::vector<thread_info> threadList;
	const thread_info &getNextThread(retStack_t *stack, general_registers_t *regs);
	int fork() { return ENOSYS; }

private:
	uint32_t uiThreadIterator;
	uint32_t uiCurrentThreadIndex;

private:
	mtx_t multitaskMutex;
	multitask();
};
#endif /* MULTITASK_H */
