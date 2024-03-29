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

#ifndef _STDDEF_H_
#define _STDDEF_H_

#ifndef __cplusplus
#define NULL ((void *)0)
#else
#define NULL 0
#endif //__cplusplus

#ifndef NAME_MAX
#define NAME_MAX 256
#endif

#if __STDC_HOSTED__ == 1
#define size_t unsigned long
#else
#if defined(_arch_x86_64_)
#define size_t unsigned long
#elif defined(_arch_efi_)
#define size_t unsigned long long
#else
#error "Arch is not defined"
#endif
#endif
#define ptrdiff_t int64_t

#define offsetof(S, d) ((size_t)(&(((S *)0)->d)))

#endif //_STDDEF_H_
