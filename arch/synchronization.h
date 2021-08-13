#ifndef SYNCHRONIZATION_H_COMMON
#define SYNCHRONIZATION_H_COMMON

extern "C"
{
    uint64_t get_spin_lock(void *lock_ptr);
    uint64_t get_async_spin_lock(void *lock_ptr);
    uint64_t release_spin_lock(void *lock_ptr);
}

#endif // SYNCHRONIZATION_H_COMMON