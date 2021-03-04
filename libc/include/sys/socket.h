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


#ifndef SOCKET_H
#define SOCKET_H

#define SOCK_STREAM (1 << 0)
#define SOCK_DGRAM (1 << 1)
#define SOCK_SEQPACKET (1 << 2)
#define SOCK_RAW (1 << 3)
#define SOCK_RDM (1 << 4)
#define SOCK_PACKET (1 << 5)
#define SOCK_NONBLOCK (1 << 6)
#define SOCK_CLOEXEC (1 << 7)

#define AF_UNIX (1 << 0)
#define AF_UNSPEC (1 << 1)
#define AF_INET (1 << 2)
#define AF_INET6 (1 << 3)

#define socklen_t unsigned long int
#define sa_family_t unsigned long int

struct sockaddr {
    sa_family_t sa_family; //  Address family.
    char sa_data[];        //  Socket address (variable-length data).
};

#endif //   SOCKET_H