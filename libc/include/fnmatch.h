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

#ifndef FNMATCH_H
#define FNMATCH_H

//	The string does not match the specified pattern
#define FNM_NOMATCH (1<<0)
//	Slash in string only matches slash in pattern
#define FNM_PATHNAME (1<<1)
//	Leading period in string must be exactly matched by period in pattern
#define FNM_PERIOD (1<<2)
//	Disable backslash escaping
#define FNM_NOESCAPE (1<<3)
//	The implementation does not support this function
#define FNM_NOSYS (1<<4)

int fnmatch(const char *, const char *, int);
#endif //FNMATCH_H