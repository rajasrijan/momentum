/*
 * Copyright 2009-2019 Srijan Kumar Sharma
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
#include <arch/x86_64/interrupts.h>
#include <arch/x86_64/paging.h>
#include <vector>
#include <threads.h>
#include <errno.h>
#include <memory>
#include <utility>
#include <ring_buffer>

/*
  *	Thread flags.
  */
#define THREAD_BUSY (1 << 0)
#define THREAD_STOP (1 << 1)

#define KERNEL_PROCESS_NAME "kernel_process"
#define KERNEL_THREAD_NAME "kernel_thread"

typedef class thread_info *thread_t;
typedef class process_info *process_t;

/*
 * Process info struct
 */

class process_info
{
public:
    ring_buffer<char, 256> key_buffer;
    std::vector<std::shared_ptr<class vnode>> path_history;

private:
    uint64_t m_uiProcessId;
    char p_szProcessName[256];
    uint64_t uiEntry;
    std::vector<thread_t> threads;
    std::vector<std::pair<uint64_t, uint64_t>> program;
    int ring;

public:
    process_info(const char *processName);
    ~process_info();
    uint64_t getProcessId() { return m_uiProcessId; }
    void addThread(thread_t thd) { threads.push_back(thd); }
    void setProgram(std::vector<std::pair<uint64_t, uint64_t>> p) { program = p; }
    void setEntry(uint64_t entry) { uiEntry = entry; }
    void setRing(int _ring) { ring = _ring; }
    uint64_t get_cs();
    uint64_t get_ss();
    const char *getName() { return p_szProcessName; }
};

/*
*	Thread info structure.
*/
class thread_info
{
public:
    mtx_t mtx;
    uint64_t flags;
    uint64_t stackSize;
    retStack_t context;
    general_registers_t regs;

private:
    process_t parentProcess;
    uint64_t uiThreadId;
    char p_sThreadName[256];
    void *(*pfnStartRoutine)(void *);

public:
    thread_info &operator=(const thread_info &t);
    thread_info(const thread_info &t) = delete;
    thread_info(const char *threadName, process_t parentProcess);
    ~thread_info();
    uint64_t getThreadID() const;
    void release();

private:
    friend class multitask;
    static void thread_start_point(thread_info *thread);
    void *arg;
};

typedef struct core_info
{
    thread_info *threads;
} __attribute__((packed)) core_info_t;

void init_multitask(void);
void DestroyStack(uint32_t esp);
void init_kernel_stack(void);
void change_thread(const thread_t thread, bool enable_interrupts = true);
void thread_end(void);
int CreateNullProcess(void);
thread_info *getNextThreadInQueue(void);

class multitask
{
public:
    ~multitask();
    static multitask *getInstance();
    int initilize();
    int allocateStack(uint64_t &stackSize, uint64_t &stackPtr, PageManager::Privilege privilege);
    const thread_t getKernelThread();
    //const thread_t getKernelProcess();
    int createProcess(process_t &prs, const char *processName, int ring, std::vector<std::pair<uint64_t, uint64_t>> program, uint64_t entry);
    int createKernelThread(thread_t &thd, const char *threadName, void *(*start_routine)(void *), void *arg);
    int createThread(process_t prs, thread_t &thd, const char *threadName, void *(*start_routine)(void *), void *arg);
    const thread_t getNextThread(retStack_t *stack, general_registers_t *regs);
    void setActiveProcess(process_t prs);
    process_t getActiveProcess();
    process_t getCurrentProcess();
    process_t getKernelProcess();
    int fork() { return ENOSYS; }
    void destroyProcess(int status);
    const std::vector<process_t> &ListProcesses() { return processList; }

private:
    mtx_t multitaskMutex;
    uint32_t uiThreadIterator;
    uint32_t uiCurrentThreadIndex;
    std::vector<thread_t> threadList;
    std::vector<process_t> processList;
    thread_t current_thread;
    process_t active_process;

private:
    multitask();
};
#endif /* MULTITASK_H */
