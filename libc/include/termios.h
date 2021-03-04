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


#include <stdint.h>
#include <sys/types.h>

#ifndef TERMIOS_H
#define TERMIOS_H

typedef uint64_t cc_t;     //        Used for terminal special characters.
typedef uint64_t speed_t;  //        Used for terminal baud rates.
typedef uint64_t tcflag_t; //        Used for terminal modes.
                           //        Size of the array c_cc for control characters.
#define NCCS 2
struct termios {

    tcflag_t c_iflag; //     Input modes.
    tcflag_t c_oflag; //     Output modes.
    tcflag_t c_cflag; //     Control modes.
    tcflag_t c_lflag; //     Local modes.
    cc_t c_cc[NCCS];  //  Control characters.
};

//    The <termios.h> header shall define the following symbolic constants for use as flags in the c_iflag field. The c_iflag field describes the basic terminal input control.

//	Signal interrupt on break.
#define BRKINT (1 << 0)
//	Map CR to NL on input.
#define ICRNL (1 << 1)
//	Ignore break condition.
#define IGNBRK (1 << 2)
//	Ignore CR.
#define IGNCR (1 << 3)
//	Ignore characters with parity errors.
#define IGNPAR (1 << 4)
//	Map NL to CR on input.
#define INLCR (1 << 5)
//	Enable input parity check.
#define INPCK (1 << 6)
//	Strip character.
#define ISTRIP (1 << 7)
//	Enable any character to restart output.
#define IXANY (1 << 8)
//	Enable start/stop input control.
#define IXOFF (1 << 9)
//	Enable start/stop output control.
#define IXON (1 << 10)
//	Mark parity errors.
#define PARMRK (1 << 11)

// Output Modes

// The <termios.h> header shall define the following symbolic constants for use as flags in the c_oflag field. The c_oflag field specifies the system treatment of output.

//	Post-process output.
#define OPOST (1 << 0)
//	Map NL to CR-NL on output.
#define ONLCR (1 << 1)
//	Map CR to NL on output.
#define OCRNL (1 << 2)
//	No CR output at column 0.
#define ONOCR (1 << 3)
//	NL performs CR function.
#define ONLRET (1 << 4)
//	Fill is DEL.
#define OFDEL (1 << 5)
//	Use fill characters for delay.
#define OFILL (1 << 6)
//	Select newline delays:
//#define NLDLY (1<<7)

//	Newline type 0.
#define NL0 (1 << 7)
//	Newline type 1.
#define NL1 (1 << 8)

//	Select carriage-return delays:
//#define CRDLY (1<<9)

//	Carriage-return delay type 0.
#define CR0 (1 << 9)
//	Carriage-return delay type 1.
#define CR1 (1 << 10)
//	Carriage-return delay type 2.
#define CR2 (1 << 11)
//	Carriage-return delay type 3.
#define CR3 (1 << 12)

//	Select horizontal-tab delays:
//#define TABDLY (1<<13)

//	Horizontal-tab delay type 0.
#define TAB0 (1 << 13)
//	Horizontal-tab delay type 1.
#define TAB1 (1 << 14)
//	Horizontal-tab delay type 2.
#define TAB2 (1 << 15)
//	Expand tabs to spaces.
#define TAB3 (1 << 16)

//	Select backspace delays:
//#define BSDLY (1<<17)

//	Backspace-delay type 0.
#define BS0 (1 << 18)
//	Backspace-delay type 1.
#define BS1 (1 << 19)

//	Select vertical-tab delays:
//#define VTDLY (1<<0)

//	Vertical-tab delay type 0.
#define VT0 (1 << 20)
//	Vertical-tab delay type 1.
#define VT1 (1 << 21)

//	Select form-feed delays:
//#define FFDLY (1 << 0)

//	Form-feed delay type 0.
#define FF0 (1 << 22)
//	Form-feed delay type 1.
#define FF1 (1 << 23)

// Baud Rate Selection

// The <termios.h> header shall define the following symbolic constants for use as values of objects of type speed_t.

// The input and output baud rates are stored in the termios structure. These are the valid values for objects of type speed_t. Not all baud rates need be supported by the underlying hardware.

//	Hang up
#define B0 (1 << 0)
//	50 baud
#define B50 (1 << 1)
//	75 baud
#define B75 (1 << 2)
//	110 baud
#define B110 (1 << 3)
//	134.5 baud
#define B134 (1 << 4)
//	150 baud
#define B150 (1 << 5)
//	200 baud
#define B200 (1 << 6)
//	300 baud
#define B300 (1 << 7)
//	600 baud
#define B600 (1 << 8)
//	1200 baud
#define B1200 (1 << 9)
//	1800 baud
#define B1800 (1 << 10)
//	2400 baud
#define B2400 (1 << 11)
//	4800 baud
#define B4800 (1 << 12)
//	9600 baud
#define B9600 (1 << 13)
//	19200 baud
#define B19200 (1 << 14)
//	38400 baud
#define B38400 (1 << 15)

// Control Modes

// The <termios.h> header shall define the following symbolic constants for use as flags in the c_cflag field. The c_cflag field describes the hardware control of the terminal; not all values specified are required to be supported by the underlying hardware.

//	Character size:
//#define CSIZE

//	5 bits
#define CS5 (1 << 0)
//	6 bits
#define CS6 (1 << 1)
//	7 bits
#define CS7 (1 << 2)
//	8 bits
#define CS8 (1 << 3)

//	Send two stop bits, else one.
#define CSTOPB (1 << 4)
//	Enable receiver.
#define CREAD (1 << 5)
//	Parity enable.
#define PARENB (1 << 6)
//	Odd parity, else even.
#define PARODD (1 << 7)
//	Hang up on last close.
#define HUPCL (1 << 8)
//	Ignore modem status lines.
#define CLOCAL (1 << 9)

// The implementation shall support the functionality associated with the symbols CS7, CS8, CSTOPB, PARODD, and PARENB.
// Local Modes

// The <termios.h> header shall define the following symbolic constants for use as flags in the c_lflag field. The c_lflag field of the argument structure is used to control various terminal functions.

//	Enable echo.
#define ECHO (1 << 0)
//	Echo erase character as error-correcting backspace.
#define ECHOE (1 << 1)
//	Echo KILL.
#define ECHOK (1 << 2)
//	Echo NL.
#define ECHONL (1 << 3)
//	Canonical input (erase and kill processing).
#define ICANON (1 << 4)
//	Enable extended input character processing.
#define IEXTEN (1 << 5)
//	Enable signals.
#define ISIG (1 << 6)
//	Disable flush after interrupt or quit.
#define NOFLSH (1 << 7)
//	Send SIGTTOU for background output.
#define TOSTOP (1 << 8)

// Attribute Selection

// The <termios.h> header shall define the following symbolic constants for use with tcsetattr():

//	Change attributes immediately.
#define TCSANOW (1 << 0)
//	Change attributes when output has drained.
#define TCSADRAIN (1 << 1)
//	Change attributes when output has drained; also flush pending input.
#define TCSAFLUSH (1 << 2)

// Line Control

// The <termios.h> header shall define the following symbolic constants for use with tcflush():

//	Flush pending input.
#define TCIFLUSH (1 << 0)
//	Flush both pending input and untransmitted output.
#define TCIOFLUSH (1 << 1)
//	Flush untransmitted output.
#define TCOFLUSH (1 << 2)

// The <termios.h> header shall define the following symbolic constants for use with tcflow():

//	Transmit a STOP character, intended to suspend input data.
#define TCIOFF (1 << 0)
//	Transmit a START character, intended to restart input data.
#define TCION (1 << 1)
//	Suspend output.
#define TCOOFF (1 << 2)
//	Restart output.
#define TCOON (1 << 3)

// The <termios.h> header shall define the pid_t type as described in <sys/types.h>.

// The following shall be declared as functions and may also be defined as macros. Function prototypes shall be provided.

speed_t cfgetispeed(const struct termios *);
speed_t cfgetospeed(const struct termios *);
int cfsetispeed(struct termios *, speed_t);
int cfsetospeed(struct termios *, speed_t);
int tcdrain(int);
int tcflow(int, int);
int tcflush(int, int);
int tcgetattr(int, struct termios *);
pid_t tcgetsid(int);
int tcsendbreak(int, int);
int tcsetattr(int, int, const struct termios *);

#endif //TERMIOS_H