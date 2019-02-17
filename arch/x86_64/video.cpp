/*
 * Copyright 2009-2018 Srijan Kumar Sharma
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
#include <string.h>


void scroll(void);


static uint16_t* videomemory = (uint16_t*) 0xb8000;
static uint16_t color = 0x0F00;
void putcharacter(const char ch, uint32_t x, uint32_t y);
void setColor(uint8_t c);


void putcharacter(const char ch, uint32_t x, uint32_t y)
{
    videomemory[((y * 80) + x)] = (uint16_t) (color | ch);
}

void scroll()
{
    memcpy((char*) videomemory, (char*) (videomemory + 80), 80 * 24 * 2);
    memset((char*) (videomemory + (80 * 24)), 0, 80 * 2);
}

void setColor(uint8_t c)
{
    color=(uint16_t)(((uint32_t)c)<<8);
}
