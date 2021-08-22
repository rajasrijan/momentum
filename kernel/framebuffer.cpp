#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <kernel/font.h>
#include <kernel/framebuffer.h>

framebuffer fb = {};
static size_t x = 0, y = 0;
static uint32_t color = 0xFFFFFFFF;
size_t console_width = 80;
size_t console_height = 24;

framebuffer &get_framebuffer()
{
    return fb;
}

int blit(const void *src_ptr, const size_t src_pitch, const size_t src_x0, const size_t src_y0, const size_t src_x1, const size_t src_y1, void *dst_ptr,
         const size_t dst_pitch, const size_t dst_x0, const size_t dst_y0, const size_t depth)
{
    uint8_t *dst_row = &(((uint8_t *)dst_ptr)[dst_y0 * dst_pitch]);
    const uint8_t *src_row = &((uint8_t *)src_ptr)[(src_y0 * src_pitch)];

    //  copy pixels
    for (size_t y = 0; y < (src_y1 - src_y0); y++)
    {
        auto dst = dst_row + (dst_x0 * depth);
        auto src = src_row + (src_x0 * depth);
        for (size_t x = 0; x < (src_x1 - src_x0); x++)
        {
            for (size_t d = 0; d < depth; d++)
            {
                dst[0] = src[0];
                dst++;
                src++;
            }
        }
        dst_row += dst_pitch;
        src_row += src_pitch;
    }

    return 0;
}
#define canprint(c) ((c >= 0x20) && (c <= 0x7E))
void print_string_fb(const char *ch)
{
    uint32_t pix[8 * 16];
    for (size_t i = 0; ch[i]; i++)
    {
        if (canprint(ch[i]))
        {
            get_font(ch[i], color, pix);
            blit((void *)pix, 4 * 8, 0, 0, 8, 16, fb.framebuffer_base, fb.pitch, x * 8, y * 16, 4);
            x++;
        }
        else if (ch[i] == '\n')
        {
            x = 0;
            y++;
        }
        else if (ch[i] == '\b')
        {
            if (x > 0)
            {
                x--;
                memset(pix, 0, sizeof(pix));
                blit((void *)pix, 4 * 8, 0, 0, 8, 16, fb.framebuffer_base, fb.pitch, x * 8, y * 16, 4);
            }
        }
        else
        {
            asm("cli;hlt");
        }

        if (x >= console_width)
        {
            x = 0;
            y += 1;
        }
        if (y >= console_height)
        {
            scroll_fb();
            y = console_height - 1;
        }
    }
}
void scroll_fb(void)
{
    memcpy(fb.framebuffer_base, fb.framebuffer_base + (fb.pitch * 16), (fb.pitch * 16 * (console_height - 1)));
    memset(fb.framebuffer_base + (fb.pitch * 16 * (console_height - 1)), 0x00, (fb.pitch * 16));
}
void setColor_fb(uint32_t c)
{
    color = c;
}
