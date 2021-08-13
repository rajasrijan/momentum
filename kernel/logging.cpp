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

#include "logging.h"
#include <stdarg.h>
#include <stdio.h>
#include <arch/video.h>

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
            setColor(0xFFFF0000);
        else if (log_lvl == WARNING)
            setColor(0xFFFFFF00);
        vprintf(format, va);
        if (log_lvl < INFO)
            setColor(0xFFFFFFFF);
        va_end(va);
    }
    return ret;
}
