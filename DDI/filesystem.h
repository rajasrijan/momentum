/*
 * Copyright 2009-2018 Srijan Kumar Sharma
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
#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#include <string>
#include "../kernel/vfs.h"
#include "../kernel/vfsops.h"

class filesystem
{
public:
    filesystem();
    filesystem(const filesystem& orig);
    virtual ~filesystem();
    std::string getName();
    virtual bool mount(uint32_t flags, std::string dev_path, vnode*& fs_root_directory) = 0;
    //virtual bool fill_vfs(vfs* v);
protected:
    std::string name;
};

extern std::vector<filesystem*> fs_list;

void register_filesystem(filesystem* fs);
void unregister_filesystem(filesystem* fs);
#endif /* FILESYSTEM_H */

