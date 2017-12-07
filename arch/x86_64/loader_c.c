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

#include "multiboot.h"

void readMultiBootInfo(struct multiboot_info* mbi) __attribute__((section(".text0")));
void readMultiBootInfo(struct multiboot_info* mbi)
{
	uint16_t *fb = 0xB800;
	for (int i = 0; i < 10; i++)
	{
		fb[i] = 0x1F3F;
	}
}