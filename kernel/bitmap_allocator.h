/*
 * Copyright 2009-2020 Srijan Kumar Sharma
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
static inline int bitmap_alloc(bitmap_allocator<bitmap_size, block_size> &allocator, uint64_t &paddr, uint64_t &vaddr)
{
    for (size_t i = 0; i < (sizeof(allocator.bitmap) / sizeof(allocator.bitmap[0])); i++)
    {
        if (allocator.bitmap[i] == 0xFFFFFFFFFFFFFFFF)
            continue;
        auto ctz = __builtin_ctz(~(allocator.bitmap[i]));
        allocator.bitmap[i] |= (1 << ctz);
        auto index = (i * 64) + ctz;
        paddr = allocator.paddr + (index * block_size);
        vaddr = allocator.vaddr + (index * block_size);
        return 0;
    }
    return -ENOMEM;
}

template <uint64_t bitmap_size, uint64_t block_size>
static inline int bitmap_free(bitmap_allocator<bitmap_size, block_size> &allocator, uint64_t &paddr)
{
    const uint64_t offset = paddr - allocator.paddr;
    const uint64_t index = offset / block_size;
    allocator.bitmap[index / 64] &= (~(1ull << (index % 64)));
    return 0;
}