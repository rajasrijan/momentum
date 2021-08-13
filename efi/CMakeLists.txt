#
#  Copyright 2009-2021 Srijan Kumar Sharma
#
#  This file is part of Momentum.
#
#  Momentum is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  Momentum is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with Momentum.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.16)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../cmake)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/../cmake/efi-pc-none-elf_toolchain.cmake)
project(efi)

enable_language(C CXX ASM)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)

set(EFI_SRC
    ../arch/efi/mm.cpp
    ../arch/efi/synchronization.cpp
    ../arch/efi/time.cpp
    ../arch/efi/video.cpp
    ../arch/video.cpp
    ../kernel/logging.cpp
    ../kernel/vfs.cpp
    ../kernel/vnode.cpp
    ../libc/string.c
    ../libc/stdlib.c
    ../libc/stdio.c
    ../libc/vsprintf.c
    ../libc/threads.cpp
    ../libc++/new.cpp
    efi_momentum.cpp
)

add_executable(BOOTX64 ${EFI_SRC})
target_compile_options(BOOTX64 PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-m64 -masm=intel>
)

target_include_directories(BOOTX64 
    PUBLIC ../libc++/
    PUBLIC ../libc/include
    PRIVATE ..
)

set(MOMENTUM_MODULE_LIST 
    ext
)
add_subdirectory(../driver driver)

install(TARGETS BOOTX64
DESTINATION EFI/BOOT/)