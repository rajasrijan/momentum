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

message(TRACE "Setting up toolchain")
set(CMAKE_SYSTEM_NAME Momentum)
set(CMAKE_SYSTEM_PROCESSOR x86_64)
set(CMAKE_CROSSCOMPILING True)

# set(CMAKE_SYSROOT .)
# set(CMAKE_STAGING_PREFIX .)
set(triple x86_64-pc-momentum-elf)
set(CMAKE_C_COMPILER clang)
set(CMAKE_C_COMPILER_TARGET ${triple})
set(CMAKE_C_FLAGS "-masm=intel -Wall -D_MOMENTUM_=1")
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_COMPILER_TARGET ${triple})
set(CMAKE_CXX_FLAGS "-masm=intel -Wall -D_MOMENTUM_=1")
set(CMAKE_LINK_FLAGS "-nostdlib")
set(CMAKE_ASM_COMPILER nasm)
set(CMAKE_LINKER ld)
set(CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_LINKER> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> <LINK_LIBRARIES> -o <TARGET>")
set(CMAKE_AR ar)
set(CMAKE_OBJCOPY objcopy)
set(CMAKE_OBJDUMP objdump)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)