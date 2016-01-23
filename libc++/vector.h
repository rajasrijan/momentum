#ifndef VECTOR_H
#define VECTOR_H

#include <stdint.h>
#include "native_sync.h"
#include "algorithm.h"

namespace std
{

    template<class T>
    class vector
    {
        T *__data;
        uint32_t _size;
        uint32_t _count;
        mtx_t lock;
    public:

        vector()
        {
            _size = 8;
            _count = 0;
            mtx_init(&lock, 0);
            __data = (T*) calloc(_size, sizeof (T));
        }

        ~vector()
        {
            if (__data != 0)
                free(__data);
        }

        size_t size()
        {
            return _count;
        }

        void push_back(T &val)
        {
            sync local_lock(&lock);
            if (_count >= _size) {
                _size += 8;
                T *temp = (T*) calloc(_size, sizeof (T));
                copy(__data, &__data[_count], temp);
                if (__data != 0)
                    free(__data);
                __data = temp;
            }
            __data[_count++] = val;
        }

        T& operator[](uint32_t index)
        {
            return __data[index];
        }

        T& back()
        {
            return __data[_count - 1];
        }
    };
}

#endif /* VECTOR_H */

