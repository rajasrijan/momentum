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

#ifndef TERMIOS_H
#define TERMIOS_H
#include <stdint.h>

//  c_iflag flag constants
#define IGNBRK 0
#define BRKINT 1
#define IGNPAR 2
#define INPCK 3
#define ISTRIP 4
#define INLCR 5
#define IGNCR 6
#define ICRNL 7
#define IUCLC 8
#define IXON 9
#define IXANY 10
#define IXOFF 11
#define IMAXBEL 12
#define IUTF8 13

//  c_oflag flag constants :

#define OPOST 0
#define OLCUC 1
#define ONLCR 2
#define OCRNL 3
#define ONOCR 4
#define ONLRET 5
#define OFILL 6
#define OFDEL 7
#define NLDLY 8
#define CRDLY 9
#define TABDLY 10
#define BSDLY 11
#define VTDLY 12
#define FFDLY 13

//  c_cflag flag constants :
#define CBAUD 0
#define CBAUDEX 1
#define CSIZE 2
#define CS5 1
#define CS6 2
#define CS7 4
#define CS8 8
#define CSTOPB 3
#define CREAD 4
#define PARENB 5
#define PARODD 6
#define HUPCL 7
#define CLOCAL 8
#define LOBLK 9
#define CIBAUD 10
#define CMSPAR 11
#define CRTSCTS 12

//  c_lflag flag constants :
#define ISIG 0
#define ICANON 1
#define XCASE 2
#define ECHO 3
#define ECHOE 4
#define ECHOK 5
#define ECHONL 6
#define ECHOCTL 7
#define ECHOPRT 8
#define ECHOKE 9
#define DEFECHO 10
#define FLUSHO 11
#define NOFLSH 12
#define TOSTOP 13
#define PENDIN 14
#define IEXTEN 15

struct termio
{
    uint64_t c_lflag;
    uint64_t c_cflag;
    uint64_t c_iflag;
    uint64_t c_oflag;
    uint64_t c_cc[256];
};

int tcgetattr(int fd, struct termio *termios_p);
#endif // TERMIOS_H
