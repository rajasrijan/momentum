/*
 * Copyright 2009-2011 Srijan Kumar Sharma
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

typedef struct thread_info
{
    uint32_t thread_id;
    registers_t context;
    struct thread_info *next_thread;
} thread_info_t;

typedef struct task_info
{
    uint32_t task_id;
    thread_info_t *threads;
    struct task_info *next_task;
} task_info_t;

typedef struct core_info
{
    thread_info_t *threads;
    task_info_t *task;
} __attribute__((packed)) core_info_t;

typedef struct thread
{
    uint32_t id;
} thread_t;

void init_multitask(void);
void* CreateStack(void);
int CreateThread(thread_t *thread, const uint32_t attr, void *((*start_routine)(void*)), void *arg);
void init_kernel_stack(void);

#ifdef	__cplusplus
}
#endif

#endif	/* MULTITASK_H */

