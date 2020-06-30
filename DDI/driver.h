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

#ifndef DRIVER_H
#define DRIVER_H

typedef void (*pfn_module_init_fn_t)(void);

#define JOIN_NAME(x, y) x##y
#define MODULT_INIT(module_init_fn) __attribute__((section("mod_init"))) pfn_module_init_fn_t pfn_##module_init_fn = module_init_fn;

/*
 * Initializes builtin drivers.
 * Implements basic functionality.
 */
void init_drivers(void);

#endif /* DRIVER_H */
