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
set(CMAKE_SYSTEM_NAME EFI)
set(CMAKE_SYSTEM_PROCESSOR x86_64)
set(CMAKE_CROSSCOMPILING True)

# set(CMAKE_SYSROOT .)
# set(CMAKE_STAGING_PREFIX .)
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)
set(triple x86_64-unknown-windows)
set(CMAKE_C_COMPILER clang)
set(CMAKE_C_COMPILER_TARGET ${triple})
set(CMAKE_C_FLAGS "-masm=intel -Wall -fno-builtin -fshort-wchar -fno-strict-aliasing -ffreestanding -fno-stack-protector -fno-stack-check -D_arch_efi_ -DHAVE_USE_MS_ABI -mno-red-zone -Wno-builtin-requires-header -Wno-incompatible-library-redeclaration -Wno-long-long -Wsign-compare -Wsign-conversion -D_MOMENTUM_=1")
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_COMPILER_TARGET ${triple})
set(CMAKE_CXX_FLAGS "-I${CMAKE_SOURCE_DIR}/../libc/include -I${CMAKE_SOURCE_DIR}/../libc++ -I${CMAKE_SOURCE_DIR}/.. -masm=intel -Wall -fno-builtin -fshort-wchar -fno-strict-aliasing -ffreestanding -fno-stack-protector -fno-stack-check -D_arch_efi_ -DHAVE_USE_MS_ABI -mno-red-zone -Wno-builtin-requires-header -Wno-incompatible-library-redeclaration -Wno-long-long -fno-exceptions -Wsign-compare -Wsign-conversion -fno-rtti -include new -D_MOMENTUM_=1")
set(CMAKE_C_LINK_FLAGS "-nostdlib -Wl,-dll -Wl,-subsystem:efi_application,-entry:efi_main -fuse-ld=lld-link")
set(CMAKE_CXX_LINK_FLAGS "-nostdlib -Wl,-dll -Wl,-subsystem:efi_application,-entry:efi_main -fuse-ld=lld-link")
set(CMAKE_ASM_COMPILER nasm)
set(CMAKE_AR ar)
set(CMAKE_OBJCOPY objcopy)
set(CMAKE_OBJDUMP objdump)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)