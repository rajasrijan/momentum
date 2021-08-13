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

#ifndef NETDB_H
#define NETDB_H
#include <sys/socket.h>

struct addrinfo
{
    int ai_flags;
    int ai_family;
    int ai_socktype;
    int ai_protocol;
    socklen_t ai_addrlen;
    struct sockaddr *ai_addr;
    char *ai_canonname;
    struct addrinfo *ai_next;
};

//	Socket address is intended for bind().
#define AI_PASSIVE (1 << 0)
//	Request for canonical name.
#define AI_CANONNAME (1 << 1)
//	Return numeric host address as name.
#define AI_NUMERICHOST (1 << 2)
//	Inhibit service name resolution.
#define AI_NUMERICSERV (1 << 3)
//	If no IPv6 addresses are found, query for IPv4 addresses and return them to the caller as IPv4-mapped IPv6 addresses.
#define AI_V4MAPPED (1 << 4)
//	Query for both IPv4 and IPv6 addresses.
#define AI_ALL (1 << 5)

//	Query for IPv4 addresses only when an IPv4 address is configured; query for IPv6 addresses only when an IPv6 address is configured.
#define AI_ADDRCONFIG (1 << 6)
//	Only the nodename portion of the FQDN is returned for local hosts.
#define NI_NOFQDN (1 << 0)
//	The numeric form of the node's address is returned instead of its name.
#define NI_NUMERICHOST (1 << 1)
//	Return an error if the node's name cannot be located in the database.
#define NI_NAMEREQD (1 << 2)
//	The numeric form of the service address is returned instead of its name.
#define NI_NUMERICSERV (1 << 3)
//	For IPv6 addresses, the numeric form of the scope identifier is returned instead of its name.
#define NI_NUMERICSCOPE (1 << 4)
//	Indicates that the service is a datagram service (SOCK_DGRAM).
#define NI_DGRAM (1 << 5)

int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);

void freeaddrinfo(struct addrinfo *res);

const char *gai_strerror(int errcode);
extern int h_errno;
#endif //  NETDB_H