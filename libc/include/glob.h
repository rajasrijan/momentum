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

#ifndef GLOB_H
#define GLOB_H

typedef struct glob {
    size_t gl_pathc; // count of paths matched by pattern
    char **gl_pathv; // pointer to a list of matched pathnames
    size_t gl_offs;  //  slots to reserve at the beginning of gl_pathv
} glob_t;

//	Append generated pathnames to those previously obtained.
#define GLOB_APPEND (1 << 0)

//	Specify how many null pointers to add to the beginning of
#define GLOB_DOOFFS (1 << 0)

//	Cause glob() to return on error.
#define GLOB_ERR (1 << 0)

//	Each pathname that is a directory that matches pattern has a slash appended.
#define GLOB_MARK (1 << 0)

//	If pattern does not match any pathname, then return a list consisting of only pattern.
#define GLOB_NOCHECK (1 << 0)

//	Disable backslash escaping.
#define GLOB_NOESCAPE (1 << 0)

//	Do not sort the pathnames returned.
#define GLOB_NOSORT (1 << 0)

//	The scan was stopped because GLOB_ERR was set or returned non-zero.
#define GLOB_ABORTED (1 << 0)

//	The pattern does not match any existing pathname, and GLOB_NOCHECK was not set in flags.
#define GLOB_NOMATCH (1 << 0)

//	An attempt to allocate memory failed.
#define GLOB_NOSPACE (1 << 0)

//	The implementation does not support this function.
#define GLOB_NOSYS (1 << 0)

int glob(const char *, int, int (*)(const char *, int), glob_t *);
void globfree(glob_t *);

#endif // GLOB_H