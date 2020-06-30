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
#ifndef PARTITION_TABLE_H
#define PARTITION_TABLE_H

#include <stdint.h>

struct Partition
{
    uint8_t status;
    uint8_t legacy1[3];
    uint8_t partitiotype;
    uint8_t legacy2[3];
    uint32_t startLBA;
    uint32_t count;
} __attribute__((packed));

struct PartitionTable
{
    char boot_code[446];
    Partition partition[4];
    uint16_t boot_signature; // 0x55AA
} __attribute__((packed));

#endif /* PARTITION_TABLE_H */
