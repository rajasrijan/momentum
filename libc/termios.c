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


#include <termios.h>
#include <errno.h>

#if __STDC_HOSTED__ == 1
speed_t cfgetispeed(const struct termios *termios_ptr)
{
    errno = ENOSYS;
    return 0;
}
speed_t cfgetospeed(const struct termios *termios_ptr)
{
    errno = ENOSYS;
    return 0;
}
int cfsetispeed(struct termios *termios_ptr, speed_t speed)
{
    return -ENOSYS;
}
int cfsetospeed(struct termios *termios_ptr, speed_t speed)
{
    return -ENOSYS;
}
int tcdrain(int a)
{
    return -ENOSYS;
}
int tcflow(int a, int b)
{
    return -ENOSYS;
}
int tcflush(int a, int b)
{
    return -ENOSYS;
}
int tcgetattr(int a, struct termios *termios_ptr)
{
    return -ENOSYS;
}
pid_t tcgetsid(int a)
{
    errno = ENOSYS;
    return 0;
}
int tcsendbreak(int a, int b)
{
    return -ENOSYS;
}
int tcsetattr(int a, int b, const struct termios *termios_ptr)
{
    return -ENOSYS;
}
#endif //   __STDC_HOSTED__ == 1