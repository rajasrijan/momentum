#include "threads.h"
#include "../arch/x86/global.h"

int mtx_init(mtx_t* mutex, int type)
{
    *mutex = 0;
    return 0;
}

int mtx_lock(mtx_t* mutex)
{
    if (!get_spin_lock(mutex))
        return thrd_success;
    return thrd_error;
}

int mtx_trylock(mtx_t *mutex)
{
    if (!get_async_spin_lock(mutex))
        return thrd_success;
    else
        return thrd_busy;

    return thrd_busy;
}

int mtx_unlock(mtx_t *mutex)
{
    release_spin_lock(mutex);
    return thrd_success;
}

void mtx_destroy(mtx_t *mutex)
{
}
