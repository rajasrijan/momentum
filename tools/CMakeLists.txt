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
project(tools)

if(NOT DEFINED SYSROOT_DIR)
    message(FATAL_ERROR "Please define SYSROOT_DIR")
endif()

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/rootfs" CACHE PATH "Choose rootfs location." FORCE)
endif()

ExternalProject_Add(binutils
    GIT_REPOSITORY "git://sourceware.org/git/binutils-gdb.git"
    GIT_TAG binutils-2_36-branch
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
    INSTALL_DIR ${SYSROOT_DIR}
    CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=<INSTALL_DIR> --target=x86_64-momentum-elf --with-sysroot=${SYSROOT_DIR} --disable-werror
    BUILD_COMMAND ${CMAKE_MAKE_PROGRAM} -j12
    EXCLUDE_FROM_ALL TRUE
)

ExternalProject_Add(gcc
    GIT_REPOSITORY "git://gcc.gnu.org/git/gcc.git"
    GIT_TAG releases/gcc-10.2.0
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
    INSTALL_DIR ${SYSROOT_DIR}
    CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=<INSTALL_DIR> --target=x86_64-momentum-elf --with-sysroot=${SYSROOT_DIR} --disable-werror --enable-languages=c,c++
    BUILD_COMMAND ${CMAKE_MAKE_PROGRAM} all-gcc all-target-libgcc -j12
    INSTALL_COMMAND make install-gcc install-target-libgcc
    DEPENDS binutils
    EXCLUDE_FROM_ALL TRUE
)

ExternalProject_Add(busybox
    GIT_REPOSITORY "git://git.busybox.net/busybox"
    GIT_TAG 1_33_stable
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    CONFIGURE_COMMAND make KBUILD_SRC=<SOURCE_DIR> -f <SOURCE_DIR>/Makefile allnoconfig
    BUILD_COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/build_target_busybox.sh ${SYSROOT_DIR}
    INSTALL_COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/install_target_busybox.sh ${SYSROOT_DIR}
    EXCLUDE_FROM_ALL TRUE
    #DEPENDS binutils gcc
)

ExternalProject_Get_Property(busybox BINARY_DIR)
install(DIRECTORY ${BINARY_DIR}/_install/ DESTINATION ".")