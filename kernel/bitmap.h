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

#ifndef BITMAP_H
#define BITMAP_H
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <kernel/logging.h>
class bitmap
{
  private:
    size_t bit_count;
    uint64_t *dat;

  public:
    bitmap(size_t count) : bit_count(count), dat(nullptr)
    {
        dat = new uint64_t[(bit_count + 63) / 64];
    }
    ~bitmap()
    {
        delete dat;
    }
    bool isBitSet(size_t index)
    {
        if (index >= bit_count)
        {
            log_error("index out of range\n");
            asm("cli;hlt");
        }
        return dat[index % 64] & (1 << (index % 64));
    }
    int alloc_bit(size_t &index)
    {
        bool found = false;
        for (size_t i = 0; i < bit_count / 64; i++)
        {
            if (dat[i] == 0xFFFFFFFFFFFFFFFF)
                continue;
            int pos = __builtin_ctz(~dat[i]);
            index = (64 * i) + pos;
            found = true;
            break;
        }
        if (found && index < bit_count)
        {
            dat[index / 64] |= (1 << (index % 64));
            return 0;
        }
        index = 0;
        return -ENOMEM;
    }
    uint8_t *get_ptr()
    {
        return (uint8_t *)dat;
    }
};

#endif // BITMAP_H