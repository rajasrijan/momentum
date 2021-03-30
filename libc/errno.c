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

#include <errno.h>
#if __STDC_HOSTED__ == 1
int errno = 0;
static char err_str[][256] = {
    "No Error",
    "Operation not permitted",
    "No such file or directory",
    "No such process",
    "Interrupted function call",
    "Input/output error",
    "No such device or address",
    "Arg list too long",
    "Exec format error",
    "Bad file descriptor",
    "No child processes",
    "Resource temporarily unavailable",
    "Not enough space",
    "Permission denied",
    "Bad address",
    "15 - Unknown Error",
    "Resource device",
    "File exists",
    "Improper link (cross-device link?)",
    "No such device",
    "Not a directory",
    "Is a directory",
    "Invalid argument",
    "Too many open files in system",
    "Too many open files",
    "Inappropriate I/O control operation",
    "26 - Unknown Error",
    "File too large",
    "No space left on device",
    "Invalid seek (seek on a pipe?)",
    "Read-only file system",
    "Too many links",
    "Broken pipe",
    "Domain error (math functions)",
    "Result too large (possibly too small)",
    "35 - Unknown Error",
    "Resource deadlock avoided (non-Cyg)",
    "37 - Unknown Error",
    "Filename too long (91 in Cyg?)",
    "No locks available (46 in Cyg?)",
    "Function not implemented (88 in Cyg?)",
    "Directory not empty (90 in Cyg?)",
    "Illegal byte sequence",
    "Loop",
    "Timed out",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Unknown error",
    "Operation not supported on transport endpoint",
    "Unknown error",
    "Address family not supported by protocol",
};
char *strerror(int errnum)
{
    return err_str[errnum];
}
#endif