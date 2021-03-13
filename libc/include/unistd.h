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

#ifndef UNISTD_H
#define UNISTD_H

#include <stddef.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <errno.h>

/*
regex for generating defines
FIND:       [\x20]*([A-Z0-9_]+)\n[\x20]+([\x20-\x7E]+)
REPLACE:    //\t$2\n#define\t$1\t(\t)\n
*/

//	Integer value indicating version of the ISO POSIX-1 standard (C language binding).
#define _POSIX_VERSION (200809L)

//	Integer value indicating version of the ISO POSIX-2 standard (Commands).
#define _POSIX2_VERSION ()

//	Integer value indicating version of the ISO POSIX-2 standard (C language binding).
#define _POSIX2_C_VERSION ()

//	Integer value indicating version of the X/Open Portability Guide to which the implementation conforms.
#define _XOPEN_VERSION ()

// _POSIX_VERSION is defined in the ISO POSIX-1 standard. It changes with each new version of the ISO POSIX-1 standard.

// _POSIX2_VERSION is defined to have the value of the ISO POSIX-2 standard's POSIX2_VERSION limit. It changes with each new version of the ISO POSIX-2 standard.

// _POSIX2_C_VERSION is defined in the ISO POSIX-2 standard. It changes with each new version of the ISO POSIX-2 standard. When the C language binding option of the ISO POSIX-2 standard and therefore the X/Open POSIX2 C-language Binding Feature Group is not supported, _POSIX2_C_VERSION will be set to -1.

// _XOPEN_VERSION is defined as an integer value equal to 500.

// _XOPEN_XCU_VERSION is defined as an integer value indicating the version of the XCU specification to which the implementation conforms. If the value is -1, no commands and utilities are provided on the implementation. If the value is greater than or equal to 4, the functionality associated with the following symbols is also supported (see Mandatory Symbolic Constants and Constants for Options and Feature Groups ):

// _POSIX2_C_BIND
// _POSIX2_C_VERSION
// _POSIX2_CHAR_TERM
// _POSIX2_LOCALEDEF
// _POSIX2_UPE
// _POSIX2_VERSION

// If this constant is not defined use the sysconf() function to determine which features are supported.

// Each of the following symbolic constants is defined only if the implementation supports the indicated issue of the X/Open Portability Guide:

//	X/Open Portability Guide, Volume 2, January 1987, XVS System Calls and Libraries (ISBN: 0-444-70175-3).
#define _XOPEN_XPG2 ()

//	X/Open Specification, February 1992, System Interfaces and Headers, Issue 3 (ISBN: 1-872630-37-5, C212); this specification was formerly X/Open Portability Guide, Issue 3, Volume 2, January 1989, XSI System Interface and Headers (ISBN: 0-13-685843-0, XO/XPG/89/003).
#define _XOPEN_XPG3 ()

//	X/Open CAE Specification, July 1992, System Interfaces and Headers, Issue 4 (ISBN: 1-872630-47-2, C202).
#define _XOPEN_XPG4 ()

//	X/Open CAE Specification, January 1997, System Interfaces and Headers, Issue 5 (ISBN: 1-85912-181-0, C606).
#define _XOPEN_UNIX ()

//	The use of chown() is restricted to a process with appropriate privileges, and to changing the group ID of a file only to the effective group ID of the process or to one of its supplementary group IDs.
#define _POSIX_CHOWN_RESTRICTED ()

//	Pathname components longer than {NAME_MAX} generate an error.
#define _POSIX_NO_TRUNC ()

//	Terminal special characters defined in <termios.h> can be disabled using this character value.
#define _POSIX_VDISABLE ()

//	Each process has a saved set-user-ID and a saved set-group-ID.
#define _POSIX_SAVED_IDS ()

//	Implementation supports job control.
#define _POSIX_JOB_CONTROL ()

//	The implementation supports the threads option.
#define _POSIX_THREADS ()

//	The implementation supports the thread stack address attribute option.
#define _POSIX_THREAD_ATTR_STACKADDR ()

//	The implementation supports the thread stack size attribute option.
#define _POSIX_THREAD_ATTR_STACKSIZE ()

//	The implementation supports the process-shared synchronisation option.
#define _POSIX_THREAD_PROCESS_SHARED ()

//	The implementation supports the thread-safe functions option.
#define _POSIX_THREAD_SAFE_FUNCTIONS ()

//	Implementation supports the C Language Binding option. This will always have a value other than -1.
#define _POSIX2_C_BIND ()

//	Implementation supports the C Language Development Utilities option.
#define _POSIX2_C_DEV ()

//	Implementation supports at least one terminal type.
#define _POSIX2_CHAR_TERM ()

//	Implementation supports the FORTRAN Development Utilities option.
#define _POSIX2_FORT_DEV ()

//	Implementation supports the FORTRAN Run-time Utilities option.
#define _POSIX2_FORT_RUN ()

//	Implementation supports the creation of locales by the localedef utility.
#define _POSIX2_LOCALEDEF ()

//	Implementation supports the Software Development Utilities option.
#define _POSIX2_SW_DEV ()

//	The implementation supports the User Portability Utilities option.
#define _POSIX2_UPE ()

//	The implementation supports the X/Open Encryption Feature Group.
#define _XOPEN_CRYPT ()

//	The implementation supports the Issue 4, Version 2 Enhanced Internationalisation Feature Group. This is always set to a value other than -1.
#define _XOPEN_ENH_I18N ()

//	The implementation supports the Legacy Feature Group.
#define _XOPEN_LEGACY ()

//	The implementation supports the X/Open Realtime Feature Group.
#define _XOPEN_REALTIME ()

//	The implementation supports the X/Open Realtime Threads Feature Group.
#define _XOPEN_REALTIME_THREADS ()

//	The implementation supports the Issue 4, Version 2 Shared Memory Feature Group. This is always set to a value other than -1.
#define _XOPEN_SHM ()

//	Implementation provides a C-language compilation environment with 32-bit int, long, pointer and off_t types.
#define _XBS5_ILP32_OFF32 ()

//	Implementation provides a C-language compilation environment with 32-bit int, long and pointer types and an off_t type using at least 64 bits.
#define _XBS5_ILP32_OFFBIG ()

//	Implementation provides a C-language compilation environment with 32-bit int and 64-bit long, pointer and off_t types.
#define _XBS5_LP64_OFF64 ()

//	Implementation provides a C-language compilation environment with an int type using at least 32 bits and long, pointer and off_t types using at least 64 bits.
#define _XBS5_LPBIG_OFFBIG ()

//	Implementation supports the Asynchronous Input and Output option.
#define _POSIX_ASYNCHRONOUS_IO ()

//	Implementation supports the Process Memory Locking option.
#define _POSIX_MEMLOCK ()

//	Implementation supports the Range Memory Locking option.
#define _POSIX_MEMLOCK_RANGE ()

//	Implementation supports the Message Passing option.
#define _POSIX_MESSAGE_PASSING ()

//	Implementation supports the Process Scheduling option.
#define _POSIX_PRIORITY_SCHEDULING ()

//	Implementation supports the Realtime Signals Extension option.
#define _POSIX_REALTIME_SIGNALS ()

//	Implementation supports the Semaphores option.
#define _POSIX_SEMAPHORES ()

//	Implementation supports the Shared Memory Objects option.
#define _POSIX_SHARED_MEMORY_OBJECTS ()

//	Implementation supports the Synchronised Input and Output option.
#define _POSIX_SYNCHRONIZED_IO ()

//	Implementation supports the Timers option.
#define _POSIX_TIMERS ()

//	Implementation supports the File Synchronisation option.
#define _POSIX_FSYNC ()

//	Implementation supports the Memory Mapped Files option.
#define _POSIX_MAPPED_FILES ()

//	Implementation supports the Memory Protection option.
#define _POSIX_MEMORY_PROTECTION ()

//	Implementation supports the Prioritized Input and Output option.
#define _POSIX_PRIORITIZED_IO ()

//	The implementation supports the thread execution scheduling option.
#define _POSIX_THREAD_PRIORITY_SCHEDULING ()

//	The implementation supports the priority inheritance option.
#define _POSIX_THREAD_PRIO_INHERIT ()

//	The implementation supports the priority protection option.
#define _POSIX_THREAD_PRIO_PROTECT ()

//	Asynchronous input or output operations may be performed for the associated file.
#define _POSIX_ASYNC_IO ()

//	Prioritized input or output operations may be performed for the associated file.
#define _POSIX_PRIO_IO ()

//	Synchronised input or output operations may be performed for the associated file.
#define _POSIX_SYNC_IO ()

//	Null pointer
#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef FCNTL_H
//	Test for read permission.
#define R_OK (0)
//	Test for write permission.
#define W_OK (1)
//	Test for execute (search) permission.
#define X_OK (2)
//	Test for existence of file.
#define F_OK (3)
#endif //  FCNTL_H

//	If the ISO POSIX-2 standard is supported, this is the value for the environment variable that finds all standard utilities. Otherwise the meaning of this value is unspecified.
#define _CS_PATH ()

//	If sysconf(_SC_XBS5_ILP32_OFF32) returns -1, the meaning of this value is unspecified. Otherwise, this value is the set of initial options to be given to the cc and c89 utilities to build an application using a programming model with 32-bit int, long, pointer, and off_t types.
#define _CS_XBS5_ILP32_OFF32_CFLAGS ()

//	If sysconf(_SC_XBS5_ILP32_OFF32) returns -1, the meaning of this value is unspecified. Otherwise, this value is the set of final options to be given to the cc and c89 utilities to build an application using a programming model with 32-bit int, long, pointer, and off_t types.
#define _CS_XBS5_ILP32_OFF32_LDFLAGS ()

//	If sysconf(_SC_XBS5_ILP32_OFF32) returns -1, the meaning of this value is unspecified. Otherwise, this value is the set of libraries to be given to the cc and c89 utilities to build an application using a programming model with 32-bit int, long, pointer, and off_t types.
#define _CS_XBS5_ILP32_OFF32_LIBS ()

//	If sysconf(_SC_XBS5_ILP32_OFF32) returns -1, the meaning of this value is unspecified. Otherwise, this value is the set of options to be given to the lint utility to check application source using a programming model with 32-bit int, long, pointer, and off_t types.
#define _CS_XBS5_ILP32_OFF32_LINTFLAGS ()

//	If sysconf(_SC_XBS5_ILP32_OFFBIG) returns -1, the meaning of this value is unspecified. Otherwise, this value is the set of initial options to be given to the cc and c89 utilities to build an application using a programming model with 32-bit int, long, and pointer types, and an off_t type using at least 64 bits.
#define _CS_XBS5_ILP32_OFFBIG_CFLAGS ()

//	If sysconf(_SC_XBS5_ILP32_OFFBIG) returns -1, the meaning of this value is unspecified. Otherwise, this value is the set of final options to be given to the cc and c89 utilities to build an application using a programming model with 32-bit int, long, and pointer types, and an off_t type using at least 64 bits.
#define _CS_XBS5_ILP32_OFFBIG_LDFLAGS ()

//	If sysconf(_SC_XBS5_ILP32_OFFBIG) returns -1, the meaning of this value is unspecified. Otherwise, this value is the set of libraries to be given to the cc and c89 utilities to build an application using a programming model with 32-bit int, long, and pointer types, and an off_t type using at least 64 bits.
#define _CS_XBS5_ILP32_OFFBIG_LIBS ()

//	If sysconf(_SC_XBS5_ILP32_OFFBIG) returns -1, the meaning of this value is unspecified. Otherwise, this value is the set of options to be given to the lint utility to check an application using a programming model with 32-bit int, long, and pointer types, and an off_t type using at least 64 bits.
#define _CS_XBS5_ILP32_OFFBIG_LINTFLAGS ()

//	If sysconf(_SC_XBS5_LP64_OFF64) returns -1, the meaning of this value is unspecified. Otherwise, this value is the set of initial options to be given to the cc and c89 utilities to build an application using a programming model with 64-bit int, long, pointer, and off_t types.
#define _CS_XBS5_LP64_OFF64_CFLAGS ()

//	If sysconf(_SC_XBS5_LP64_OFF64) returns -1, the meaning of this value is unspecified. Otherwise, this value is the set of final options to be given to the cc and c89 utilities to build an application using a programming model with 64-bit int, long, pointer, and off_t types.
#define _CS_XBS5_LP64_OFF64_LDFLAGS ()

//	If sysconf(_SC_XBS5_LP64_OFF64) returns -1, the meaning of this value is unspecified. Otherwise, this value is the set of libraries to be given to the cc and c89 utilities to build an application using a programming model with 64-bit int, long, pointer, and off_t types.
#define _CS_XBS5_LP64_OFF64_LIBS ()

//	If sysconf(_SC_XBS5_LP64_OFF64) returns -1, the meaning of this value is unspecified. Otherwise, this value is the set of options to be given to the lint utility to check application source using a programming model with 64-bit int, long, pointer, and off_t types.
#define _CS_XBS5_LP64_OFF64_LINTFLAGS ()

//	If sysconf(_SC_XBS5_LPBIG_OFFBIG) returns -1, the meaning of this value is unspecified. Otherwise, this value is the set of initial options to be given to the cc and c89 utilities to build an application using a programming model with an int type using at least 32 bits and long, pointer, and off_t types using at least 64 bits.
#define _CS_XBS5_LPBIG_OFFBIG_CFLAGS ()

//	If sysconf(_SC_XBS5_LPBIG_OFFBIG) returns -1, the meaning of this value is unspecified. Otherwise, this value is the set of final options to be given to the cc and c89 utilities to build an application using a programming model with an int type using at least 32 bits and long, pointer, and off_t types using at least 64 bits.
#define _CS_XBS5_LPBIG_OFFBIG_LDFLAGS ()

//	If sysconf(_SC_XBS5_LPBIG_OFFBIG) returns -1, the meaning of this value is unspecified. Otherwise, this value is the set of libraries to be given to the cc and c89 utilities to build an application using a programming model with an int type using at least 32 bits and long, pointer, and off_t types using at least 64 bits.
#define _CS_XBS5_LPBIG_OFFBIG_LIBS ()

//	If sysconf(_SC_XBS5_LPBIG_OFFBIG) returns -1, the meaning of this value is unspecified. Otherwise, this value is the set of options to be given to the lint utility to check application source using a programming model with an int type using at least 32 bits and long, pointer, and off_t types using at least 64 bits.
#define _CS_XBS5_LPBIG_OFFBIG_LINTFLAGS ()

#ifndef STDIO_H
//	Set file offset to offset.
#define SEEK_SET (0)

//	Set file offset to current plus offset.
#define SEEK_CUR (1)

//	Set file offset to EOF plus offset.
#define SEEK_END (2)
#endif //  STDIO_H
//	Lock a section for exclusive use.
#define F_LOCK ()

//	Unlock locked sections.
#define F_ULOCK ()

//	Test section for locks by other processes.
#define F_TEST ()

//	Test and lock a section for exclusive use.
#define F_TLOCK ()

// The following symbolic constants are defined for pathconf():

// _PC_ASYNC_IO
// _PC_CHOWN_RESTRICTED
// _PC_FILESIZEBITS
// _PC_LINK_MAX
// _PC_MAX_CANON
// _PC_MAX_INPUT
// _PC_NAME_MAX
// _PC_NO_TRUNC
// _PC_PATH_MAX
// _PC_PIPE_BUF
// _PC_PRIO_IO
// _PC_SYNC_IO
// _PC_VDISABLE
#define _SC_CLK_TCK 5
//	File number of stdin. It is 0.
#define STDIN_FILENO (0)

//	File number of stdout. It is 1.
#define STDOUT_FILENO (1)

//	File number of stderr. It is 2.
#define STDERR_FILENO (2)

int access(const char *, int);
unsigned int alarm(unsigned int);
int brk(void *);
int chdir(const char *);
int chroot(const char *);
int chown(const char *, uid_t, gid_t);
int close(int);
size_t confstr(int, char *, size_t);
char *crypt(const char *, const char *);
char *ctermid(char *);
char *cuserid(char *s);
int dup(int);
int dup2(int, int);
void encrypt(char[64], int);
int execl(const char *, const char *, ...);
int execle(const char *, const char *, ...);
int execlp(const char *, const char *, ...);
int execv(const char *, char *const[]);
int execve(const char *, char *const[], char *const[]);
int execvp(const char *, char *const[]);
void _exit(int);
int fchown(int, uid_t, gid_t);
int fchdir(int);
int fdatasync(int);
pid_t fork(void);
long int fpathconf(int, int);
int fsync(int);
int ftruncate(int, off_t);
char *getcwd(char *, size_t);
int getdtablesize(void);

gid_t getegid(void);
uid_t geteuid(void);
gid_t getgid(void);
gid_t getgroups(int n, gid_t *groups);
long gethostid(void);
char *getlogin(void);
int getlogin_r(char *, size_t);
int getopt(int, char *const[], const char *);
int getpagesize(void);
char *getpass(const char *);

pid_t getpgid(pid_t);
pid_t getpgrp(void);
pid_t getpid(void);
pid_t getppid(void);
pid_t getsid(pid_t);
uid_t getuid(void);
char *getwd(char *);
int isatty(int);
int lchown(const char *, uid_t, gid_t);
int link(const char *, const char *);
int lockf(int, int, off_t);
off_t lseek(int, off_t, int);
int nice(int);
long int pathconf(const char *, int);
int pause(void);
int pipe(int[2]);
ssize_t pread(int, void *, size_t, off_t);
int pthread_atfork(void (*)(void), void (*)(void),
                   void (*)(void));
ssize_t pwrite(int, const void *, size_t, off_t);
ssize_t read(int, void *, size_t);
int readlink(const char *, char *, size_t);
int rmdir(const char *);
void *sbrk(intptr_t);
int setgid(gid_t);
int setpgid(pid_t, pid_t);
pid_t setpgrp(void);
int setregid(gid_t, gid_t);
int setreuid(uid_t, uid_t);
pid_t setsid(void);
int setuid(uid_t);
//unsigned int sleep(unsigned int);
void swab(const void *, void *, ssize_t);
int symlink(const char *, const char *);
void sync(void);
long int sysconf(int);
pid_t tcgetpgrp(int);
int tcsetpgrp(int, pid_t);
int truncate(const char *, off_t);
char *ttyname(int);
int ttyname_r(int, char *, size_t);
useconds_t ualarm(useconds_t, useconds_t);
int unlink(const char *);
int usleep(useconds_t);
pid_t vfork(void);
ssize_t write(int, const void *, size_t);

extern char *optarg;
extern int optind, opterr, optopt;

//  OLD

int chdir(const char *path);
char *getcwd(char *buf, size_t size);
int close(int fd);
int fsync(int fd);
int fdatasync(int fd);

#endif