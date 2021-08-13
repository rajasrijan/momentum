#include <stdint.h>
#include <stddef.h>
#include <arch/video.h>

print_string printString = nullptr;
scroll scrollLine = nullptr;
set_color setColor = nullptr;

int set_io_functions(print_string print_string_new, scroll scroll_new, set_color setColor_new)
{
    printString = print_string_new;
    scrollLine = scroll_new;
    setColor = setColor_new;
    return 0;
}