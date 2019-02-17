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
#include "driver.h"
#include <stdint.h>
#include <stdio.h>

extern "C" uint64_t __MOD_LIST__;
extern "C" uint64_t __MOD_END__;

void init_drivers()
{
    uint64_t noOfModules = (uint64_t)(&__MOD_END__ - &__MOD_LIST__);
	printf("No of modules: %x\n", noOfModules);
	for (uint64_t i = 0; i < noOfModules; i++)
	{
		printf("calling...[%x]\n", (&__MOD_LIST__)[i]);
		void (*constructor_fn)(void) = (void (*)(void))(&__MOD_LIST__)[i];
		constructor_fn();
	}
}
