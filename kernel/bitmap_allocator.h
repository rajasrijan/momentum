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
#pragma once

template <uint64_t bitmap_size, uint64_t block_size>
struct bitmap_allocator
{
    uint64_t paddr, vaddr;
    uint64_t bitmap[bitmap_size / 64];
};

template <uint64_t bitmap_size, uint64_t block_size>
static inline int bitmap_alloc(bitmap_allocator<bitmap_size, block_size> &allocator, uint64_t &paddr, uint64_t &vaddr, size_t count = 1)
{
    size_t count_so_far = 0;
    uint64_t i_start, j_start;
    for (size_t i = 0; i < (sizeof(allocator.bitmap) / sizeof(allocator.bitmap[0])); i++)
    {
        if (allocator.bitmap[i] == 0xFFFFFFFFFFFFFFFF)
            continue;
        for (size_t j = 0; j < 64; j++)
        {
            if (!((1ull << j) & allocator.bitmap[i]))
            {
                if (count_so_far == 0)
                {
                    i_start = i;
                    j_start = j;
                }
                count_so_far++;
                if (count_so_far == count)
                {
                    auto index = (i_start * 64) + j_start;
                    paddr = allocator.paddr + (index * block_size);
                    vaddr = allocator.vaddr + (index * block_size);
                    while (count_so_far)
                    {
                        allocator.bitmap[i_start] |= (1ull << j_start);
                        j_start++;
                        if (j_start >= 64)
                        {
                            j_start = 0;
                            i_start++;
                        }
                        count_so_far--;
                    }
                    return 0;
                }
            }
            else
            {
                count_so_far = 0;
            }
        }
    }
    return -ENOMEM;
}

template <uint64_t bitmap_size, uint64_t block_size>
static inline int bitmap_isfree(bitmap_allocator<bitmap_size, block_size> &allocator, uint64_t &paddr, uint64_t &vaddr)
{
    const uint64_t offset = paddr - allocator.paddr;
    const uint64_t index = offset / block_size;
    return !(allocator.bitmap[index / 64] & (~(1ull << (index % 64))));
}

template <uint64_t bitmap_size, uint64_t block_size>
static inline int bitmap_free(bitmap_allocator<bitmap_size, block_size> &allocator, uint64_t paddr, size_t count = 1)
{
    const uint64_t offset = paddr - allocator.paddr;
    const uint64_t index = offset / block_size;
    auto i = index / 64;
    auto j = index % 64;

    while (count)
    {
        allocator.bitmap[i] &= (~(1ull << j));
        j++;
        if (j >= 64)
        {
            j = 0;
            i++;
        }
        count--;
    }
    return 0;
}

template <uint64_t bitmap_size, uint64_t block_size>
static inline int bitmap_setall(bitmap_allocator<bitmap_size, block_size> &allocator)
{
    for (auto &v : allocator.bitmap)
        v = 0xFFFFFFFFFFFFFFFF;
    return 0;
}

template <uint64_t bitmap_size, uint64_t block_size>
static inline int bitmap_clearall(bitmap_allocator<bitmap_size, block_size> &allocator)
{
    for (auto &v : allocator.bitmap)
        v = 0;
    return 0;
}

template <uint64_t bitmap_size, uint64_t block_size>
static inline int bitmap_set(bitmap_allocator<bitmap_size, block_size> &allocator, uint64_t paddr, size_t count = 1)
{
    const uint64_t offset = paddr - allocator.paddr;
    const uint64_t index = offset / block_size;
    auto i = index / 64;
    auto j = index % 64;

    while (count)
    {
        allocator.bitmap[i] |= (1ull << j);
        j++;
        if (j >= 64)
        {
            j = 0;
            i++;
        }
        count--;
    }
    return 0;
}

template <uint64_t bitmap_size, uint64_t block_size>
static inline int bitmap_get_vaddr(bitmap_allocator<bitmap_size, block_size> &allocator, uint64_t paddr, uint64_t &vaddr)
{
    vaddr = (paddr - allocator.paddr) + allocator.vaddr;
    return 0;
}
