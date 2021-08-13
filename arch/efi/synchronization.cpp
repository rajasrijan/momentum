#include <stdint.h>
#include <arch/synchronization.h>

uint64_t get_spin_lock(void *lock_ptr)
{
    int ret = 0;
    do
    {
        asm(".intel_syntax noprefix\n"
            "mov rax,1\n"
            "xchg rax,[rbx]\n"
            //".att_syntax"
            : "=a"(ret)
            : "b"(lock_ptr));
    } while (ret);
    return ret;
}
uint64_t get_async_spin_lock(void *lock_ptr)
{
    asm("cli;hlt");
    return 1;
}
uint64_t release_spin_lock(void *lock_ptr)
{
    ((uint64_t *)lock_ptr)[0] = 0;
    return 0;
}