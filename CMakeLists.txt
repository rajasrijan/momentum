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
include(ExternalProject)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_TOOLCHAIN_FILE cmake/x86_64-pc-none-elf_toolchain.cmake)
project(Momentum)

enable_language(C CXX ASM)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message("Setting build type to 'Debug' as none was specified.")
  set(CMAKE_BUILD_TYPE "Debug" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/rootfs" CACHE PATH "Choose rootfs location." FORCE)
endif()

#   project configuration
set(MOMENTUM_KERNEL_BASE "0x000000C000000000" CACHE STRING "Base address where kernel will be loaded.")
set(MOMENTUM_KERNEL_VGA True CACHE BOOL "Enable VGA kernel mode.")
set(DEBUG_DUMP_ALL_CMAKE_VARIABLES False CACHE BOOL "Dump all cmake variables while configuration.")

if(MOMENTUM_KERNEL_VGA)
    set(KERNEL_GRUB_VIDEO 1)
else()
    set(KERNEL_GRUB_VIDEO 0)
endif()

#   config files
configure_file(x86_64.ld.in x86_64.ld)
configure_file(${CMAKE_SOURCE_DIR}/kernel/config.h.in ${CMAKE_SOURCE_DIR}/kernel/config.h)
configure_file(${CMAKE_SOURCE_DIR}/arch/x86_64/loader.s.in ${CMAKE_SOURCE_DIR}/arch/x86_64/loader.s)

#   source files
set(SOURCE_FILES 
    #   arch/x86_64/multiboot2.cpp
    #   DDI
    DDI/block_driver.cpp
    DDI/ddi.cpp
    DDI/driver.cpp
    DDI/pci_driver.cpp
    #   kernel files
    kernel/acpica_glue.cpp
    kernel/binary_loader.cpp
    kernel/bitmap_allocator.cpp
    kernel/ELFFile.cpp
    kernel/ELFLoader.cpp
    kernel/gui.cpp
    kernel/logging.cpp
    kernel/multitask.cpp
    kernel/sys_info.cpp
    kernel/vfs.cpp
    kernel/vfsops.cpp
    #   libc++
    libc++/cxxglue.cpp
    libc++/new.cpp
    libc++/shared_ptr.cpp
    libc++/string.cpp
    libc++/vector.cpp
    #   libc
    libc/ctype.c
    libc/dirent.c
    libc/errno.c
    libc/fcntl.c
    libc/netdb.c
    libc/select.c
    libc/signal.c
    libc/socket.c
    libc/stdio.c
    libc/stdlib.c
    libc/string.c
    libc/termio.c
    libc/termios.c
    libc/threads.cpp
    libc/time.c
    libc/unistd.c
    libc/utime.c
    libc/vsprintf.c
)

add_executable(momentum ${SOURCE_FILES})
install(TARGETS momentum
    DESTINATION .
)


# target_compile_definitions(momentum 
#     PUBLIC _MOMENTUM_=1)
target_include_directories(momentum
    PRIVATE "."
    PRIVATE "libc/include"
    PRIVATE "libc++"
    PRIVATE "acpica/include"
)

#set_target_properties(momentum PROPERTIES COMPILE_FLAGS "-m64 -mcmodel=large -masm=intel -ffreestanding")
target_compile_options(momentum PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:-m64 -mno-red-zone -fno-exceptions -fno-rtti -include new>
    $<$<COMPILE_LANGUAGE:C>:-m64>
)
#set_target_properties(momentum PROPERTIES LINK_FLAGS "-nostdlib -nodefaultlibs -v")

target_link_options(momentum PUBLIC "-Tx86_64.ld")
#   add submodules
add_subdirectory(arch/x86_64)   #   arch is made a submodule so that arch specific compilation and hackery can be kept separate from main cmake file
add_subdirectory(driver)        #   add all the drivers
add_subdirectory(acpica)        #   acpica
#   add_subdirectory(libc)          #   libc
#   build target for disk image
add_custom_target(disk_image 
    DEPENDS momentum.raw
    COMMENT "Building Disk Image"
)

ExternalProject_Add(fusepart
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/tools/fusepart
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
)

ExternalProject_Get_Property(fusepart BINARY_DIR)
set(FUSEPART_EXE "${INSTALL_DIR}/bin/fusepart")

set(TMP_ROOTFS "${PROJECT_BINARY_DIR}/rootfs")
set(TMP_DISKFS "${PROJECT_BINARY_DIR}/diskfs")

add_custom_command(OUTPUT momentum.raw
    COMMAND ${CMAKE_SOURCE_DIR}/create_image.sh momentum.raw
    #   remove old directory
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${TMP_ROOTFS}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${TMP_DISKFS}
    #   make new mount point
    COMMAND ${CMAKE_COMMAND} -E make_directory ${TMP_ROOTFS}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${TMP_DISKFS}
    COMMAND ${FUSEPART_EXE} momentum.raw ${TMP_DISKFS}
    COMMAND fuseext2 -o rw+ "${TMP_DISKFS}/part0" "${TMP_ROOTFS}"
    COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_PREFIX=${TMP_ROOTFS} -P cmake_install.cmake
    #   cleanup mount directory
    COMMAND umount ${TMP_ROOTFS}
    COMMAND umount ${TMP_DISKFS}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${TMP_ROOTFS}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${TMP_DISKFS}
    DEPENDS momentum fusepart ${CMAKE_SOURCE_DIR}/create_image.sh
)

add_custom_target(install_to_disk_image
    #   remove old directory
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${TMP_ROOTFS}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${TMP_DISKFS}
    #   make new mount point
    COMMAND ${CMAKE_COMMAND} -E make_directory ${TMP_ROOTFS}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${TMP_DISKFS}
    COMMAND ${FUSEPART_EXE} momentum.raw ${TMP_DISKFS}
    COMMAND fuseext2 -o rw+ "${TMP_DISKFS}/part0" "${TMP_ROOTFS}"
    COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_PREFIX=${TMP_ROOTFS} -P cmake_install.cmake
    #   cleanup mount directory
    COMMAND umount ${TMP_ROOTFS}
    COMMAND umount ${TMP_DISKFS}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${TMP_ROOTFS}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${TMP_DISKFS}
    DEPENDS momentum
)

add_custom_command(TARGET momentum POST_BUILD
    COMMAND ${CMAKE_OBJDUMP} -d -M intel -S momentum.elf -j .text -j .text0> momentum.s
    COMMENT "Generating object dump"
)

ExternalProject_Add(libc 
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/libc
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
)

if(DEBUG_DUMP_ALL_CMAKE_VARIABLES)
    get_cmake_property(_variableNames VARIABLES)
    list (SORT _variableNames)
    foreach (_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
endif()