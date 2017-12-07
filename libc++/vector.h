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
			__data = (T*)calloc(_size, sizeof(T));
		}

		vector(int n)
		{
			_size = n + 8;
			_count = n;
			mtx_init(&lock, 0);
			__data = (T*)calloc(_size, sizeof(T));
		}

		~vector()
		{
			if (__data != 0)
				free(__data);
		}

		size_t size() const
		{
			return _count;
		}

		void push_back(const T &val)
		{
			sync local_lock(lock);
			if (_count >= _size) {
				_size += 8;
				T *temp = (T*)calloc(_size, sizeof(T));
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
		T* data()
		{
			return __data;
		}
		const T* data() const
		{
			return __data;
		}
		class iterator
		{
		public:
			iterator() :m_pdataStart(nullptr), m_count(0), m_index(0)
			{
			}

			~iterator()
			{
			}

			bool operator!=(const iterator &it)
			{
				return m_index != it.m_index;
			}
			bool operator==(const iterator &it)
			{
				return m_index == it.m_index;
			}
			void operator++(/*int r*/)
			{
				m_index++;
			}
			T& operator*()
			{
				return m_pdataStart[m_index];
			}
			T* operator->()
			{
				return &m_pdataStart[m_index];
			}
		protected:
			friend vector;
			iterator(T* dataPtr, uint32_t sz, uint32_t index) :m_pdataStart(dataPtr), m_count(sz), m_index(index)
			{
			}
		private:
			T *m_pdataStart;
			uint32_t m_count;
			uint32_t m_index;
		};
		class const_iterator
		{
		public:
			const_iterator() :m_pdataStart(nullptr), m_count(0), m_index(0)
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
			T& operator*()
			{
				return m_pdataStart[m_index];
			}
			T* operator->()
			{
				return &m_pdataStart[m_index];
			}
		protected:
			friend vector;
			const_iterator(T* dataPtr, uint32_t sz, uint32_t index) :m_pdataStart(dataPtr), m_count(sz), m_index(index)
			{
			}
		private:
			T *m_pdataStart;
			uint32_t m_count;
			uint32_t m_index;
		};
		iterator begin()
		{
			return iterator(__data, _count, 0);
		}
		const_iterator begin() const
		{
			return const_iterator(__data, _count, 0);
		}
		iterator end()
		{
			return iterator(__data, _count, _count);
		}
		const_iterator end() const
		{
			return const_iterator(__data, _count, _count);
		}
	};
}

#endif /* VECTOR_H */

