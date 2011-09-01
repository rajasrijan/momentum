#include "multitask.h"
#include "descriptor_tables.h"
#include "stdlib.h"
#include "stdio.h"
#include "mm.h"
#include "paging.h"
#include "global.h"

static void* CreateStack(void* arg);

task_info_t *p_task;

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
    printf("\nCreating kernel process..");
    p_task = calloc(1, sizeof (task_info_t));
    p_task->task_id = getrandom();
    p_task->threads = calloc(1, sizeof (thread_info_t));
    p_task->threads->thread_id = getrandom();
    core_info_t *cinfo = calloc(1, sizeof (core_info_t));
    cinfo->threads = p_task->threads;
    cinfo->task = p_task;
    printf("\n core info at :%x", cinfo);
    /* Thread information block */
    set_gdt_gate(&gt[0].ge[5], (uint32_t) (cinfo), sizeof (core_info_t), 0xF2, 0xCF);
    __asm__("xchg %%bx,%%bx;" "mov $0x28,%%ax;" "mov %%ax, %%fs;" : : : "eax");
    printf(".....");
}

int CreateThread(thread_t *thread, const uint32_t attr, void *((*start_routine)(void*)), void *arg)
{
    thread_info_t *new_thread = calloc(1, sizeof (thread_info_t));
    thread_info_t *old_thread = 0;
    __asm__("mov %%fs:(0x0),%0;" : "=r"(old_thread));
    new_thread->thread_id = getrandom();
    new_thread->context.esp = (uint32_t) CreateStack(arg);
    new_thread->context.eip = (uint32_t) start_routine;
    old_thread->next_thread = new_thread;
    thread->id = new_thread->thread_id;
    return 0;
}

static void* CreateStack(void* arg)
{
    map_4mb((uint32_t) KERNEL_STACK_PTR);
    /*
     * Create proper call stack.
     */
    ((uint32_t*) KERNEL_STACK_PTR)[0x100000 - 1] = (uint32_t) arg; //Push argument.
    ((uint32_t*) KERNEL_STACK_PTR)[0x100000 - 2] = (uint32_t) 0; //Push return address.
    return (void*) (((uint32_t*) KERNEL_STACK_PTR)+(0x100000 - 2));
}
