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
    mman.c
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
    getopt.c
    resource.c
    glob.c
    stubs.c
    syscall.s
    setjmp.s
    setjmp.c
)

add_library(c STATIC ${LIBC_SRC})
target_compile_options(c PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-m64 -masm=intel --sysroot=.>
)
target_include_directories(c 
    PUBLIC include
    PRIVATE ..
)

add_library(g STATIC libg_dummy.c)
target_compile_options(g PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-m64 -masm=intel --sysroot=.>
)
target_include_directories(g 
    PUBLIC include
    PRIVATE ..
)

add_library(crt0 OBJECT crt0.c)
target_compile_options(crt0 PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-m64 -masm=intel --sysroot=.>
)
target_include_directories(crt0 
    PUBLIC include
    PRIVATE ..
)

add_library(crtbegin OBJECT crtbegin.c)
target_compile_options(crtbegin PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-m64 -masm=intel --sysroot=.>
)
target_include_directories(crtbegin 
    PUBLIC include
    PRIVATE ..
)


add_library(crtend OBJECT crtend.c)
target_compile_options(crtend PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-m64 -masm=intel --sysroot=.>
)
target_include_directories(crtend 
    PUBLIC include
    PRIVATE ..
)

include(GNUInstallDirs)
install(TARGETS c)
install(TARGETS g)
install(DIRECTORY include/ 
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    FILES_MATCHING PATTERN "*.h")

install(FILES $<TARGET_OBJECTS:crt0> DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RENAME crt0.o
)

install(FILES $<TARGET_OBJECTS:crtbegin> DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RENAME crtbegin.o
)

install(FILES $<TARGET_OBJECTS:crtend> DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RENAME crtend.o
)