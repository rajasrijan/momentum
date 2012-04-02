/*
 * Copyright 2009-2012 Srijan Kumar Sharma
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
#define	MULTITASK_H

#ifdef	__cplusplus
extern "C"
{
#endif

#include "multitask.h"
#include "interrupts.h"
#include "paging.h"
#include "../kernel/lists.h"

typedef struct thread_info
{
    uint32_t ProcessID;
    uint32_t ThreadID;
    uint32_t isactive;
    registers_t context;
    vector_list_t *page_table;
} thread_info_t;

typedef struct core_info
{
    thread_info_t *threads;
} __attribute__((packed)) core_info_t;

typedef struct thread
{
    uint32_t id;
} thread_t;

void init_multitask(void);
void* CreateStack(void);
void DestroyStack(uint32_t esp);
int CreateThread(uint32_t *thread, const uint32_t attr, void *((*start_routine)(void*)), void *arg);
void init_kernel_stack(void);
void change_thread(thread_info_t* thread);
void thread_end(void);
#ifdef	__cplusplus
}
#endif

#endif	/* MULTITASK_H */

