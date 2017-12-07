/*
 * Copyright 2009-2017 Srijan Kumar Sharma
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

#ifndef KEYBOARD_H
#define	KEYBOARD_H

#ifdef	__cplusplus
extern "C"
{
#endif

enum keymap
{
    SC_ESC=0x01,
    SC_1,
    SC_2,
    SC_3,
    SC_4,
    SC_5,
    SC_6,
    SC_7,
    SC_8,
    SC_9,
    SC_A,
    SC_B,
    SC_C,
    SC_D,
    SC_E,
    SC_LSHIFT=0x2A,
    SC_RSHIFT=0x36
};
void init_keyboard(void);



#ifdef	__cplusplus
}
#endif

#endif	/* KEYBOARD_H */

