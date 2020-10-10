/*
 * Copyright 2009-2020 Srijan Kumar Sharma
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
#include <arch/x86_64/multiboot2.h>
#include <arch/x86_64/multiboot.h>
#include <arch/x86_64/paging.h>
#include <arch/x86_64/video.h>
#include <arch/x86_64/font.h>
#include <stdio.h>
#include <kernel/config.h>
#include <xmmintrin.h>
#include <tmmintrin.h>

static uint8_t *videomemory = nullptr;
static size_t screen_width = 0, screen_height = 0;
static size_t pitch = 0, depth = 0;
static uint16_t color = 0;
size_t font_width = 0, font_height = 0, font_depth = 0, text_width = 0, text_height = 0;
DisplayMode currentDisplayMode;
//  print functions
void (*putcharacter)(const uint8_t ch, uint32_t x, uint32_t y) = nullptr;
void (*scroll)(void) = nullptr;
void (*setColor)(uint8_t c) = nullptr;
int set_io_functions(void (*putcharacter_new)(const uint8_t ch, uint32_t x, uint32_t y),
                     void (*scroll_new)(void),
                     void (*setColor_new)(uint8_t c))
{
    putcharacter = putcharacter_new;
    scroll = scroll_new;
    setColor = setColor_new;
    return 0;
}
__uint128_t *get_framebuffer()
{
    return (__uint128_t *)videomemory;
}

void get_screenparams(size_t *_screen_width, size_t *_screen_height, size_t *_pitch, size_t *_depth)
{
    *_screen_width = screen_width;
    *_screen_height = screen_height;
    *_pitch = pitch;
    *_depth = depth;
}

//  --------------------------------------------------------------------------------
//  text only functions
//  --------------------------------------------------------------------------------
void putcharacter_text(const uint8_t ch, uint32_t x, uint32_t y)
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

void putcharacter_vga(const uint8_t ch, uint32_t x, uint32_t y)
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
//  VGA functions
//  --------------------------------------------------------------------------------

void init_font()
{
    font_width = 8;
    font_height = 16;
    font_depth = depth;
    text_width = screen_width / font_width;
    text_height = screen_height / font_height;
}

int init_video(multiboot_tag_framebuffer *mbi)
{
    int ret = 0;
    extern uint64_t PAGE_DIRECTORY;
    extern uint64_t DIR_OFFSET;
    videomemory = (uint8_t *)mbi->common.framebuffer_addr;
    screen_width = mbi->common.framebuffer_width;
    screen_height = mbi->common.framebuffer_height;
    pitch = mbi->common.framebuffer_pitch;
    depth = mbi->common.framebuffer_bpp / 8;
    color = 0x0F00;
    //  Paging is not initialized yet, so use 1GB temp paging structure.
    uint64_t *pml4t = (uint64_t *)PAGE_DIRECTORY;
    if ((pml4t[(mbi->common.framebuffer_addr >> 39) & 0x1FF] & 3) != 3)
    {
        //  page structure doesn't exist. Can't continue.
        asm("cli;hlt");
    }
    uint64_t *pdpt = (uint64_t *)(pml4t[(mbi->common.framebuffer_addr >> 39) & 0x1FF] & 0xFFFFFFFFFFFFF000);
    if ((pdpt[(mbi->common.framebuffer_addr >> 30) & 0x1FF] & 3) != 3)
    {
        pdpt[(mbi->common.framebuffer_addr >> 30) & 0x1FF] = (PAGE_DIRECTORY + DIR_OFFSET) | 0x3;
        DIR_OFFSET += 0x1000;
    }
    uint64_t *pte = (uint64_t *)(pdpt[(mbi->common.framebuffer_addr >> 30) & 0x1FF] & 0xFFFFFFFFFFFFF000);
    for (uint64_t addr = mbi->common.framebuffer_addr; addr < (mbi->common.framebuffer_addr + mbi->common.size); addr += 0x200000)
    {
        if ((pte[(mbi->common.framebuffer_addr >> 21) & 0x1FF] & 3) == 3)
        {
            //  already mapped. Check if mapping is same as what we need, else can't continue.
            if ((mbi->common.framebuffer_addr & 0xFFFFFFFFFFF00000) != (pte[(mbi->common.framebuffer_addr >> 21) & 0x1FF] & 0xFFFFFFFFFFF00000))
                asm("cli;hlt");
        }
        else
            pte[(mbi->common.framebuffer_addr >> 21) & 0x1FF] = (mbi->common.framebuffer_addr & 0xFFFFFFFFFFF00000) | 0x83;
    }
    //  flush page cache
    asm volatile("invlpg (%0)" ::"r"(mbi->common.framebuffer_addr));
    //  keep record to read pages later.
    ret = PageManager::add_early_kernel_mapping(mbi->common.framebuffer_addr, mbi->common.framebuffer_addr, pitch * screen_height);
    if (ret < 0)
    {
        printf("failed to add_early_kernel_mapping\n");
        asm("cli;hlt");
    }
    if (mbi->common.framebuffer_type == MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT)
    {
        currentDisplayMode = TEXT_ONLY;
        putcharacter = putcharacter_text;
        scroll = scroll_text;
        setColor = setColor_text;
        text_width = screen_width;
        text_height = screen_height;
    }
    else if (mbi->common.framebuffer_type == MULTIBOOT_FRAMEBUFFER_TYPE_RGB)
    {
        currentDisplayMode = GRAPHICS;
        init_font();
        putcharacter = putcharacter_vga;
        scroll = scroll_vga;
        setColor = setColor_vga;
    }
    return ret;
}
