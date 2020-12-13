#include "logging.h"
#include <stdarg.h>
#include <stdio.h>
#include <arch/x86_64/video.h>

int current_log_level = ALL;

void set_log_lvl(int log_lvl)
{
    if (log_lvl < ERROR)
    {
        current_log_level = ERROR;
    }
    else if (log_lvl > ALL)
    {
        current_log_level = ALL;
    }
    else
    {
        current_log_level = log_lvl;
    }
    log_info("Logging level changed to [%d]\n", current_log_level);
}

int log(const int log_lvl, const char *format, ...)
{
    int ret = 0;
    if (log_lvl <= current_log_level)
    {
        va_list va;
        va_start(va, format);
        if (log_lvl == ERROR)
            setColor(0x4F);
        vprintf(format, va);
        if (log_lvl == ERROR)
            setColor(0x0F);
        va_end(va);
    }
    return ret;
}
