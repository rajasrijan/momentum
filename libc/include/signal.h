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
//	(T) Pollable event. [Option End]
#define SIGPOLL 20
//	(T) Profiling timer expired. [Option End]
#define SIGPROF 21
//	(A) Bad system call. [Option End]
#define SIGSYS 22
//	(A) Trace/breakpoint trap. [Option End]
#define SIGTRAP 23
//	(I) High bandwidth data is available at a socket.
#define SIGURG 24
//	(T) Virtual timer expired.
#define SIGVTALRM 25
//	(A) CPU time limit exceeded.
#define SIGXCPU 26
//	(A) File size limit exceeded.
#define SIGXFSZ 27

#define SA_NOCLDSTOP 1
#define SA_ONSTACK 2
#define SA_RESETHAND 3
#define SA_RESTART 4
#define SA_SIGINFO 5
#define SA_NOCLDWAIT 6
#define SA_NODEFER 7
#define SS_ONSTACK 8
#define SS_DISABLE 9
#define MINSIGSTKSZ 10
#define SIGSTKSZ 11

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

int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
#endif //   SIGNAL_H