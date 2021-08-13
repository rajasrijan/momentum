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

#ifndef IN_H
#define IN_H

#include <stdint.h>
#include <sys/socket.h>

#define in_addr_t uint32_t
#define in_port_t uint16_t

struct in_addr
{
    in_addr_t s_addr;
};

struct sockaddr_in
{
    sa_family_t sin_family;  // AF_INET.
    in_port_t sin_port;      // Port number.
    struct in_addr sin_addr; // IP address.
};

//  IPv4 local host address.
#define INADDR_ANY (1 << 0)
// IPv4 broadcast address.
#define INADDR_BROADCAST (1 << 1)

#endif