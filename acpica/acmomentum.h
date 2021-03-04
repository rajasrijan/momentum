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
#ifndef __ACMOMENTUM_H__
#define __ACMOMENTUM_H__

#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <native_sync.h>

#define ACPI_MACHINE_WIDTH 64

#define ACPI_DEBUG_OUTPUT

#define ACPI_MUTEX_TYPE ACPI_OSL_MUTEX

#define ACPI_USE_SYSTEM_CLIBRARY

#define ACPI_MUTEX mtx_t *
#define ACPI_SPINLOCK mtx_t *
#define ACPI_SEMAPHORE sem_t * 

#define ACPI_USE_LOCAL_CACHE

#endif /* __ACMOMENTUM_H__ */
