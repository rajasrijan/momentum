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

#ifndef SIGNAL_H
#define SIGNAL_H

#include <sys/types.h>
#include <time.h>

#define SIG_ERR 2
#define SIG_DFL 1
#define SIG_IGN 0

//	(A) Process abort signal.
#define SIGABRT 0
//	(T) Alarm clock.
#define SIGALRM 1
//	(A) Access to an undefined portion of a memory object.
#define SIGBUS 2
//	(I) Child process terminated, stopped,or continued.
#define SIGCHLD 3
//	(C) Continue executing, if stopped.
#define SIGCONT 4
//	(A) Erroneous arithmetic operation.
#define SIGFPE 5
//	(T) Hangup.
#define SIGHUP 6
//	(A) Illegal instruction.
#define SIGILL 7
//	(T) Terminal interrupt signal.
#define SIGINT 8
//	(T) Kill (cannot be caught or ignored).
#define SIGKILL 9
//	(T) Write on a pipe with no one to read it.
#define SIGPIPE 10
//	(A) Terminal quit signal.
#define SIGQUIT 11
//	(A) Invalid memory reference.
#define SIGSEGV 12
//	(S) Stop executing (cannot be caught or ignored).
#define SIGSTOP 13
//	(T) Termination signal.
#define SIGTERM 14
//	(S) Terminal stop signal.
#define SIGTSTP 15
//	(S) Background process attempting read.
#define SIGTTIN 16
//	(S) Background process attempting write.
#define SIGTTOU 17
//	(T) User-defined signal 1.
#define SIGUSR1 18
//	(T) User-defined signal 2.
#define SIGUSR2 19
//	(T) Pollable event.
#define SIGPOLL 20
//	(T) Profiling timer expired.
#define SIGPROF 21
//	(A) Bad system call.
#define SIGSYS 22
//	(A) Trace/breakpoint trap.
#define SIGTRAP 23
//	(I) High bandwidth data is available at a socket.
#define SIGURG 24
//	(T) Virtual timer expired.
#define SIGVTALRM 25
//	(A) CPU time limit exceeded.
#define SIGXCPU 26
//	(A) File size limit exceeded.
#define SIGXFSZ 27

#define NSIG 64

typedef uint64_t sigset_t;
typedef uint64_t sig_atomic_t;

union sigval {       /* Data passed with notification */
    int sival_int;   /* Integer value */
    void *sival_ptr; /* Pointer value */
};

typedef struct siginfo {
    int si_signo;          /* Signal number */
    int si_errno;          /* An errno value */
    int si_code;           /* Signal code */
    int si_trapno;         /* Trap number that caused
                                         hardware-generated signal
                                         (unused on most architectures) */
    pid_t si_pid;          /* Sending process ID */
    uid_t si_uid;          /* Real user ID of sending process */
    int si_status;         /* Exit value or signal */
    uint64_t si_utime;     /* User time consumed */
    uint64_t si_stime;     /* System time consumed */
    union sigval si_value; /* Signal value */
    int si_int;            /* POSIX.1b signal */
    void *si_ptr;          /* POSIX.1b signal */
    int si_overrun;        /* Timer overrun count;
                                         POSIX.1b timers */
    int si_timerid;        /* Timer ID; POSIX.1b timers */
    void *si_addr;         /* Memory location which caused fault */
    long si_band;          /* Band event (was int in
                                         glibc 2.3.2 and earlier) */
    int si_fd;             /* File descriptor */
    short si_addr_lsb;     /* Least significant bit of address
                                         (since Linux 2.6.32) */
    void *si_lower;        /* Lower bound when address violation
                                         occurred (since Linux 3.19) */
    void *si_upper;        /* Upper bound when address violation
                                         occurred (since Linux 3.19) */
    int si_pkey;           /* Protection key on PTE that caused
                                         fault (since Linux 4.6) */
    void *si_call_addr;    /* Address of system call instruction
                                         (since Linux 3.5) */
    int si_syscall;        /* Number of attempted system call
                                         (since Linux 3.5) */
    unsigned int si_arch;  /* Architecture of attempted system call
                                         (since Linux 3.5) */
} siginfo_t;

struct sigaction {
    void (*sa_handler)(int);
    void (*sa_sigaction)(int, siginfo_t *, void *);
    sigset_t sa_mask;
    int sa_flags;
    void (*sa_restorer)(void);
};

//	Do not generate SIGCHLD when children stop.
#define SA_NOCLDSTOP (1 << 0)

//	The resulting set is the union of the current set and the signal set pointed to by the argument set.
#define SIG_BLOCK (1 << 0)

//	The resulting set is the intersection of the current set and the complement of the signal set pointed to by the argument set.
#define SIG_UNBLOCK (1 << 0)

//	The resulting set is the signal set pointed to by the argument set.
#define SIG_SETMASK (1 << 0)

//	Causes signal delivery to occur on an alternate stack.
#define SA_ONSTACK (1 << 0)

//	Causes signal dispositions to be set to SIG_DFL on entry to signal handlers.
#define SA_RESETHAND (1 << 0)

//	Causes certain functions to become restartable.
#define SA_RESTART (1 << 0)

//	Causes extra information to be passed to signal handlers at the time of receipt of a signal.
#define SA_SIGINFO (1 << 0)

//	Causes implementations not to create zombie processes on child death.
#define SA_NOCLDWAIT (1 << 0)

//	Causes signal not to be automatically blocked on entry to signal handler.
#define SA_NODEFER (1 << 0)

//	Process is executing on an alternate signal stack.
#define SS_ONSTACK (1 << 0)

//	Alternate signal stack is disabled.
#define SS_DISABLE (1 << 0)

//	Minimum stack size for a signal handler.
#define MINSIGSTKSZ (1 << 0)

//	Default size in bytes for the alternate signal stack.
#define SIGSTKSZ (1 << 0)

typedef struct stack {

    void *ss_sp;    //       stack base or pointer
    size_t ss_size; //     stack size
    int ss_flags;   //    flags
} stack_t;
struct sigstack {

    int ss_onstack; //  non-zero when signal stack is in use
    void *ss_sp;    //       signal stack pointer
};

void (*bsd_signal(int, void (*)(int)))(int);
int kill(pid_t, int);
int killpg(pid_t, int);
int pthread_kill(pthread_t, int);
int pthread_sigmask(int, const sigset_t *, sigset_t *);
int raise(int);
int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
int sigaddset(sigset_t *, int);
int sigaltstack(const stack_t *, stack_t *);
int sigdelset(sigset_t *, int);
int sigemptyset(sigset_t *);
int sigfillset(sigset_t *);
int sighold(int);
int sigignore(int);
int siginterrupt(int, int);
int sigismember(const sigset_t *, int);
void (*signal(int, void (*)(int)))(int);
int sigpause(int);
int sigpending(sigset_t *);
int sigprocmask(int, const sigset_t *, sigset_t *);
int sigqueue(pid_t, int, const union sigval);
int sigrelse(int);
void (*sigset(int, void (*)(int)))(int);
int sigstack(struct sigstack *ss,
             struct sigstack *oss);
int sigsuspend(const sigset_t *);
int sigtimedwait(const sigset_t *, siginfo_t *, const struct timespec *);
int sigwait(const sigset_t *set, int *sig);
int sigwaitinfo(const sigset_t *, siginfo_t *);
#endif //   SIGNAL_H