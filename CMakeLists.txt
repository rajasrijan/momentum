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

set(MOMENTUM_KERNEL_ARCH "x86_64" CACHE STRING "Kernel architecture.")
set_property(CACHE MOMENTUM_KERNEL_ARCH PROPERTY STRINGS "x86_64" "efi")

set(CMAKE_TOOLCHAIN_FILE cmake/${MOMENTUM_KERNEL_ARCH}-pc-none-elf_toolchain.cmake)
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
set(MOMENTUM_KERNEL_BASE_VADDR "0x000000C000000000" CACHE STRING "Base virtual address where kernel will be loaded.")
set(MOMENTUM_KERNEL_BASE_PADDR "0x0000000000100000" CACHE STRING "Base physical address where kernel will be loaded.")
set(MOMENTUM_KERNEL_VGA True CACHE BOOL "Enable VGA kernel mode.")
set(MOMENTUM_KERNEL_COVERAGE True CACHE BOOL "Enable code coverage.")
set(DEBUG_DUMP_ALL_CMAKE_VARIABLES False CACHE BOOL "Dump all cmake variables while configuration.")

if(MOMENTUM_KERNEL_VGA)
    set(KERNEL_GRUB_VIDEO 1)
else()
    set(KERNEL_GRUB_VIDEO 0)
endif()

#   config files
configure_file(${CMAKE_SOURCE_DIR}/kernel/config.h.in ${CMAKE_SOURCE_DIR}/kernel/config.h)

if(MOMENTUM_KERNEL_ARCH STREQUAL "x86_64")
    configure_file(${CMAKE_SOURCE_DIR}/arch/x86_64/loader.s.in ${CMAKE_SOURCE_DIR}/arch/x86_64/loader.s)
endif()

#   source files
set(SRC_FILES 
    arch/video.cpp
    #   DDI
    DDI/ddi.cpp
    DDI/driver.cpp
    #   kernel files
    kernel/bitmap_allocator.cpp
    kernel/binary_loader.cpp
    kernel/ELFFile.cpp
    kernel/ELFLoader.cpp
    kernel/font.cpp
    kernel/framebuffer.cpp
    #   kernel/gui.cpp
    kernel/logging.cpp
    kernel/sys_info.cpp
    kernel/vfs.cpp
    kernel/vnode.cpp
    kernel/vfsops.cpp
    kernel/multitask.cpp
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

if(MOMENTUM_KERNEL_ARCH STREQUAL "x86_64")
    configure_file(x86_64.ld.in x86_64.ld)
    set(SRC_FILES
        ${SRC_FILES}
        DDI/pci_driver.cpp
        kernel/acpica_glue.cpp
    )
    add_executable(momentum ${SRC_FILES})
    install(TARGETS momentum
        DESTINATION .
    )

    target_include_directories(momentum
        PRIVATE "."
        PRIVATE "libc/include"
        PRIVATE "libc++"
        PRIVATE "acpica/include"
    )
    
    target_compile_options(momentum PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:-m64 -mno-red-zone -fno-exceptions -fno-rtti -Wpedantic -include new>
        $<$<COMPILE_LANGUAGE:C>:-m64 -Wpedantic>
    )
    target_link_options(momentum PUBLIC "-Tx86_64.ld")
elseif(MOMENTUM_KERNEL_ARCH STREQUAL "efi")
    add_executable(BOOTX64 ${SRC_FILES})

    add_custom_command(TARGET BOOTX64 POST_BUILD
        COMMAND ${CMAKE_OBJDUMP} -D -x -M intel -S BOOTX64.efi > BOOTX64.s
        COMMENT "Generating object dump"
    )

    install(TARGETS BOOTX64
        DESTINATION .
    )
    
    target_include_directories(BOOTX64
        PRIVATE "."
        PRIVATE "libc/include"
        PRIVATE "libc++"
        PRIVATE "acpica/include"
    )
    
    target_compile_options(BOOTX64 PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:-m64 -mno-red-zone -fno-exceptions -fno-rtti -include new>
        $<$<COMPILE_LANGUAGE:C>:-m64>
    )
endif()

#   add submodules
if(MOMENTUM_KERNEL_ARCH STREQUAL "x86_64")
    set(MOMENTUM_MODULE_LIST 
        disk
        fatgen
        ext
        standard_output
    )
elseif(MOMENTUM_KERNEL_ARCH STREQUAL "efi")
    set(MOMENTUM_MODULE_LIST 
        ext
    )
endif()

add_subdirectory(arch/${MOMENTUM_KERNEL_ARCH})   #   arch is made a submodule so that arch specific compilation and hackery can be kept separate from main cmake file
add_subdirectory(driver)        #   add all the drivers

if(MOMENTUM_KERNEL_ARCH STREQUAL "x86_64")
    add_subdirectory(acpica)        #   acpica

    #   build target for disk image
    add_custom_target(disk_image 
        DEPENDS momentum.raw
        COMMENT "Building Disk Image"
    )

    ExternalProject_Add(fusepart
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/tools/fusepart
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
    )

    ExternalProject_Get_Property(fusepart INSTALL_DIR)
    set(FUSEPART_EXE "${INSTALL_DIR}/bin/fusepart")

    set(TMP_ROOTFS "${PROJECT_BINARY_DIR}/rootfs")
    set(TMP_EFIFS "${PROJECT_BINARY_DIR}/EFI")
    set(TMP_DISKFS "${PROJECT_BINARY_DIR}/diskfs")

    add_custom_command(OUTPUT momentum.raw
        #   create the actual file
        COMMAND ${CMAKE_COMMAND} -E echo "Creating momentum.raw"
        COMMAND truncate -s 256M momentum.raw
        #   format the file
        COMMAND ${CMAKE_COMMAND} -E echo "Creating partition table on momentum.raw"
        COMMAND bash -c "echo -e \"start=2048,size=202753,type=ef,bootable\\nstart=206848,type=83\" | sfdisk -q momentum.raw"
        #   remove old directory
        COMMAND ${CMAKE_COMMAND} -E echo "Removing old mount paths"
        #   wait for folder to unmount
        COMMAND bash -c "while mountpoint -q ${TMP_ROOTFS}; do echo \"Waiting for ${TMP_ROOTFS} to unmount...\"; sleep 1; umount ${TMP_ROOTFS}; done; rm -rf ${TMP_ROOTFS}"
        #   wait for folder to unmount
        COMMAND bash -c "while mountpoint -q ${TMP_EFIFS}; do echo \"Waiting for ${TMP_EFIFS} to unmount...\"; sleep 1; umount ${TMP_EFIFS}; done; rm -rf ${TMP_EFIFS}"
        #   wait for folder to unmount
        COMMAND bash -c "while mountpoint -q ${TMP_DISKFS}; do echo \"Waiting for ${TMP_DISKFS} to unmount...\"; sleep 1; umount ${TMP_DISKFS}; done; rm -rf ${TMP_DISKFS}"
        #   make new mount point
        COMMAND ${CMAKE_COMMAND} -E echo "Creating new mount paths"
        COMMAND ${CMAKE_COMMAND} -E make_directory ${TMP_ROOTFS}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${TMP_DISKFS}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${TMP_EFIFS}
        COMMAND ${CMAKE_COMMAND} -E echo "Mounting momentum.raw"
        COMMAND ${FUSEPART_EXE} momentum.raw ${TMP_DISKFS}
        COMMAND ${CMAKE_COMMAND} -E echo "Formatting partition 0 to EFI partition.."
        COMMAND mkfs.vfat -F 32 "${TMP_DISKFS}/part0"
        COMMAND ${CMAKE_COMMAND} -E echo "Formatting partition 1 to EXT2 partition.."
        COMMAND mkfs.ext2 "${TMP_DISKFS}/part1"
        #   cleanup mount directory
        COMMAND ${CMAKE_COMMAND} -E echo "Removing mount paths"
        #   wait for folder to unmount
        COMMAND bash -c "while mountpoint -q ${TMP_ROOTFS}; do echo \"Waiting for ${TMP_ROOTFS} to unmount...\"; sleep 1; umount ${TMP_ROOTFS}; done; rm -rf ${TMP_ROOTFS}"
        #   wait for folder to unmount
        COMMAND bash -c "while mountpoint -q ${TMP_EFIFS}; do echo \"Waiting for ${TMP_EFIFS} to unmount...\"; sleep 1; umount ${TMP_EFIFS}; done; rm -rf ${TMP_EFIFS}"
        #   wait for folder to unmount
        COMMAND bash -c "while mountpoint -q ${TMP_DISKFS}; do echo \"Waiting for ${TMP_DISKFS} to unmount...\"; sleep 1; umount ${TMP_DISKFS}; done; rm -rf ${TMP_DISKFS}"
        # DEPENDS momentum fusepart
        VERBATIM
    )
    add_custom_command(TARGET momentum POST_BUILD
        COMMAND ${CMAKE_OBJDUMP} -d -x -h -M intel -s -S momentum.elf> momentum.s
        COMMENT "Generating object dump"
    )

    set(SYSROOT_DIR ${CMAKE_BINARY_DIR}/sysroot)

    ExternalProject_Add(libc
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/libc
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${SYSROOT_DIR}/usr -DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    )

    ExternalProject_Add(efi
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/.
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> -DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE} -DCMAKE_BUILD_TYPE=Release -DMOMENTUM_KERNEL_ARCH=efi
        BUILD_ALWAYS 1
    )   
    
    ExternalProject_Get_Property(efi BINARY_DIR)
    set(efi_BINARY_DIR ${BINARY_DIR})

    #   various tools
    # add_subdirectory(${CMAKE_SOURCE_DIR}/tools)

    add_custom_target(install_to_disk_image
        #   remove old directory
        COMMAND ${CMAKE_COMMAND} -E echo "Removing old mount paths"
        #   wait for folder to unmount
        COMMAND bash -c "while mountpoint -q ${TMP_ROOTFS}; do echo \"Waiting for ${TMP_ROOTFS} to unmount...\"; sleep 1; umount ${TMP_ROOTFS}; done; rm -rf ${TMP_ROOTFS}"
        #   wait for folder to unmount
        COMMAND bash -c "while mountpoint -q ${TMP_EFIFS}; do echo \"Waiting for ${TMP_EFIFS} to unmount...\"; sleep 1; umount ${TMP_EFIFS}; done; rm -rf ${TMP_EFIFS}"
        #   wait for folder to unmount
        COMMAND bash -c "while mountpoint -q ${TMP_DISKFS}; do echo \"Waiting for ${TMP_DISKFS} to unmount...\"; sleep 1; umount ${TMP_DISKFS}; done; rm -rf ${TMP_DISKFS}"
        #   make new mount point
        COMMAND ${CMAKE_COMMAND} -E echo "Creating new mount paths"
        COMMAND ${CMAKE_COMMAND} -E make_directory ${TMP_ROOTFS}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${TMP_DISKFS}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${TMP_EFIFS}
        COMMAND ${CMAKE_COMMAND} -E echo "Mounting momentum.raw"
        COMMAND ${FUSEPART_EXE} momentum.raw ${TMP_DISKFS}
        COMMAND ${CMAKE_COMMAND} -E echo "Mounting EFI and OS drives"
        COMMAND fusefat -o rw+ "${TMP_DISKFS}/part0" "${TMP_EFIFS}"
        COMMAND fuseext2 -o rw+ "${TMP_DISKFS}/part1" "${TMP_ROOTFS}"
        COMMAND ${CMAKE_COMMAND} -E echo "Installing Momentum to OS partition"
        COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_PREFIX=${TMP_ROOTFS} -P cmake_install.cmake
        COMMAND ${CMAKE_COMMAND} -E echo "Installing EFI bootloader to EFI partition"
        COMMAND rm -f ${TMP_EFIFS}/EFI/BOOT/BOOTX64.efi
        COMMAND ${CMAKE_COMMAND} -E make_directory ${TMP_EFIFS}/EFI/BOOT/
        COMMAND cp ${efi_BINARY_DIR}/BOOTX64.efi ${TMP_EFIFS}/EFI/BOOT/BOOTX64.efi
        # COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_PREFIX=${TMP_EFIFS} -P ${efi_BINARY_DIR}/cmake_install.cmake
        #   cleanup mount directory
        # COMMAND ${CMAKE_COMMAND} -E echo "Removing mount paths"
        #   wait for folder to unmount
        COMMAND bash -c "while mountpoint -q ${TMP_ROOTFS}; do echo \"Waiting for ${TMP_ROOTFS} to unmount...\"; sleep 1; umount ${TMP_ROOTFS}; done; rm -rf ${TMP_ROOTFS}"
        #   wait for folder to unmount
        COMMAND bash -c "while mountpoint -q ${TMP_EFIFS}; do echo \"Waiting for ${TMP_EFIFS} to unmount...\"; sleep 1; umount ${TMP_EFIFS}; done; rm -rf ${TMP_EFIFS}"
        #   wait for folder to unmount
        COMMAND bash -c "while mountpoint -q ${TMP_DISKFS}; do echo \"Waiting for ${TMP_DISKFS} to unmount...\"; sleep 1; umount ${TMP_DISKFS}; done; rm -rf ${TMP_DISKFS}"
        DEPENDS momentum.raw efi fusepart momentum
        VERBATIM
    )

endif()
if(DEBUG_DUMP_ALL_CMAKE_VARIABLES)
    get_cmake_property(_variableNames VARIABLES)
    list (SORT _variableNames)
    foreach (_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
endif()
