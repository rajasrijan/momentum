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


#ifndef TYPES_H
#define TYPES_H
#include <stdint.h>
#include <stddef.h>

typedef uint64_t off_t;
typedef uint64_t mode_t;
typedef uint64_t pid_t;
typedef uint64_t dev_t;
#define __pid_t_defined
typedef uint64_t uid_t;
#define __uid_t_defined
typedef uint64_t gid_t;
#define __gid_t_defined
typedef unsigned int u_int;
typedef unsigned long u_long;
#define ssize_t signed long
typedef uint64_t blkcnt_t;

typedef uint64_t blksize_t;

typedef uint64_t clock_t;

typedef uint64_t clockid_t;

typedef uint64_t dev_t;

typedef uint64_t fsblkcnt_t;

typedef uint64_t fsfilcnt_t;

typedef uint64_t gid_t;

typedef uint64_t id_t;

typedef uint64_t ino_t;

typedef uint64_t key_t;

typedef uint64_t mode_t;

typedef uint64_t nlink_t;

typedef uint64_t off_t;

typedef uint64_t pid_t;

typedef uint64_t pthread_attr_t;

typedef uint64_t pthread_barrier_t;

typedef uint64_t pthread_barrierattr_t;

typedef uint64_t pthread_cond_t;

typedef uint64_t pthread_condattr_t;

typedef uint64_t pthread_key_t;

typedef uint64_t pthread_mutex_t;

typedef uint64_t pthread_mutexattr_t;

typedef uint64_t pthread_once_t;

typedef uint64_t pthread_rwlock_t;

typedef uint64_t pthread_rwlockattr_t;

typedef uint64_t pthread_spinlock_t;

typedef uint64_t pthread_t;

typedef uint64_t suseconds_t;

typedef uint64_t time_t;

typedef uint64_t timer_t;

typedef uint64_t trace_attr_t;

typedef uint64_t trace_event_id_t;

typedef uint64_t trace_event_set_t;

typedef uint64_t trace_id_t;

typedef uint64_t uid_t;
#endif //TYPES_H
