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

#ifndef LOGGING_H
#define LOGGING_H

#define log_error(...) log(ERROR, __VA_ARGS__)
#define log_warning(...) log(WARNING, __VA_ARGS__)
#define log_info(...) log(INFO, __VA_ARGS__)
#define log_debug(...) log(DEBUG, __VA_ARGS__)
#define log_all(...) log(ALL, __VA_ARGS__)

#ifdef __cplusplus
extern "C"
{
#endif //    __cplusplus
    enum log_lvl
    {
        ERROR = 0,
        WARNING,
        INFO,
        DEBUG,
        ALL
    };
    extern int current_log_level;
    void set_log_lvl(int log_lvl);
    int log(const int log_lvl, const char *format, ...);
#ifdef __cplusplus
}
#endif //    __cplusplus
#endif //    LOGGING_H