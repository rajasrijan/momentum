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

#include <memory>
#include <utility>
#include <vector>
#include <kernel/gui.h>
#include <arch/x86_64/video.h>
#include <stdlib.h>
#include <emmintrin.h>
#include <arch/x86_64/timer.h>
#include <kernel/multitask.h>

using namespace std;

static __uint128_t *framebuffer = nullptr;
static size_t screen_width, screen_height;
static size_t pitch, _depth;
static thread_t graphics_tid = nullptr;
static window_manager *win_manager;

int bitblit(__uint128_t *dst, size_t dst_pitch, size_t dst_height, __uint128_t *src, size_t src_pitch, size_t src_height)
{
    auto height = min(dst_height, src_height);
    dst_pitch /= sizeof(__uint128_t);
    src_pitch /= sizeof(__uint128_t);

    auto p = min(dst_pitch, src_pitch);

    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < p; j++) {
            dst[(i * dst_pitch) + j] = src[(i * src_pitch) + j];
        }
    }
    return 0;
}

int cleartexture(__uint128_t *dst, uint32_t clear_color, size_t p, size_t height)
{
    __uint128_t color = (__int128_t)_mm_set1_epi32((int32_t)clear_color);
    const auto pp = (p / sizeof(__uint128_t));
    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < pp; j++) {
            dst[(i * pp) + j] = color;
        }
    }
    return 0;
}

void *graphics_thread(void *arg)
{
    while (true) {
        sleep(100);
        cleartexture(framebuffer, 0x00000000, pitch, screen_height);
        win_manager->draw_all(framebuffer, screen_height, pitch);
    }

    return nullptr;
}

int initilize_graphics()
{
    set_io_functions(nullptr, nullptr, nullptr);
    framebuffer = get_framebuffer();
    get_screenparams(&screen_width, &screen_height, &pitch, &_depth);
    cleartexture(framebuffer, 0x00000000, pitch, screen_height);
    win_manager = new window_manager;
    auto win = win_manager->create_window(100, 100, 800, 600);
    win->show();

    multitask::getInstance()->createKernelThread(graphics_tid, "graphics-thread", graphics_thread, nullptr);

    return 0;
}

texture::texture(size_t sw, size_t sh, size_t p, size_t d)
    : screen_width(sw), screen_height(sh), pitch(p), depth(d)
{
    data = (__uint128_t *)aligned_malloc(screen_height * pitch, 7);
    cleartexture(data, 0x00000000, pitch, screen_height);
}

texture::~texture()
{
    free(data);
    data = nullptr;
}
