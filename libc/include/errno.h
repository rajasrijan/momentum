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

#ifndef _ERRNO_H
#define _ERRNO_H

#if __STDC_HOSTED__ == 1
#ifdef __cplusplus
extern "C"
{
#endif
    extern int errno;
    extern char *program_invocation_short_name;
    extern char *program_invocation_name;
#ifdef __cplusplus
}
#endif
#endif

#define EPERM 1    /* Operation not permitted */
#define ENOFILE 2  /* No such file or directory */
#define ENOENT 2   /*  No such entry */
#define ESRCH 3    /* No such process */
#define EINTR 4    /* Interrupted function call */
#define EIO 5      /* Input/output error */
#define ENXIO 6    /* No such device or address */
#define E2BIG 7    /* Arg list too long */
#define ENOEXEC 8  /* Exec format error */
#define EBADF 9    /* Bad file descriptor */
#define ECHILD 10  /* No child processes */
#define EAGAIN 11  /* Resource temporarily unavailable */
#define ENOMEM 12  /* Not enough space */
#define EACCES 13  /* Permission denied */
#define EFAULT 14  /* Bad address */
#define EUNK 15    /* Unknown Error */
#define EBUSY 16   /* strerror reports "Resource device" */
#define EEXIST 17  /* File exists */
#define EXDEV 18   /* Improper link (cross-device link?) */
#define ENODEV 19  /* No such device */
#define ENOTDIR 20 /* Not a directory */
#define EISDIR 21  /* Is a directory */
#define EINVAL 22  /* Invalid argument */
#define ENFILE 23  /* Too many open files in system */
#define EMFILE 24  /* Too many open files */
#define ENOTTY 25  /* Inappropriate I/O control operation */
/* 26 - Unknown Error */
#define EFBIG 27  /* File too large */
#define ENOSPC 28 /* No space left on device */
#define ESPIPE 29 /* Invalid seek (seek on a pipe?) */
#define EROFS 30  /* Read-only file system */
#define EMLINK 31 /* Too many links */
#define EPIPE 32  /* Broken pipe */
#define EDOM 33   /* Domain error (math functions) */
#define ERANGE 34 /* Result too large (possibly too small) */
/* 35 - Unknown Error */
#define EDEADLOCK 36 /* Resource deadlock avoided (non-Cyg) */
#define EDEADLK 36
/* 37 - Unknown Error */
#define ENAMETOOLONG 38 /* Filename too long (91 in Cyg?) */
#define ENOLCK 39       /* No locks available (46 in Cyg?) */
#define ENOSYS 40       /* Function not implemented (88 in Cyg?) */
#define ENOTEMPTY 41    /* Directory not empty (90 in Cyg?) */
#define EILSEQ 42       /* Illegal byte sequence */
#define ELOOP 43
#define ETIMEDOUT 44
#define EOPNOTSUPP 95   /* Operation not supported on transport endpoint */
#define EAFNOSUPPORT 97 /* Address family not supported by protocol */
#endif
