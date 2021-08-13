#ifndef VIDEO_COMMON_H
#define VIDEO_COMMON_H

#ifdef _arch_x86_64_
#include <arch/x86_64/video.h>
#endif

#ifdef _arch_efi_
#include <arch/efi/video.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

    typedef void (*print_string)(const char *ch);
    typedef void (*scroll)(void);
    typedef void (*set_color)(uint32_t c);

    extern print_string printString;
    extern scroll scrollLine;
    extern set_color setColor;

    int set_io_functions(print_string print_string_new, scroll scroll_new, set_color setColor_new);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // VIDEO_COMMON_H