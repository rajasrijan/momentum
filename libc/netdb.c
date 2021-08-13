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

#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#if __STDC_HOSTED__ == 1
int getaddrinfo(const char *node, const char *service,
                const struct addrinfo *hints,
                struct addrinfo **res)
{
    asm("cli;hlt");
    return -ENOSYS;
}

void freeaddrinfo(struct addrinfo *res)
{
    asm("cli;hlt");
    errno = ENOSYS;
}

const char *gai_strerror(int errcode)
{
    asm("cli;hlt");
    errno = ENOSYS;
    return 0;
}
#endif //  __STDC_HOSTED__==1
