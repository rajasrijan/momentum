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

#ifndef _GUI_H_
#define _GUI_H_
#include <stdint.h>
#include <stddef.h>

int bitblit(__uint128_t *dst, size_t dst_pitch, size_t dst_height, __uint128_t *src, size_t src_pitch, size_t src_height);
int cleartexture(__uint128_t *dst, uint32_t clear_color, size_t p, size_t height);

int initilize_graphics();

class texture
{
  public:
    __uint128_t *data;
    [[maybe_unused]] size_t screen_width, screen_height;
    [[maybe_unused]] size_t pitch, depth;

  public:
    texture(size_t sw, size_t sh, size_t p, size_t d);
    ~texture();
};

class window
{
    friend class window_manager;
    size_t x, y;

  private:
    texture fb;
    window(size_t _x, size_t _y, size_t width, size_t height)
        : x(_x), y(_y), fb(width, height, width * 4, 4)
    {
    }

  public:
    ~window()
    {
    }
    int draw()
    {
        cleartexture(fb.data, 0x00AAAAAA, fb.pitch, 30);
        cleartexture(fb.data + ((30 * fb.pitch) / sizeof(__uint128_t)), 0x00FFFFFF, fb.pitch, fb.screen_height - 30);
        return 0;
    }
    int show()
    {
        return 0;
    }
};

class window_manager
{
  private:
    std::vector<std::shared_ptr<window>> window_list;

  public:
    window_manager()
    {
    }
    ~window_manager()
    {
    }

    template <typename... Args>
    std::shared_ptr<window> create_window(Args... arg)
    {
        auto tmp = std::shared_ptr(new window(arg...));
        window_list.push_back(tmp);
        return tmp;
    }

    std::vector<std::shared_ptr<window>> &get_window_list()
    {
        return window_list;
    }

    void draw_all(__uint128_t *framebuffer, size_t screen_height, size_t pitch)
    {
        for (auto current_window : window_list) {
            auto &fb_ex = current_window->fb;
            auto blit_start_point = (__uint128_t *)(((uint64_t)framebuffer) + (current_window->y * pitch) + current_window->x);
            current_window->draw();
            bitblit(blit_start_point, pitch, screen_height, fb_ex.data, fb_ex.pitch, fb_ex.screen_height);
        }
    }
};

#endif
