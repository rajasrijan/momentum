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

#ifndef MULTITASK_H
#define MULTITASK_H

#include "multitask.h"
#include <arch/x86_64/interrupts.h>
#include <arch/x86_64/paging.h>
#include <errno.h>
#include <memory>
#include <ring_buffer>
#include <threads.h>
#include <utility>
#include <vector>
#include <string>

/*
 *	Thread flags.
 */
#define THREAD_BUSY (1 << 0)
#define THREAD_STOP (1 << 1)
/*
 *	Process flags.
 */
#define PROCESS_BUSY (1 << 0)
#define PROCESS_STOP (1 << 1)

#define KERNEL_PROCESS_NAME "kernel_process"
#define KERNEL_THREAD_NAME "kernel_thread"

typedef class thread_info *thread_t;
typedef class process_info *process_t;

/*
 * Process info struct
 */

class process_info
{
    friend class multitask;

  public:
    uint64_t flags;
    std::vector<MemPage> memory_map;
    ring_buffer<char, 256> key_buffer;
    std::vector<std::shared_ptr<class vnode>> path_history;

  private:
    uint64_t m_uiProcessId;
    char p_szProcessName[256];
    std::string command_line;
    uint64_t uiEntry;
    std::vector<thread_t> threads;
    int ring;
    std::vector<class vfile *> open_file_descriptor;
    mtx_t open_file_descriptor_mtx;

  public:
    process_info(const char *processName, const std::string &cmd_line);
    ~process_info();
    uint64_t getProcessId()
    {
        return m_uiProcessId;
    }
    void addThread(thread_t thd)
    {
        threads.push_back(thd);
    }
    void setMemoryMap(std::vector<MemPage> &p)
    {
        memory_map = p;
    }
    void setEntry(uint64_t entry)
    {
        uiEntry = entry;
    }
    void setRing(int _ring)
    {
        ring = _ring;
    }
    uint64_t get_cs();
    uint64_t get_ss();
    const char *getName()
    {
        return p_szProcessName;
    }
    void release();
    int removeThread(thread_t thread);
    bool isStopped()
    {
        return (flags == PROCESS_STOP) && threads.empty();
    }
    const std::string &getCommandLine()
    {
        return command_line;
    }
    int insert_opened_file(vfile *file_obj)
    {
        class sync lock(open_file_descriptor_mtx);
        open_file_descriptor.push_back(file_obj);
        return open_file_descriptor.size() - 1;
    }
    int get_opened_file(int fd, vfile **file_obj)
    {
        class sync lock(open_file_descriptor_mtx);

        if (fd < 0 || (size_t)fd >= open_file_descriptor.size())
            return -EBADF;
        *file_obj = open_file_descriptor[fd];
        return 0;
    }
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

typedef struct core_info {
    thread_info *threads;
} __attribute__((packed)) core_info_t;

void init_multitask(void);
void DestroyStack(uint32_t esp);
void init_kernel_stack(void);
[[noreturn]] void change_thread(const thread_t thread, bool enable_interrupts = true);
void thread_end(void);
int CreateNullProcess(void);
thread_info *getNextThreadInQueue(void);

class multitask
{
  public:
    ~multitask();
    multitask(const multitask &) = delete;
    multitask &operator=(const multitask &) = delete;
    static multitask *getInstance();
    int initilize();
    int allocateStack(uint64_t &stackSize, uint64_t &stackPtr, PageManager::Privilege privilege);
    thread_t getKernelThread();
    // const thread_t getKernelProcess();
    int createProcess(process_t &prs, const char *processName, const std::string &cmd_line, int ring, std::vector<MemPage> &mem_map, uint64_t entry);
    int createKernelThread(thread_t &thd, const char *threadName, void *(*start_routine)(void *), void *arg);
    int createThread(process_t prs, thread_t &thd, const char *threadName, void *(*start_routine)(void *), void *arg);
    thread_t getNextThread(retStack_t *stack, general_registers_t *regs);
    void setActiveProcess(process_t prs);
    process_t getActiveProcess();
    process_t getCurrentProcess();
    process_t getKernelProcess();
    int fork()
    {
        return -ENOSYS;
    }
    void destroyProcess(int status);
    const std::vector<process_t> &ListProcesses()
    {
        return processList;
    }
    int allocate_virtual_memory(uint64_t &vaddr, size_t len, PageManager::PageType pageType);

  private:
    mtx_t multitaskMutex;
    uint32_t uiThreadIterator;
    uint32_t uiCurrentThreadIndex;
    std::vector<thread_t> threadList;
    std::vector<process_t> processList;
    process_t active_process;
    static std::vector<MemPage> *kernel_memory_map;

  private:
    multitask();
};
#endif /* MULTITASK_H */
