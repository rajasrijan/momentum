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

#ifndef RESOURCE_H
#define RESOURCE_H
#include <stdint.h>
#include <sys/types.h>
#include <sys/time.h>

//	Identifies the who argument as a process ID.
#define PRIO_PROCESS (1 << 0)
//	Identifies the who argument as a process group ID.
#define PRIO_PGRP (1 << 0)
//	Identifies the who argument as a user ID.
#define PRIO_USER (1 << 0)

typedef uint64_t rlim_t;

//	A value of rlim_t indicating no limit.
#define RLIM_INFINITY (1 << 0)
//	A value of type rlim_t indicating an unrepresentable saved hard limit.
#define RLIM_SAVED_MAX (1 << 0)
//	A value of type rlim_t indicating an unrepresentable saved soft limit.
#define RLIM_SAVED_CUR (1 << 0)

//	Returns information about the current process.
#define RUSAGE_SELF (1 << 0)
//	Returns information about children of the current process.
#define RUSAGE_CHILDREN (1 << 0)

struct rlimit
{

    rlim_t rlim_cur; //  The current (soft) limit.
    rlim_t rlim_max; //  The hard limit.
};
struct rusage
{

    struct timeval ru_utime; //  User time used.
    struct timeval ru_stime; //  System time used.
};

//	Limit on size of core file.
#define RLIMIT_CORE (1 << 0)
//	Limit on CPU time per process.
#define RLIMIT_CPU (1 << 0)
//	Limit on data segment size.
#define RLIMIT_DATA (1 << 0)
//	Limit on file size.
#define RLIMIT_FSIZE (1 << 0)
//	Limit on number of open files.
#define RLIMIT_NOFILE (1 << 0)
//	Limit on stack size.
#define RLIMIT_STACK (1 << 0)
//	Limit on address space size.
#define RLIMIT_AS (1 << 0)

int getpriority(int, id_t);
int getrlimit(int, struct rlimit *);
int getrusage(int, struct rusage *);
int setpriority(int, id_t, int);
int setrlimit(int, const struct rlimit *);

#endif // RESOURCE_H