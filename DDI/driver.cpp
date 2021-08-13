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
#include <stdint.h>
#include <stdio.h>
#include "driver.h"
#include <kernel/logging.h>

extern pfn_module_init_fn_t pfn_ext_init;

int init_drivers()
{
    uint64_t noOfModules = 0;
#ifdef _arch_x86_64_
    noOfModules = (uint64_t)(&__MOD_END__ - &__MOD_LIST__);
    log_info("No of modules: %x\n", noOfModules);
    for (uint64_t i = 0; i < noOfModules; i++)
    {
        log_info("calling...[%x]\n", (&__MOD_LIST__)[i]);
        pfn_module_init_fn_t constructor_fn = (void (*)(void))(&__MOD_LIST__)[i];
        constructor_fn();
    }
#elif _arch_efi_
    log_info("Initialize drivers\n");
    pfn_ext_init();
#endif
    return 0;
}
