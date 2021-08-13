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

#include <stdint.h>
#include <DDI/driver.h>
#include <DDI/filesystem.h>
#include "ext_vfs.h"

vfs *ext_new_vfs()
{
    return new ext_vfs;
}
void ext_delete_vfs(vfs *ptr)
{
    delete ptr;
}
fileSystem ext = {ext_new_vfs, ext_delete_vfs};
void ext_init()
{
    printf("Initializing ext\n");
    register_filesystem(ext, "ext");
}
MODULT_INIT(ext_init)