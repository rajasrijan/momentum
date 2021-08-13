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

set(ARCH_SRC 
    efi_momentum.cpp  
    mm.cpp  
    synchronization.cpp  
    time.cpp  
    video.cpp
    cxxglue.cpp
    paging.cpp
)

add_library(arch_${MOMENTUM_KERNEL_ARCH} OBJECT ${ARCH_SRC})
target_include_directories(arch_${MOMENTUM_KERNEL_ARCH}
    PRIVATE ${CMAKE_SOURCE_DIR}
    PRIVATE "${CMAKE_SOURCE_DIR}/libc/include"
    PRIVATE "${CMAKE_SOURCE_DIR}/libc++"
    PRIVATE "${CMAKE_SOURCE_DIR}/acpica/include"
)

target_compile_options(arch_${MOMENTUM_KERNEL_ARCH} PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:-m64 -fno-exceptions -fno-rtti -include new>
)
target_link_libraries(BOOTX64 arch_${MOMENTUM_KERNEL_ARCH})     #   add self to BOOTX64