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
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/../cmake/x86_64-pc-momentum-elf_toolchain.cmake)
project(libc)

enable_language(C CXX ASM)

set(LIBC_SRC 
    ctype.c
    dirent.c
    errno.c
    fcntl.c
    netdb.c
    select.c
    signal.c
    socket.c
    stdio.c
    stdlib.c
    string.c
    termio.c
    termios.c
    time.c
    unistd.c
    utime.c
    vsprintf.c
)

add_library(c STATIC ${LIBC_SRC})
target_compile_options(c PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-m64 --sysroot=include>
)
target_include_directories(c 
    PUBLIC include
    PRIVATE ..
)
include(GNUInstallDirs)
install(TARGETS c)
install(DIRECTORY include/ 
DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    FILES_MATCHING PATTERN "*.h")