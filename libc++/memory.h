#ifndef MEMORY_H
#define MEMORY_H

#include "native_sync.h"

namespace std
{

template<class T>
class shared_ptr
{
    T *__data;
    unsigned int *__refCount;
    mtx_t *lock;
public:

    shared_ptr() : __data(0), __refCount(0)
    {

    }

    shared_ptr(T* data_ptr)
    {
        __data = data_ptr;
        lock = new mtx_t;
        mtx_init(lock, 0);
        __refCount = new unsigned int;
        *__refCount++;
    }

    ~shared_ptr()
    {
        sync local_lock(lock);
        __refCount--;

        if (__refCount == 0)
        {
            if (__data != 0)
                delete __data;
            if (lock != 0)
                delete lock;
            if (__refCount != 0)
                delete __refCount;
        }
    }

    T *get()
    {
        return __data;
    }

    operator shared_ptr<char>()
    {
        return *(shared_ptr<char>*)this;
    }
};
}

#endif /* MEMORY_H */

