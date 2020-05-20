/*
 * Copyright 2009-2019 Srijan Kumar Sharma
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

#include "multiboot2.h"
#include "multiboot.h"

__attribute__((section(".multiboot"))) kernel_multiboot_header momentum_multiboot_header = {
    {
        .magic = MULTIBOOT2_HEADER_MAGIC,
        .architecture = MULTIBOOT_ARCHITECTURE_I386,
        .header_length = sizeof(kernel_multiboot_header),
        .checksum = -(multiboot_uint32_t)(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT_ARCHITECTURE_I386 + sizeof(kernel_multiboot_header)),
    },
    {
        .type = MULTIBOOT_HEADER_TAG_INFORMATION_REQUEST,
        .flags = 0,
        .size = sizeof(multiboot_header_tag_information_request) + sizeof(kernel_multiboot_header::requests),
    },
    {
        MULTIBOOT_TAG_TYPE_CMDLINE,
        MULTIBOOT_TAG_TYPE_MMAP,
        MULTIBOOT_TAG_TYPE_FRAMEBUFFER,
        MULTIBOOT_TAG_TYPE_VBE,
        MULTIBOOT_TAG_TYPE_END,
    },
    {
        .type = MULTIBOOT_HEADER_TAG_FRAMEBUFFER,
        .flags = 0,
        .size = sizeof(multiboot_header_tag_framebuffer),
        .width = 1024,
        .height = 768,
        .depth = 32,
    },
    {
        .type = MULTIBOOT_HEADER_TAG_MODULE_ALIGN,
        .flags = 0,
        .size = sizeof(multiboot_header_tag_module_align),
    },
    {
        .type = MULTIBOOT_HEADER_TAG_END,
        .flags = 0,
        .size = sizeof(multiboot_header_tag),
    },
};