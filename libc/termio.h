#if __STDC_HOSTED__ == 1
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
#define VDISCARD 16
#define VDSUSP 17
#define VEOF 18
#define VEOL 19
#define VEOL2 20
#define VERASE 21
#define VINTR 22
#define VKILL 23
#define VLNEXT 24
#define VMIN 25
#define VQUIT 26
#define VREPRINT 27
#define VSTART 28
#define VSTATUS 29
#define VSTOP 30
#define VSUSP 31
#define VSWTCH 32
#define VTIME 33
#define VWERASE 34

struct termio
{
    uint64_t c_lflag;
    uint64_t c_cflag;
    uint64_t c_iflag;
    uint64_t c_oflag;
    uint64_t c_cc[256];
};

int tcgetattr(int fd, struct termio *termios_p);
#endif //TERMIOS_H
#endif