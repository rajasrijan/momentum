/* 
 * File:   multitask.h
 * Author: Srijan
 *
 * Created on July 1, 2011, 8:57 PM
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
int CreateThread(thread_t *thread, const uint32_t attr, void *((*start_routine)(void*)), void *arg);

#ifdef	__cplusplus
}
#endif

#endif	/* MULTITASK_H */

