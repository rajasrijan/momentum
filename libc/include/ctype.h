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

#ifndef _STDC_CTYPE_H_
#define _STDC_CTYPE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define isalnum(c) (isalpha(c) || isdigit(c))
#define isalpha(c) (islower(c) || isupper(c))
#define iscntrl(c) (((c) >= 0x0 && (c) <= 0x1F) || (c == 0x7F))
#define isdigit(c) ((c) >= '0' && (c) <= '9')
#define isgraph(c) (ispunct(c) || isalnum(c))
#define islower(c) ((c) >= 'a' && (c) <= 'z')
#define isprint(c) (isgraph(c) || isspace(c))
#define ispunct(c) (((c) >= 0x21 && (c) <= 0x2F) || ((c) >= 0x3A && (c) <= 0x40) || ((c) >= 0x5B && (c) <= 0x60) || ((c) >= 0x7B && (c) <= 0x7E))
#define isspace(c) ((c) == ' ' || (c) == '\t' || (c) == '\r' || (c) == '\n' || (c) == '\f' || (c) == '\v')
#define isupper(c) ((c) >= 'A' && (c) <= 'Z')
#define isxdigit(c) (isdigit(c) || ((c) >= 'a' && (c) <= 'f') || ((c) >= 'A' && (c) <= 'F'))
#define tolower(c) (isupper(c) ? ((c) + 'a' - 'A') : (c))
#define toupper(c) (islower(c) ? ((c) + 'A' - 'a') : (c))

    int isblank(int c);

#ifdef __cplusplus
}
#endif

#endif
