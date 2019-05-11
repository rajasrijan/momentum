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

#include <stdint.h>
#include <string.h>
#include <arch/x86_64/multiboot.h>
#include <arch/x86_64/paging.h>
#include <arch/x86_64/video.h>
#include <arch/x86_64/font.h>
#include <stdio.h>

uint8_t *videomemory = nullptr;
size_t screen_width = 0, screen_height = 0;
size_t pitch = 0, depth = 0;
uint16_t color = 0;
size_t font_width = 0, font_height = 0, font_depth = 0, text_width = 0, text_height = 0;
DisplayMode currentDisplayMode;
//  print functions
void (*putcharacter)(const char ch, uint32_t x, uint32_t y) = nullptr;
void (*scroll)(void) = nullptr;
void (*setColor)(uint8_t c) = nullptr;
//  --------------------------------------------------------------------------------
//  text only functions
//  --------------------------------------------------------------------------------
void putcharacter_text(const char ch, uint32_t x, uint32_t y)
{
    ((uint16_t *)videomemory)[((y * 80) + x)] = (uint16_t)(color | ch);
}

void scroll_text()
{
    memcpy((char *)videomemory, (char *)(videomemory + 160), 80 * 24 * 2);
    memset((char *)(videomemory + (160 * 24)), 0, 80 * 2);
}

void setColor_text(uint8_t c)
{
    color = (uint16_t)(((uint32_t)c) << 8);
}
//  --------------------------------------------------------------------------------
//  VGA functions
//  --------------------------------------------------------------------------------
void putcharacter_vga(const char ch, uint32_t x, uint32_t y)
{
    for (size_t i = 0; i < font_height; i++)
    {
        for (size_t j = 0; j < font_width; j++)
        {
            uint8_t tmp = (vga_font[ch][i] & (0x80 >> j)) ? 0xFF : 0x00;
            for (size_t c = 0; c < depth; c++)
            {
                videomemory[(((y * font_height + i) * pitch) + ((x * font_width + j) * depth)) + c] = tmp;
            }
        }
    }
}

void scroll_vga()
{
    memcpy(videomemory, videomemory + (pitch * font_height), (text_height - 1) * font_height * pitch);
    memset(videomemory + ((text_height - 1) * font_height * pitch), 0, font_height * pitch);
}

void setColor_vga(uint8_t c)
{
    color = (uint16_t)(((uint32_t)c) << 8);
}
//  --------------------------------------------------------------------------------

void init_font()
{
    font_width = 8;
    font_height = 16;
    font_depth = depth;
    text_width = screen_width / font_width;
    text_height = screen_height / font_height;
}

void init_video(multiboot_info *mbi)
{
    videomemory = (uint8_t *)mbi->framebuffer_addr;
    screen_width = mbi->framebuffer_width;
    screen_height = mbi->framebuffer_height;
    pitch = mbi->framebuffer_pitch;
    depth = mbi->framebuffer_bpp / 8;
    color = 0x0F00;
    if (mbi->framebuffer_type == MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT)
    {
        currentDisplayMode = TEXT_ONLY;
        putcharacter = putcharacter_text;
        scroll = scroll_text;
        setColor = setColor_text;
        text_width = screen_width;
        text_height = screen_height;
    }
    else if (mbi->framebuffer_type == MULTIBOOT_FRAMEBUFFER_TYPE_RGB)
    {
        PageManager::getInstance()->setVirtualToPhysicalMemory(mbi->framebuffer_addr, mbi->framebuffer_addr, mbi->framebuffer_width * mbi->framebuffer_height * mbi->framebuffer_bpp / 8, PageManager::Supervisor, PageManager::Read_Write);
        currentDisplayMode = GRAPHICS;
        init_font();
        putcharacter = putcharacter_vga;
        scroll = scroll_vga;
        setColor = setColor_vga;
    }
}
