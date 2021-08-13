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

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <xmmintrin.h>
#include <tmmintrin.h>
#include <arch/video.h>
#include <kernel/config.h>
#include <kernel/framebuffer.h>
#include <kernel/font.h>
#include <arch/x86_64/video.h>
#include <arch/paging.h>

extern framebuffer fb;
//  print functions
void get_screenparams(size_t *_screen_width, size_t *_screen_height, size_t *_pitch, size_t *_depth)
{
    *_screen_width = fb.horizontal_sz;
    *_screen_height = fb.vertical_sz;
    *_pitch = fb.pitch;
    *_depth = fb.depth;
}

int init_video()
{
    int ret = -EUNK;
    ret = PageManager::setVirtualToPhysicalMemory((uint64_t)fb.framebuffer_base, (uint64_t)fb.framebuffer_base,
                                                  PageManager::round_up_to_pagesize(fb.framebuffer_size), PageManager::Privilege::Supervisor,
                                                  PageManager::PageType::Read_Write);
    memset(fb.framebuffer_base, 0, fb.framebuffer_size);
    console_height = fb.vertical_sz / 16;
    console_width = fb.horizontal_sz / 8;
    set_io_functions(print_string_fb, scroll_fb, setColor_fb);
    printf("Switched to software frame buffer rendering!\n");
    return ret;
}
