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

#include "vector.h"
#include <ctype.h>
#include <string>

std::string operator+(const char *first, std::string second)
{
    size_t new_length = strlen(first) + second.size();
    char *new_string = new char[new_length + 1];
    strcpy(new_string, first);
    strcat(new_string, second.c_str());
    std::string ret = new_string;
    delete[] new_string;
    return ret;
}