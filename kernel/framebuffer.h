#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include <stdint.h>

extern size_t console_width;
extern size_t console_height;

struct framebuffer
{
    uint8_t *framebuffer_base;
    size_t framebuffer_size;
    size_t horizontal_sz;
    size_t vertical_sz;
    size_t pitch;
    size_t depth;
};
static_assert(sizeof(framebuffer) == 0x30, "Invalid structure size!");

framebuffer &get_framebuffer();
void print_string_fb(const char *ch);
void scroll_fb(void);
void setColor_fb(uint32_t c);
int blit(const void *src_ptr, const size_t src_pitch, const size_t src_x0, const size_t src_y0, const size_t src_x1, const size_t src_y1, void *dst_ptr,
         const size_t dst_pitch, const size_t dst_x0, const size_t dst_y0, const size_t depth);
#endif //  FRAMEBUFFER_H
