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

#ifndef POLL_H
#define POLL_H
typedef unsigned long int nfds_t;
struct pollfd {
    int fd;        //       The following descriptor being polled.
    short events;  //   The input event flags (see below).
    short revents; //  The output event flags (see below).
};

//	Data other than high-priority data may be read without blocking.
#define POLLIN (1 << 0)
//	Normal data may be read without blocking.
#define POLLRDNORM (1 << 1)
//	Priority data may be read without blocking.
#define POLLRDBAND (1 << 2)
//	High priority data may be read without blocking.
#define POLLPRI (1 << 3)
//	Normal data may be written without blocking.
#define POLLOUT (1 << 4)
//	Equivalent to POLLOUT.
#define POLLWRNORM (1 << 5)
//	Priority data may be written.
#define POLLWRBAND (1 << 6)
//	An error has occurred (revents only).
#define POLLERR (1 << 7)
//	Device has been disconnected (revents only).
#define POLLHUP (1 << 8)
//	Invalid fd member (revents only).
#define POLLNVAL (1 << 9)

#endif //   POLL_H