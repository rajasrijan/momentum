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
#include <stdint.h>
#include <sys/types.h>

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

//	Socket is accepting connections.
#define SO_ACCEPTCONN (1)

//	Transmission of broadcast messages is supported.
#define SO_BROADCAST (2)

//	Debugging information is being recorded.
#define SO_DEBUG (3)

//	Bypass normal routing.
#define SO_DONTROUTE (4)

//	Socket error status.
#define SO_ERROR (5)

//	Connections are kept alive with periodic messages.
#define SO_KEEPALIVE (6)

//	Socket lingers on close.
#define SO_LINGER (7)

//	Out-of-band data is transmitted in line.
#define SO_OOBINLINE (8)

//	Receive buffer size.
#define SO_RCVBUF (9)

//	Receive ``low water mark''.
#define SO_RCVLOWAT (10)

//	Receive timeout.
#define SO_RCVTIMEO (11)

//	Reuse of local addresses is supported.
#define SO_REUSEADDR (12)

//	Send buffer size.
#define SO_SNDBUF (13)

//	Send ``low water mark''.
#define SO_SNDLOWAT (14)

//	Send timeout.
#define SO_SNDTIMEO (15)

//	Socket type.
#define SO_TYPE (16)

#define SOL_SOCKET 17

#define socklen_t unsigned long int
#define sa_family_t unsigned long int

struct sockaddr {
    sa_family_t sa_family; //  Address family.
    char sa_data[];        //  Socket address (variable-length data).
};

struct hostent {

    char *h_name;       // Official name of the host.
    char **h_aliases;   // A pointer to an array of pointers to alternative host names, terminated by a null pointer.
    int h_addrtype;     // Address type.
    int h_length;       // The length, in bytes, of the address.
    char **h_addr_list; // A pointer to an array of pointers to network addresses (in network byte order) for the host, terminated by a null pointer.
};

struct netent {

    char *n_name;     // Official, fully-qualified (including the domain) name of the host.
    char **n_aliases; // A pointer to an array of pointers to alternative network names, terminated by a null pointer.
    int n_addrtype;   // The address type of the network.
    uint32_t n_net;   // The network number, in host byte order.
};

struct protoent {

    char *p_name;     // Official name of the protocol.
    char **p_aliases; // A pointer to an array of pointers to alternative protocol names, terminated by a null pointer.
    int p_proto;      // The protocol number.
};

struct servent {

    char *s_name;     // Official name of the service.
    char **s_aliases; // A pointer to an array of pointers to alternative service names, terminated by a null pointer.
    int s_port;       // A value which, when converted to uint16_t, yields the port number in network byte order at which the service resides.
    char *s_proto;    // The name of the protocol to use when contacting the service.
};

int accept(int, struct sockaddr *restrict, socklen_t *restrict);
int bind(int, const struct sockaddr *, socklen_t);
int connect(int, const struct sockaddr *, socklen_t);
int getpeername(int, struct sockaddr *restrict, socklen_t *restrict);
int getsockname(int, struct sockaddr *restrict, socklen_t *restrict);
int getsockopt(int, int, int, void *restrict, socklen_t *restrict);
int listen(int, int);
ssize_t recv(int, void *, size_t, int);
ssize_t recvfrom(int, void *restrict, size_t, int, struct sockaddr *restrict, socklen_t *restrict);
ssize_t recvmsg(int, struct msghdr *, int);
ssize_t send(int, const void *, size_t, int);
ssize_t sendmsg(int, const struct msghdr *, int);
ssize_t sendto(int, const void *, size_t, int, const struct sockaddr *, socklen_t);
int setsockopt(int, int, int, const void *, socklen_t);
int shutdown(int, int);
int socket(int, int, int);
int sockatmark(int);
int socketpair(int, int, int, int[2]);

#endif //   SOCKET_H