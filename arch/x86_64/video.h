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

#pragma once
#include <stdint.h>
#include "multiboot2.h"

enum DisplayMode {
    TEXT_ONLY,
    GRAPHICS
};
extern size_t text_width, text_height;
extern void (*putcharacter)(const uint8_t ch, uint32_t x, uint32_t y);
extern void (*scroll)(void);
extern void (*setColor)(uint8_t c);
int init_video(struct multiboot_tag_framebuffer *mbi);
int set_io_functions(void (*putcharacter_new)(const uint8_t ch, uint32_t x, uint32_t y),
                     void (*scroll_new)(void),
                     void (*setColor_new)(uint8_t c));
__uint128_t *get_framebuffer();
void get_screenparams(size_t *_screen_width, size_t *_screen_height, size_t *_pitch, size_t *_depth);