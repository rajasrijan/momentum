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

#ifndef WAIT_H
#define WAIT_H
#include <sys/types.h>
#include <signal.h>

//	  Report status of continued child process.
#define WCONTINUED (1 << 0)

//	Do not hang if no status is available; return immediately.
#define WNOHANG (1 << 0)

//	Report status of stopped child process.
#define WUNTRACED (1 << 0)

//	Return exit status.
#define WEXITSTATUS(x) ((x)&0xFF)

//	  True if child has been continued.
#define WIFCONTINUED (1 << 0)

//	True if child exited normally.
#define WIFEXITED(x) (x >= 0)

//	True if child exited due to uncaught signal.
#define WIFSIGNALED(x) (WIFEXITED(x) ? WEXITSTATUS(x) : 0)

//	True if child is currently stopped.
#define WIFSTOPPED (1 << 0)

//	Return signal number that caused process to stop.
#define WSTOPSIG (1 << 0)

//	Return signal number that caused process to terminate.
#define WTERMSIG(x) WEXITSTATUS(x)

//	Wait for processes that have exited.
#define WEXITED (1 << 0)

//	Keep the process whose status is returned in infop in a waitable state.
#define WNOWAIT (1 << 0)

//	Status is returned for any child that has stopped upon receipt of a signal.
#define WSTOPPED (1 << 0)

typedef enum idtype {
    P_ALL,
    P_PGID,
    P_PID
} idtype_t;

pid_t wait(int *);
int waitid(idtype_t idtype, id_t id, siginfo_t *siginfo, int i);
pid_t waitpid(pid_t, int *, int);

#endif //   WAIT_H