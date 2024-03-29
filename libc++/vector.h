/*
 * Copyright 2009-2021 Srijan Kumar Sharma
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
#ifndef VECTOR_H
#define VECTOR_H

#include "algorithm.h"
#include "native_sync.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

namespace std
{
template <class T>
class vector
{
    T *__data;
    uint32_t _size;
    uint32_t _count;
    mtx_t lock;
    const size_t element_size = sizeof(T);

  public:
    vector() : __data(nullptr), _size(0), _count(0), lock(0)
    {
        _size = 8;
        _count = 0;
        mtx_init(&lock, 0);
        __data = (T *)calloc(_size, sizeof(T));
    }

    vector(size_t n) : __data(nullptr), _size(0), _count(0), lock(0)
    {
        _size = n;
        _count = n;
        mtx_init(&lock, 0);
        __data = (T *)calloc(_size, sizeof(T));
    }

    vector(const vector &v) : __data(nullptr), _size(0), _count(0), lock(0)
    {
        _size = v._size;
        _count = 0;
        mtx_init(&lock, 0);
        __data = (T *)calloc(_size, sizeof(T));
        for (const auto &var : v)
        {
            push_back(var);
        }
    }

    ~vector()
    {
        if (__data != nullptr)
        {
            for (size_t index = 0; index < _count; index++)
            {
                __data[index].~T();
            }
            free(__data);
            __data = nullptr;
        }
        _size = 0;
        _count = 0;
    }

    void resize(size_t count)
    {
        for (; _count < count;)
        {
            emplace_back();
        }
        for (; _count > count;)
        {
            pop_back();
        }
    }

    bool empty() const
    {
        return size() == 0;
    }

    size_t size() const
    {
        return _count;
    }

    void push_back(const T &val)
    {
        class sync local_lock(lock);
        if (_count >= _size)
        {
            _size += 8;
            T *temp = (T *)calloc(_size, sizeof(T));
            copy(__data, &__data[_count], temp);
            if (__data != 0)
                free(__data);
            __data = temp;
        }
        void *tmp = (void *)&__data[_count];
        new (tmp) T(val);
        _count++;
    }

    template <class... Args>
    void emplace_back(Args &&...args)
    {
        class sync local_lock(lock);
        if (_count >= _size)
        {
            _size += 8;
            T *temp = (T *)calloc(_size, sizeof(T));
            copy(__data, &__data[_count], temp);
            if (__data != 0)
                free(__data);
            __data = temp;
        }
        void *tmp = (void *)&__data[_count];
        new (tmp) T(args...);
        _count++;
    }

    void pop_back()
    {
        class sync local_lock(lock);
        _count--;
        __data[_count].~T();
    }

    T &operator[](uint32_t index)
    {
        if (index >= _count)
        {
            printf("Index %d is greater than count %d\n", index, _count);
            printf("%s:%d\n", __FILE__, __LINE__);
            asm("cli;hlt;");
        }
        void *tmp = (void *)((char *)__data + (element_size * index));
        return *((T *)tmp);
    }

    const T &operator[](uint32_t index) const
    {
        void *tmp = (void *)((char *)__data + (element_size * index));
        return *((const T *)tmp);
    }

    vector &operator=(const vector &other)
    {
        class sync local_lock(lock);
        if (__data != nullptr)
        {
            free(__data);
            __data = nullptr;
        }
        _size = other._size;
        _count = other._count;
        __data = (T *)calloc(_size, sizeof(T));
        for (size_t i = 0; i < _count; i++)
            new ((void *)&__data[i]) T(other[i]);
        return *this;
    }

    T &back()
    {
        return __data[_count - 1];
    }
    T *data()
    {
        return __data;
    }
    const T *data() const
    {
        return __data;
    }
    void clear()
    {
        if (__data != nullptr)
        {
            for (size_t index = 0; index < _count; index++)
            {
                __data[index].~T();
            }
            free(__data);
            __data = nullptr;
        }
        _size = 0;
        _count = 0;
    }
    class iterator
    {
      public:
        iterator() : m_pdataStart(nullptr), m_count(0), m_index(0)
        {
        }

        ~iterator()
        {
        }
        iterator(const iterator &it) = default;

        iterator &operator=(const iterator &it) = default;

        size_t operator-(const iterator &it)
        {
            return m_index - it.m_index;
        }

        bool operator!=(const iterator &it)
        {
            return m_index != it.m_index;
        }

        bool operator==(const iterator &it) const
        {
            return m_index == it.m_index;
        }

        bool operator<=(const iterator &it) const
        {
            return m_index <= it.m_index;
        }

        bool operator<(const iterator &it) const
        {
            return m_index < it.m_index;
        }

        iterator &operator++(/*int r*/)
        {
            m_index++;
            return *this;
        }

        iterator operator++([[maybe_unused]] int r)
        {
            m_index++;
            return *this;
        }
        T &operator*()
        {
            return (*m_pdataStart)[m_index];
        }
        T *operator->()
        {
            return &(*m_pdataStart)[m_index];
        }
        iterator operator+(ssize_t n)
        {
            return iterator(m_pdataStart, m_count, (m_index + n > m_count) ? m_count : m_index + n);
        }
        iterator operator-(ssize_t n)
        {
            return iterator(m_pdataStart, m_count, m_index - n);
        }

      protected:
        friend vector;
        iterator(vector<T> *dataPtr, ssize_t sz, ssize_t index) : m_pdataStart(dataPtr), m_count(sz), m_index(index)
        {
        }

      private:
        vector<T> *m_pdataStart;
        ssize_t m_count;
        ssize_t m_index;
    };
    class const_iterator
    {
      public:
        const_iterator(const const_iterator &) = default;
        const_iterator() : m_pdataStart(nullptr), m_count(0), m_index(0)
        {
        }

        ~const_iterator()
        {
        }
        const_iterator &operator=(const const_iterator &) = default;

        bool operator!=(const const_iterator &it)
        {
            return m_index != it.m_index;
        }
        const_iterator &operator++(/*int r*/)
        {
            m_index++;
            return *this;
        }
        const T &operator*()
        {
            return (*m_pdataStart)[m_index];
        }
        const T *operator->()
        {
            return &(*m_pdataStart)[m_index];
        }
        bool operator==(const const_iterator &it) const
        {
            return m_index == it.m_index;
        }

      protected:
        friend vector;
        const_iterator(const vector<T> *dataPtr, uint32_t sz, uint32_t index) : m_pdataStart(dataPtr), m_count(sz), m_index(index)
        {
        }

      private:
        const vector<T> *m_pdataStart;
        uint32_t m_count;
        uint32_t m_index;
    };
    iterator begin()
    {
        return iterator(this, _count, 0);
    }
    const_iterator begin() const
    {
        return const_iterator(this, _count, 0);
    }
    iterator end()
    {
        return iterator(this, _count, _count);
    }
    const_iterator end() const
    {
        return const_iterator(this, _count, _count);
    }
    iterator insert(iterator position, const T &val)
    {
        class sync local_lock(lock);
        _count++;
        if (_count >= _size)
        {
            _size += 8;
            T *temp = (T *)calloc(_size, sizeof(T));
            copy(__data, &__data[_count], temp);
            if (__data != 0)
                free(__data);
            __data = temp;
        }
        for (ssize_t i = _count - 1; i > position.m_index; i--)
            __data[i] = __data[i - 1];

        new ((void *)&(*position)) T(val);
        return iterator(this, _count, position.m_index);
    }

    template <class TargetIterator>
    void insert(iterator position, TargetIterator first, TargetIterator last)
    {
        class sync local_lock(lock);
        size_t element_count = last - first;
        size_t new_count = _count + element_count;

        if (new_count >= _size)
        {
            new_count = std::max(new_count, (size_t)_size * 2);
            auto temp = (T *)calloc(new_count, sizeof(T));
            copy(begin(), position, temp);
            copy(position, end(), temp + (position - begin()) + element_count);
            __data = temp;
            _size = new_count;
        }
        else
        {
            copy(position, end(), position + element_count);
            _size = new_count;
        }
        _count += element_count;
        copy(first, last, position);
    }

    iterator erase(iterator position)
    {
        class sync local_lock(lock);
        position->~T();
        copy(position + 1, end(), position);
        _count--;
        return iterator(this, _count, position.m_index);
    }
};
} // namespace std

#endif /* VECTOR_H */
