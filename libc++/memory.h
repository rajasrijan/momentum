/*
 * Copyright 2009-2020 Srijan Kumar Sharma
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
#ifndef MEMORY_H
#define MEMORY_H

#include "native_sync.h"

namespace std
{
template <class T>
class shared_ptr
{
    template <class U>
    friend class shared_ptr;
    T *__data;
    unsigned int *__refCount;
    mtx_t *lock;

  public:
    shared_ptr() : __data(nullptr), __refCount(nullptr), lock(nullptr)
    {
    }

    shared_ptr(T *data_ptr) : __data(nullptr), __refCount(nullptr), lock(nullptr)
    {
        __data = data_ptr;
        lock = new mtx_t;
        mtx_init(lock, 0);
        __refCount = new unsigned int;
        __refCount[0] = 1;
    }

    shared_ptr(const shared_ptr &src) : __data(nullptr), __refCount(nullptr), lock(nullptr)
    {
        lock = src.lock;
        if (lock)
        {
            sync auto_lock(*lock);
            __refCount = src.__refCount;
            __data = src.__data;
            __refCount[0]++;
        }
    }

    template <class Y>
    shared_ptr(shared_ptr<Y> &&src) : __data(nullptr), __refCount(nullptr), lock(nullptr)
    {
        lock = src.lock;
        if (lock)
        {
            sync auto_lock(*lock);
            __refCount = src.__refCount;
            __data = src.__data;
            __refCount[0]++;
        }
    }

    template <class Y>
    shared_ptr &operator=(shared_ptr<Y> &&r)
    {
        //	Relese old
        {
            if (__refCount != 0)
            {
                sync auto_lock(*lock);
                if (__refCount != 0)
                {
                    __refCount[0]--;

                    if (__refCount[0] == 0)
                    {
                        if (__data != 0)
                        {
                            delete __data;
                            __data = 0;
                        }
                        if (lock != 0)
                        {
                            auto_lock.invalidateLock();
                            delete lock;
                            lock = 0;
                        }

                        if (__refCount != 0)
                        {
                            delete __refCount;
                            __refCount = 0;
                        }
                    }
                }
            }
        }
        lock = r.lock;
        if (lock)
        {
            sync auto_lock(*lock);
            __data = r.__data;
            __refCount = r.__refCount;
            __refCount[0]++;
        }
        return *this;
    }

    shared_ptr &operator=(const shared_ptr &r)
    {
        //	Relese old
        {
            if (__refCount != 0)
            {
                sync auto_lock(*lock);
                if (__refCount != 0)
                {
                    __refCount[0]--;

                    if (__refCount[0] == 0)
                    {
                        if (__data != 0)
                        {
                            delete __data;
                            __data = 0;
                        }
                        if (lock != 0)
                        {
                            auto_lock.invalidateLock();
                            delete lock;
                            lock = 0;
                        }

                        if (__refCount != 0)
                        {
                            delete __refCount;
                            __refCount = 0;
                        }
                    }
                }
            }
        }
        lock = r.lock;
        if (lock)
        {
            sync auto_lock(*lock);
            __data = r.__data;
            __refCount = r.__refCount;
            __refCount[0]++;
        }
        return *this;
    }

    shared_ptr &operator=(T *data_ptr)
    {
        //	Relese old
        {
            if (__refCount != 0)
            {
                sync auto_lock(*lock);
                if (__refCount != 0)
                {
                    __refCount[0]--;

                    if (__refCount[0] == 0)
                    {
                        if (__data != 0)
                        {
                            delete __data;
                            __data = 0;
                        }
                        if (lock != 0)
                        {
                            auto_lock.invalidateLock();
                            delete lock;
                            lock = 0;
                        }

                        if (__refCount != 0)
                        {
                            delete __refCount;
                            __refCount = 0;
                        }
                    }
                }
            }
        }

        //	Create new
        __data = data_ptr;
        lock = new mtx_t;
        mtx_init(lock, 0);
        __refCount = new unsigned int;
        __refCount[0]++;
        return *this;
    }

    ~shared_ptr()
    {
        if (__refCount != 0)
        {
            sync auto_lock(*lock);
            if (__refCount != 0)
            {
                __refCount[0]--;

                if (__refCount[0] == 0)
                {
                    if (__data != 0)
                    {
                        delete __data;
                        __data = 0;
                    }
                    if (lock != 0)
                    {
                        auto_lock.invalidateLock();
                        delete lock;
                        lock = 0;
                    }

                    if (__refCount != 0)
                    {
                        delete __refCount;
                        __refCount = 0;
                    }
                }
            }
        }
    }

    T *get()
    {
        return __data;
    }

    const T *get() const
    {
        return __data;
    }

    T *operator->()
    {
        return __data;
    }

    const T *operator->() const
    {
        return __data;
    }

    operator shared_ptr<char> &()
    {
        return *(shared_ptr<char> *)this;
    }

    bool operator==(const T *ptr) const
    {
        return __data == ptr;
    }

    bool operator==(const shared_ptr &ptr) const
    {
        return __data == ptr.__data;
    }

    bool operator!=(const T *ptr)
    {
        return __data != ptr;
    }
    // operator T *()
    // {
    // 	return __data;
    // }
};

template <class T, class... Args>
shared_ptr<T> make_shared(Args &&... args)
{
    return shared_ptr<T>(new T(args...));
}
} // namespace std

#endif /* MEMORY_H */
