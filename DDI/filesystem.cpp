/*
 * Copyright 2009-2019 Srijan Kumar Sharma
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
#include "filesystem.h"
#include <vector>

filesystem::filesystem()
{
}

filesystem::filesystem(const filesystem& orig)
{
}

filesystem::~filesystem()
{
}

std::string filesystem::getName()
{
    return name;
}

std::vector<filesystem*> fs_list;

void register_filesystem(filesystem* fs)
{
    fs_list.push_back(fs);
}

void unregister_filesystem(filesystem* fs)
{
    //fs_list.remove(fs);
}
