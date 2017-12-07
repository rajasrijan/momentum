/*
 * Copyright 2009-2017 Srijan Kumar Sharma
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
#ifndef STRING_H
#define STRING_H

#include "algorithm.h"
#include <stdio.h>

namespace std
{

class string
{
    char* data;
    size_t count;
    size_t _size;
public:

    string()
    {
        _size = 32;
        count = 0;
        data = new char[_size];
    }

    string(const char* str)
    {
        if (str != 0)
        {
            for (count = 0; str[count] != 0; count++);
            _size = count + 32;
            data = new char[_size];
            copy(str, &str[count], data);
            data[count] = 0;
        }
        else
        {
            _size = 32;
            count = 0;
            data = new char[_size];
        }
    }

    string(const char* str, size_t n)
    {
        if (str != 0)
        {
            for (count = 0; str[count] != 0; count++);
            count = (n < count) ? n : count;
            _size = count + 32;
            data = new char[_size];
            copy(str, &str[count], data);
            data[count] = 0;
        }
        else
        {
            _size = 32;
            count = 0;
            data = new char[_size];
        }
    }

    ~string()
    {
        if (data != 0)
            delete data;
        _size = 0;
        data = 0;
    }

    void operator=(const char* str)
    {
        // remove current
        if (data != 0)
            delete data;
        _size = 0;
        data = 0;
        // add
        if (str != 0)
        {
            for (count = 0; str[count] != 0; count++);
            _size = count + 32;
            data = new char[_size];
            copy(str, &str[count], data);
            data[count] = 0;
        }
        else
        {
            _size = 32;
            count = 0;
            data = new char[_size];
        }
    }

    void operator=(string s)
    {
        const char* str = s.c_str();

        // remove current
        if (data != 0)
            delete data;
        _size = 0;
        data = 0;
        // add
        if (str != 0)
        {
            for (count = 0; str[count] != 0; count++);
            _size = count + 32;
            data = new char[_size];
            copy(str, &str[count], data);
            data[count] = 0;
        }
        else
        {
            _size = 32;
            count = 0;
            data = new char[_size];
        }
    }

    size_t size()
    {
        return count;
    }

    const char* c_str()
    {
        return data;
    }

    char& operator[](size_t index)
    {
        if (index < count)
        {
            return data[index];
        }
        return data[count];
    }

    bool operator==(string str2)
    {
        for (int i = 0; data[i] != 0 && str2[i] != 0; i++)
        {
            if (data[i] != str2[i])
                return false;
        }
        //printf("\nstring equal\n");
        return true;
    }

    void operator+=(char c)
    {

    }
protected:

    int add_bytes(unsigned int len)
    {
    }
};

//  Number to string

inline string to_string(unsigned val)
{
    char number[256] = {0};
    int len = 0;
    while (val)
    {
        number[len++] = (char) ((unsigned) '0' + (val % 10));
        val /= 10;
    }

    for (int i = 0; i < (len / 2); i++)
    {
        char t = number[i];
        number[i] = number[len - i];
        number[len - i] = t;
    }

    return string(number);
}
}

#endif /* STRING_H */

