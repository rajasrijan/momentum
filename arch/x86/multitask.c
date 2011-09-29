#include "multitask.h"
#include "descriptor_tables.h"
#include "stdlib.h"
#include "stdio.h"
#include "mm.h"
#include "paging.h"
#include "global.h"

task_info_t *p_task;

static uint16_t stack_map = 0;

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
    p_task->next_task = p_task;
    /*
     * main thread stack.
     */
    stack_map |= 1;
    core_info_t *cinfo = calloc(1, sizeof (core_info_t));
    cinfo->threads = p_task->threads;
    cinfo->task = p_task;
    /* Thread information block */
    set_gdt_gate(&gt[0].ge[5], (uint32_t) (cinfo), (sizeof (core_info_t) + 4095) / 4096, 0xF2, 0xCF);
    __asm__("mov $0x28,%%ax;"
            "mov %%ax, %%fs;"
            :
            :
            : "eax");
    printf(".....");
    __asm__("xchg %%bx,%%bx;"
            "int $32" :);
}

int CreateThread(thread_t *thread, const uint32_t attr, void *((*start_routine)(void*)), void *arg)
{
    thread_info_t *new_thread = calloc(1, sizeof (thread_info_t));
    thread_info_t *old_thread = 0;
    __asm__("mov %%fs:(0x0),%0;" : "=r"(old_thread));
    new_thread->thread_id = getrandom();
    /*
     *  We should copy critical sections,like cs,ds,eflags etc...
     *  And zero out general registers.
     */
    printf("\n%x", old_thread);
    new_thread->context = old_thread->context;
    new_thread->context.eax = 0;
    new_thread->context.ebx = 0;
    new_thread->context.ecx = 0;
    new_thread->context.edx = 0;
    new_thread->context.esi = 0;
    new_thread->context.edi = 0;
    new_thread->context.ebp = 0;
    /*
     * Create proper stack.Push arg and return value.
     */
    uint32_t* esp = (uint32_t*) CreateStack();
    esp[0x100000 - 1] = (uint32_t) arg;
    esp[0x100000 - 2] = 0;
    new_thread->context.esp = (uint32_t) (&(esp[0x100000 - 2 - 3 - 2]));
    new_thread->context.eip = (uint32_t) start_routine;
    old_thread->next_thread = new_thread;
    thread->id = new_thread->thread_id;
    return 0;
}

void* CreateStack()
{
    uint32_t* esp = 0;
    for (uint32_t i = 1; i < 16; i++)
    {
        if ((stack_map & (1 << i)) == 0)
        {
            stack_map = (uint16_t) (stack_map | (1 << i));
            break;
        }
        else
        {
            esp += 0x100000;
        }
    }
    esp = (uint32_t*) ((uint32_t) esp + (uint32_t) KERNEL_STACK_PTR);
    map_4mb((uint32_t) esp);
    return esp;
}
