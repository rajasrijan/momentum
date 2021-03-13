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

#ifndef UTSNAME_H
#define UTSNAME_H

struct utsname {
    char sysname[256];  //  Name of this implementation of the operating system.
    char nodename[256]; // Name of this node within the communications network to which this node is attached, if any.
    char release[256];  //  Current release level of this implementation.
    char version[256];  //  Current version level of this release.
    char machine[256];  //  Name of the hardware type on which the system is running.
};

int uname(struct utsname *);

#endif //  UTSNAME_H