#ifndef SIGNAL_H
#define SIGNAL_H

#include <sys/types.h>

#define SIGABRT 0
#define SIGFPE 1
#define SIGILL 2
#define SIGINT 3
#define SIGSEGV 4
#define SIGTERM 5
#define SIGALRM 6
#define SIGCHLD 7
#define SIGQUIT 8
#define SIGPIPE 9

#define SIG_ERR 2
#define SIG_DFL 1
#define SIG_IGN 0

typedef uint64_t sigset_t;
typedef uint64_t  sig_atomic_t;

#endif //   SIGNAL_H