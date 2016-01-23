/*
 * Copyright 2009-2012 Srijan Kumar Sharma
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

#ifndef THREADS_H
#define	THREADS_H

#include <stdint.h>

enum {
    thrd_success,
    thrd_nomem,
    thrd_timedout,
    thrd_busy,
    thrd_error
};

typedef uint32_t mtx_t;
int mtx_init( mtx_t* mutex, int type );
int mtx_lock( mtx_t* mutex );
int mtx_trylock( mtx_t *mutex );
int mtx_unlock( mtx_t *mutex );
void mtx_destroy( mtx_t *mutex );


#endif	/* THREADS_H */

