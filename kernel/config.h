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

/*  
 *  GENERATED FILE. DO NOT MODIFY HERE
 *  MODIFY config.h.in
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#ifndef KERNEL_BASE_PADDR_PTR
#define KERNEL_BASE_PADDR_PTR (0x0000000000100000)
#endif // KERNEL_BASE_PADDR_PTR

#ifndef KERNEL_BASE_VADDR_PTR
#define KERNEL_BASE_VADDR_PTR (0x000000C000000000)
#endif // KERNEL_BASE_VADDR_PTR

#ifndef KERNEL_PAGE_DIRECTORY_VADDR_PTR
#define KERNEL_PAGE_DIRECTORY_VADDR_PTR (0x000000D000000000)
#endif // KERNEL_PAGE_DIRECTORY_VADDR_PTR

#if KERNEL_BASE_VADDR_PTR < 0x100000
#error Kernel has to be above 1MB in RAM.
#endif

#if (KERNEL_BASE_VADDR_PTR & 0xFFFF000000000000) != 0
#error kernel base pointer cannot be beyond mappable region. Change the value of KERNEL_BASE_VADDR_PTR.
#endif

#if KERNEL_BASE_VADDR_PTR < 0x100000000
#warning placing kernel lower than 4GB mark can lead to collision with memory mapped IO.
#endif

#ifndef KERNEL_HEAP_PTR
#define KERNEL_HEAP_PTR (0x00000000D0000000)
#endif // KERNEL_HEAP_PTR

#ifndef KERNEL_MAXIMUM_PHYSICAL_RAM
//  64GB
#define KERNEL_MAXIMUM_PHYSICAL_RAM (0x0000001000000000)
#endif // KERNEL_MAXIMUM_PHYSICAL_RAM

#ifndef KERNEL_MINIMUM_PHYSICAL_RAM
//  128MB
#define KERNEL_MINIMUM_PHYSICAL_RAM (0x0000000008000000)
#endif // KERNEL_MINIMUM_PHYSICAL_RAM

#ifndef KERNEL_TEMP_PAGE_TABLE_LOCATION
//  At 64MB.
//  Bug: collision risk with memory mapped io.
#define KERNEL_TEMP_PAGE_TABLE_LOCATION (0x0000000004000000)
#endif // KERNEL_TEMP_PAGE_TABLE_LOCATION

#ifndef KERNEL_ENABLE_MULTITASKING
#ifdef _arch_x86_64_
#define KERNEL_ENABLE_MULTITASKING 1
#elif _arch_efi_
#define KERNEL_ENABLE_MULTITASKING 0
#else
#error Unknown architexture
#endif
#endif // KERNEL_ENABLE_MULTITASKING

#ifndef KERNEL_GRUB_VIDEO
#define KERNEL_GRUB_VIDEO (1)
#endif

#ifndef KERNEL_GRUB_VIDEO
#define KERNEL_GRUB_VIDEO (1)
#endif
#define MOMENTUM_KERNEL_VGA 1
#endif //_CONFIG_H_
