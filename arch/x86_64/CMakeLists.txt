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


#   trampolin binary blob
add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/trampolin.cpp
    MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/trampolin.s
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${CMAKE_ASM_COMPILER} -f bin trampolin.s -o trampolin
    COMMAND xxd -i trampolin > trampolin.cpp
    BYPRODUCTS ${CMAKE_CURRENT_SOURCE_DIR}/trampolin
    COMMENT "Generating trampolin.cpp"
)

set(ARCH_X86_64_SRC
    #   architecture specific files
    acpi.cpp
    apic.cpp
    descriptor_tables.cpp
    global.cpp
    interrupts.cpp
    keyboard.cpp
    mm.cpp
    paging.cpp
    pci.cpp
    rtc.cpp
    stage2.cpp
    timer.cpp
    trampolin.cpp
    video.cpp
    arch_hal.s
    loader.s
)

add_library(arch_x86_64 OBJECT ${ARCH_X86_64_SRC})
target_include_directories(arch_x86_64
    PRIVATE ${CMAKE_SOURCE_DIR}
    PRIVATE "${CMAKE_SOURCE_DIR}/libc/include"
    PRIVATE "${CMAKE_SOURCE_DIR}/libc++"
    PRIVATE "${CMAKE_SOURCE_DIR}/acpica/acpica/source/include"
)

target_compile_options(arch_x86_64 PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:-m64 -fno-exceptions -fno-rtti -include new>
)
target_link_libraries(momentum arch_x86_64)     #   add self to momentum
