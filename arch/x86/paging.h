/*
 * Copyright 2009-2011 Srijan Kumar Sharma
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

#ifndef PAGING_H
#define	PAGING_H
#include <stdint.h>

struct paging_structure
{
    uint32_t page_directory[1024];
    uint32_t page_table[1024 * 1024];
};

void map_4mb(uint32_t virtual_address);
typedef struct paging_structure paging_structure_t;
void new_paging_structure(paging_structure_t* ps);
void init_paging(void);
void identity_map_4mb(uint32_t address);
void force_map(uint32_t physical, uint32_t virtual, uint32_t pages);
uint32_t get_physical_address(uint32_t virtual);

#endif	/* PAGING_H */

