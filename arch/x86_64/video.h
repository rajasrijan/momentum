/*
 * Copyright 2009-2019 Srijan Kumar Sharma
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

#pragma once

enum DisplayMode
{
    TEXT_ONLY,
    GRAPHICS
};
extern size_t text_width, text_height;
extern void (*putcharacter)(const char ch, uint32_t x, uint32_t y);
extern void (*scroll)(void);
extern void (*setColor)(uint8_t c);
void init_video(struct multiboot_tag_framebuffer *mbi);