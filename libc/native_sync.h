/*
 * Copyright 2009-2019 Srijan Kumar Sharma
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

/*
  * File:   native_sync.h
  * Author: Srijan
  *
  * Created on 23 October, 2015, 12:15 PM
  */

#ifndef NATIVE_SYNC_H
#define NATIVE_SYNC_H

#include <stdint.h>
#ifdef __cplusplus
extern "C"
{
#endif //	__cplusplus
	enum
	{
		thrd_success,
		thrd_nomem,
		thrd_timedout,
		thrd_busy,
		thrd_error
	};

	typedef uint64_t mtx_t;
	int mtx_init(mtx_t *mutex, int type);
	int mtx_lock(mtx_t *mutex);
	int mtx_trylock_notimeout(mtx_t *mutex);
	int mtx_trylock(mtx_t *mutex, uint64_t timeout);
	int mtx_unlock(mtx_t *mutex);
	void mtx_destroy(mtx_t *mutex);

	typedef struct _sem_t
	{
		int64_t count, max_count;
		mtx_t lock;
	} sem_t;
	int sem_init(sem_t *semaphore, int64_t max_count, int64_t count);
#ifdef __cplusplus
}
#endif //	__cplusplus

#ifdef __cplusplus
/*Syncrosization helper class. Acquires while constructed, releases while destroyed.*/
class sync
{
	mtx_t &lock;
	bool validity;

  public:
	sync(mtx_t &lock_ptr) : lock(lock_ptr), validity(true)
	{
		mtx_lock(&lock);
	}

	~sync()
	{
		if (validity)
			mtx_unlock(&lock);
	}

	void invalidateLock()
	{
		validity = false;
	}
};
#endif //	__cplusplus
#endif /* NATIVE_SYNC_H */
