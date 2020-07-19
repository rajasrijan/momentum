/*
 * Copyright 2009-2020 Srijan Kumar Sharma
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
#include <arch/x86_64/global.h>
#include <arch/x86_64/mm.h>
#include <kernel/syscall.h>
#include <kernel/vfs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

uint64_t thread_id_counter = 0;
uint64_t process_id_counter = 0;
vector<MemPage> *multitask::kernel_memory_map = nullptr;

//	TODO: remove hardcoding
const retStack_t defaultThreadContext = {
    0,    // padding
    0,    // interruptNumber
    0,    // err;
    0,    // rip
    0x08, // cs
    0,    // rflags
    0,    // rsp
    0x10  // ss
};

[[maybe_unused]] static uint32_t getrandom(void)
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
 * WARNING: THIS FUNCTION CANT HAVE ANY CLASS VARIABLES.
 * SINCE THIS FUNCTION DOESNT RETURN, DESTRUCTOR WONT BE CALLED.
 * WHICH MEANS NO smart_ptr
 *
 *
 * Changes current thread to new thread with context reg.
 * also enables interrupts (clears NT bit in 'rflags' set by 'cli')
 */
__attribute__((noreturn)) void change_thread(const thread_t thread, bool enable_interrupts)
{
    retStack_t context = thread->context;
    uint64_t rsp = ((context.rsp) / 0x10) * 0x10;
    // set maskable interrupt flag.
    if (enable_interrupts)
    {
        context.rflags |= 0x200;
    }
    if (((context.cs) >> 3) == 0)
    {
        printf("\nInvalid Code section.Halting...");
        asm("cli;hlt;");
    }
    rsp -= sizeof(retStack_t);
    memcpy((char *)rsp, (char *)&(context), sizeof(retStack_t));
    rsp -= sizeof(general_registers_t);
    memcpy((char *)rsp, (char *)&(thread->regs), sizeof(general_registers_t));
    switch_context(rsp, thread->context.ss);
    while (1)
    {
        asm("cli;hlt;");
    }
}

void thread_end()
{
    while (1)
    {
        asm("sti;hlt;");
    }
}

multitask::multitask() : multitaskMutex(0), uiThreadIterator(0), uiCurrentThreadIndex(0), threadList(), processList()
{
    printf("NOT MORE THAN ONCE\n");
}

/*
allocates stack of stackSize size.
returns stack pointer and size.
*/
int multitask::allocateStack(uint64_t &stackSize, uint64_t &stackPtr, PageManager::Privilege privilege)
{
    int ret = 0;
    stackSize = PageManager::roundToPageSize(stackSize);
    //	Find free space after 4GB mark.
    ret = PageManager::findFreeVirtualMemory(stackPtr, stackSize, 0x100000000);
    if (ret < 0)
    {
        printf("failed to findFreeVirtualMemory\n");
        asm("cli;hlt");
    }

    if (PageManager::setPageAllocation(stackPtr, stackSize, privilege, PageManager::Read_Write))
    {
        printf("Failed to set page address\n");
        asm("cli;hlt");
    }
    stackPtr += stackSize;
    return ret;
}

thread_t multitask::getKernelThread()
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
    process_t kernel_process = new process_info(KERNEL_PROCESS_NAME);
    thread_t kernel_thread = new thread_info(KERNEL_THREAD_NAME, kernel_process);
    kernel_process->addThread(kernel_thread);

    printf("Creating kernel ProcessID [%d]\n", kernel_process->getProcessId());
    printf("Creating kernel ThreadID [%d]\n", kernel_thread->getThreadID());
    kernel_memory_map = &(kernel_process->memory_map);
    kernel_thread->context.ss = kernel_process->get_ss();
    kernel_thread->stackSize = KERNEL_STACK_SZ;
    errCode = allocateStack(kernel_thread->stackSize, kernel_thread->context.rsp, PageManager::Supervisor);
    kernel_thread->context.rsp -= 8;
    kernel_thread->regs.rbp = kernel_thread->context.rsp;
    if (errCode)
    {
        printf("Error allocating stack for kernel thread\n");
        return errCode;
    }
    kernel_thread->context.rip = (uint64_t)state_c0;
    kernel_thread->context.rflags = get_rflags();
    kernel_thread->context.cs = kernel_process->get_cs();

    processList.push_back(kernel_process);
    threadList.push_back(kernel_thread);
    uiCurrentThreadIndex = 0;
    setActiveProcess(kernel_process);
    extern void (*system_call)();
    //	set system call function pointer
    wrmsr(0xC0000082, (uint64_t)&system_call);
    // wrmsr(0xC0000084, 0x200);
    //	set cs
    wrmsr(0xC0000081, (0x08l << 32) | (0x10l << 48));
    return 0;
}

process_t multitask::getCurrentProcess()
{
    return threadList[uiCurrentThreadIndex]->parentProcess;
}

process_t multitask::getKernelProcess()
{
    return threadList[0]->parentProcess;
}

process_t multitask::getActiveProcess()
{
    sync lock(multitaskMutex);
    return active_process;
}

void multitask::setActiveProcess(process_t prs)
{
    //	freeze multitasking
    sync lock(multitaskMutex);
    active_process = prs;
}

int multitask::createProcess(process_t &prs, const char *processName, int ring, vector<MemPage> &mem_map, uint64_t entry)
{
    thread_t thd = nullptr;
    char threadName[256] = {};
    {
        //	freeze multitasking
        sync lock(multitaskMutex);
        processList.push_back(new process_info(processName));
        prs = processList.back();
        prs->path_history.push_back(rnode);
        // create main thread
        strcpy(threadName, processName);
        strcat(threadName, "-MainThread");
        prs->setMemoryMap(mem_map);
        prs->setEntry(entry);
        prs->setRing(ring);
    }
    createThread(prs, thd, threadName, (void *(*)(void *))entry, nullptr);
    setActiveProcess(prs);
    return 0;
}

void multitask::destroyProcess(int status)
{
    process_t prs = nullptr;
    {
        //	freeze multitasking
        sync lock(multitaskMutex);
        thread_t thd = threadList[uiCurrentThreadIndex];
        prs = thd->parentProcess;
        for (auto it = processList.begin(); it != processList.end(); it++)
        {
            if (*it == prs)
            {
                processList.erase(it);
                break;
            }
        }
        delete prs;
    }
    //  set kernel thread as active.
    setActiveProcess(getKernelProcess());
}

int multitask::createKernelThread(thread_t &thd, const char *threadName, void *(*start_routine)(void *), void *arg)
{
    process_t kernelProcess = processList[0];
    return createThread(kernelProcess, thd, threadName, start_routine, arg);
}

int multitask::createThread(process_t prs, thread_t &thd, const char *threadName, void *(*start_routine)(void *), void *arg)
{
    //	freeze multitasking
    sync lock(multitaskMutex);
    int ret = 0;
    threadList.push_back(new thread_info(threadName, prs));
    thd = threadList.back();
    prs->addThread(thd);
    thd->flags = THREAD_BUSY;
    thd->context = defaultThreadContext;
    thd->pfnStartRoutine = start_routine;
    thd->arg = arg;
    thd->context.cs = prs->get_cs();
    thd->context.ss = prs->get_ss();
    thd->context.rflags = get_rflags();
    thd->stackSize = KERNEL_STACK_SZ;
    if ((thd->context.cs & 3) == 0)
    {
        ret = allocateStack(thd->stackSize, thd->context.rsp, PageManager::Supervisor);
    }
    else
    {
        ret = allocateStack(thd->stackSize, thd->context.rsp, PageManager::User);
    }
    if (ret)
    {
        printf("Error allocating stack for thread\n");
        return ret;
    }
    thd->context.rsp -= 8;
    thd->regs.rbp = thd->context.rsp;
    switch (thd->context.cs & 3)
    {
    case 0:
        thd->context.rip = (uint64_t)&thread_info::thread_start_point;
        thd->regs.rdi = (uint64_t)thd;
        break;
    case 1:
    case 2:
    case 3:
        thd->context.rip = (uint64_t)thd->pfnStartRoutine;
        thd->regs.rdi = (uint64_t)thd->arg;
        break;
    default:
        printf("Invalid CPL");
        asm("cli;hlt");
    };
    printf("Creating P:%x,T:%x\n", (uint32_t)prs->getProcessId(), (uint32_t)thd->getThreadID());
    return 0;
}

thread_t multitask::getNextThread(retStack_t *stack, general_registers_t *regs)
{
    threadList[uiCurrentThreadIndex]->context = *stack;
    threadList[uiCurrentThreadIndex]->regs = *regs;

    if (mtx_trylock_notimeout(&multitaskMutex) == thrd_success)
    {
        mtx_unlock(&threadList[uiCurrentThreadIndex]->mtx);
        const size_t uiThreadCount = threadList.size();
        do
        {
            uiThreadIterator = (uiThreadIterator + 1) % uiThreadCount;
        } while ((mtx_trylock_notimeout(&threadList[uiThreadIterator]->mtx) != thrd_success) && uiCurrentThreadIndex != uiThreadIterator);
        //  if there is a process switch, need to change page directory.
        if (threadList[uiCurrentThreadIndex]->parentProcess != threadList[uiThreadIterator]->parentProcess)
        {
            //  remove old page directory
            PageManager::removeMemoryMap(threadList[uiCurrentThreadIndex]->parentProcess->memory_map);
            //  apply new page directory
            PageManager::applyMemoryMap(threadList[uiThreadIterator]->parentProcess->memory_map, PageManager::Privilege::User, PageManager::PageType::Read_Write);
        }
        if (threadList[uiCurrentThreadIndex]->flags == THREAD_STOP && uiCurrentThreadIndex != uiThreadIterator)
        {
            printf("Thread needs cleanup\n");
            delete threadList[uiCurrentThreadIndex];
            threadList.erase(threadList.begin() + uiCurrentThreadIndex);
        }

        uiCurrentThreadIndex = uiThreadIterator;
        mtx_unlock(&multitaskMutex);
    }
    return threadList[uiCurrentThreadIndex];
}

thread_info::thread_info(const char *threadName, process_t _parentProcess) : mtx(0), flags(0), stackSize(0), context({}), regs({}), parentProcess(nullptr), uiThreadId(0), pfnStartRoutine(), arg(nullptr)
{
    parentProcess = _parentProcess;
    uiThreadId = ++thread_id_counter;
    strcpy(p_sThreadName, threadName);
}

thread_info &thread_info::operator=(const thread_info &t)
{
    printf("stack %x,%x\n", stackSize, t.stackSize);
    stackSize = t.stackSize;
    printf("stack %x,%x\n", stackSize, t.stackSize);
    return *this;
}

thread_info::~thread_info()
{
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

void thread_info::release()
{
    flags = THREAD_STOP;
}

process_info::process_info(const char *processName) : memory_map(), m_uiProcessId(0), uiEntry(0), threads(), ring(0)
{
    m_uiProcessId = ++process_id_counter;
    strcpy(p_szProcessName, processName);
}

process_info::~process_info()
{
    for (size_t i = 0; i < threads.size(); i++)
    {
        threads[i]->release();
    }
    threads.clear();
    //  remove mapping
    PageManager::removeMemoryMap(memory_map);
    //  reclaim physical memory
    for (const auto &mmap : memory_map)
    {
        for (size_t i = 0; i < mmap.size; i += PageManager::PAGESIZE)
        {
            rel_2mb_block(mmap.paddr + i);
        }
    }
}

uint64_t process_info::get_cs()
{
    if (ring == 0)
    {
        return 0x08;
    }
    else if (ring == 3)
    {
        return 0x23;
    }
    else
    {
        printf("Invalid RING");
        asm("cli;hlt");
        return 0x0;
    }
}

uint64_t process_info::get_ss()
{
    if (ring == 0)
    {
        return 0x10;
    }
    else if (ring == 3)
    {
        return 0x1B;
    }
    else
    {
        printf("Invalid RING");
        asm("cli;hlt");
        return 0x0;
    }
}

extern "C" uint64_t syscall(int64_t callid, int64_t arg0, int64_t arg1)
{
    switch (callid)
    {
    case SYSCALL_EXIT:
    {
        multitask::getInstance()->destroyProcess(arg0);
        //  there is nothing left to return to. Wait for reschedule and cleanup.
        while (true)
        {
            asm("pause;hlt");
        }
    }
    case SYSCALL_PUTCHAR:
    {
        return putchar(arg0);
    }
    case SYSCALL_GETCHAR:
    {
        return getchar();
    }
    case SYSCALL_GETCWD:
    {
        std::string curDir = getCurrentPath();
        strncpy((char *)arg0, curDir.c_str(), min((unsigned long)arg1, curDir.size() + 1));
        return arg0;
    }
    case SYSCALL_CHDIR:
    {
        return chdir((const char *)arg0);
    }
    case SYSCALL_CLOSE:
    {
        return close(arg0);
    }
    case SYSCALL_OPENAT:
    {
        openat_args *args = (openat_args *)arg0;
        return openat(args->dirfd, args->pathname, args->flags, 0);
    }
    case SYSCALL_GETDENTS:
    {
        getdents_args *args = (getdents_args *)arg0;
        vector<string> dir;
        int ret = getdents(args->fd, dir);
        auto dent_sz = dir.size();
        for (size_t i = 0; i < min(dent_sz, args->count); i++)
        {
            strncpy(args->dirp[i].d_name, dir[i].c_str(), NAME_MAX);
        }
        ret = min(dent_sz, args->count);
        return ret;
    }
    default:
    {
        printf("Unknown syscall: %d\n", callid);
        asm("cli;hlt");
    }
    }
    return 0;
}
