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

#include "standard_output.h"
#include <DDI/driver.h>
#include <limits.h>
#include <kernel/logging.h>

void standard_io_init()
{
    printf("standard_io_init\n");
    char device_name[256] = {};
    //  try 'all' the id numbers
    for (int tty_id = 0; tty_id < INT_MAX; tty_id++)
    {
        snprintf(device_name, sizeof(device_name), "tty%d", tty_id);
        shared_ptr<vnode> std_io = make_shared<standard_output>(device_name);
        if (!mknod("/dev/", std_io))
            break;
    }
}

standard_output::standard_output(const std::string &_name) : vnode(nullptr)
{
    setName(_name.c_str());
    v_type = VCHR;
    log_debug("standard_output()\n");
}
standard_output::~standard_output()
{
    log_debug("~standard_output()\n");
}
int standard_output::bwrite(size_t position, size_t size, const char *data, size_t *bytesWritten)
{
    int write_count = 0;
    for (size_t i = 0; i < size; i++)
    {
        putchar(data[i]);
        write_count++;
    }
    if (bytesWritten)
        *bytesWritten = write_count;
    return 0;
}

MODULT_INIT(standard_io_init)
