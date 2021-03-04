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
project(Tools)
find_package(PkgConfig REQUIRED)

pkg_check_modules(FUSE fuse REQUIRED)
add_executable(fusepart fusepart.c)
if(FUSE_FOUND)
    target_include_directories(fusepart PRIVATE ${FUSE_INCLUDE_DIRS})
    target_link_libraries(fusepart ${FUSE_LINK_LIBRARIES})
endif()
include(GNUInstallDirs)
install(TARGETS fusepart)
target_compile_definitions(fusepart PRIVATE _FILE_OFFSET_BITS=64)