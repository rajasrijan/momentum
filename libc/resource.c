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

#include <sys/resource.h>
#include <errno.h>

int getpriority(int, id_t)
{
    return -ENOSYS;
}
int getrlimit(int, struct rlimit *)
{
    return -ENOSYS;
}
int getrusage(int, struct rusage *)
{
    return -ENOSYS;
}
int setpriority(int, id_t, int)
{
    return -ENOSYS;
}
int setrlimit(int, const struct rlimit *)
{
    return -ENOSYS;
}