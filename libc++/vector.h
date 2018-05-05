/*
 * Copyright 2009-2017 Srijan Kumar Sharma
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

#include <stdint.h>
#include "native_sync.h"
#include "algorithm.h"

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
	vector()
	{
		_size = 8;
		_count = 0;
		mtx_init(&lock, 0);
		__data = (T *)calloc(_size, sizeof(T));
	}

	vector(int n)
	{
		_size = n + 8;
		_count = n;
		mtx_init(&lock, 0);
		__data = (T *)calloc(_size, sizeof(T));
	}

	~vector()
	{
		_size = 0;
		_count = 0;
		if (__data != nullptr)
		{
			for (size_t index = 0; index < _count; index++)
			{
				__data[index].~T();
			}
			free(__data);
			__data = nullptr;
		}
	}

	size_t size() const
	{
		return _count;
	}

	void push_back(const T &val)
	{
		sync local_lock(lock);
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

	T &operator[](uint32_t index)
	{
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
		sync local_lock(lock);
		if (__data != nullptr)
		{
			free(__data);
			__data = nullptr;
		}
		_size = other._size;
		_count = other._count;
		__data = (T *)calloc(_size, sizeof(T));
		for (int i = 0; i < _count; i++)
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
	class iterator
	{
	  public:
		iterator() : m_pdataStart(nullptr), m_count(0), m_index(0)
		{
		}

		~iterator()
		{
		}
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
		iterator operator++(/*int r*/)
		{
			m_index++;
			return *this;
		}
		iterator operator++(int r)
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
		iterator operator+(uint32_t n)
		{
			return iterator(m_pdataStart, m_count, (m_index + n > m_count) ? m_count : m_index + n);
		}

	  protected:
		friend vector;
		iterator(vector<T> *dataPtr, uint32_t sz, uint32_t index) : m_pdataStart(dataPtr), m_count(sz), m_index(index)
		{
		}

	  private:
		vector<T> *m_pdataStart;
		uint32_t m_count;
		uint32_t m_index;
	};
	class const_iterator
	{
	  public:
		const_iterator() : m_pdataStart(nullptr), m_count(0), m_index(0)
		{
		}

		~const_iterator()
		{
		}

		bool operator!=(const const_iterator &it)
		{
			return m_index != it.m_index;
		}
		void operator++(/*int r*/)
		{
			m_index++;
		}
		T &operator*()
		{
			return m_pdataStart[m_index];
		}
		T *operator->()
		{
			return &m_pdataStart[m_index];
		}

	  protected:
		friend vector;
		const_iterator(T *dataPtr, uint32_t sz, uint32_t index) : m_pdataStart(dataPtr), m_count(sz), m_index(index)
		{
		}

	  private:
		T *m_pdataStart;
		uint32_t m_count;
		uint32_t m_index;
	};
	iterator begin()
	{
		return iterator(this, _count, 0);
	}
	const_iterator begin() const
	{
		return const_iterator(__data, _count, 0);
	}
	iterator end()
	{
		return iterator(this, _count, _count);
	}
	const_iterator end() const
	{
		return const_iterator(__data, _count, _count);
	}
	void insert(iterator position, const T &val)
	{
		sync local_lock(lock);
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
		copy(position, position + (_count - 1), position + 1);
		new ((void *)&(*position)) T(val);
	}
	iterator erase(iterator position)
	{
		sync local_lock(lock);
		position->~T();
		copy(position + 1, end(), position);
		_count--;
		return position;
	}
};
}

#endif /* VECTOR_H */
