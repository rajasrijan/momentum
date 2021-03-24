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

#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include <kernel/config.h>

#pragma pack(push, 8)
struct kernel_multiboot_header {
    alignas(8) multiboot_header mboot_header;
    alignas(8) multiboot_header_tag_information_request tag_info_req;
    multiboot_uint32_t requests[6];
#if KERNEL_GRUB_VIDEO == 1
    alignas(8) multiboot_header_tag_framebuffer tag_framebuffer;
#endif
    alignas(8) multiboot_header_tag_module_align tag_mod_align;
    alignas(8) multiboot_header_tag tag_end;
};
#pragma pack(pop)

extern kernel_multiboot_header momentum_multiboot_header;

#endif