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
			__refCount[0] = 1;
		}

		shared_ptr(shared_ptr& src)
		{
			lock = src.lock;
			{
				sync auto_lock(*lock);
				__refCount = src.__refCount;
				__data = src.__data;
				__refCount[0]++;
			}
		}

		void operator=(T* data_ptr)
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

		operator shared_ptr<char>&()
		{
			return *(shared_ptr<char>*)this;
		}
	};
}

#endif /* MEMORY_H */

