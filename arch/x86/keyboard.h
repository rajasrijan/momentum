/* 
 * File:   keyboard.h
 * Author: Srijan
 *
 * Created on July 19, 2011, 9:42 PM
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
    SC_E
};
void init_keyboard(void);



#ifdef	__cplusplus
}
#endif

#endif	/* KEYBOARD_H */

