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

# separate_arguments(C_FLAGS_32 UNIX_COMMAND ${CMAKE_C_FLAGS})
# add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/bootstrap_functions.c.o
#     MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/bootstrap_functions.c
#     COMMAND ${CMAKE_C_COMPILER} -m32 ${C_FLAGS_32} -I${CMAKE_SOURCE_DIR} -I${CMAKE_SOURCE_DIR}/libc/include -c -o ${CMAKE_CURRENT_SOURCE_DIR}/bootstrap_functions.c.o32 ${CMAKE_CURRENT_SOURCE_DIR}/bootstrap_functions.c
#     COMMAND objcopy -O elf64-x86-64 ${CMAKE_CURRENT_SOURCE_DIR}/bootstrap_functions.c.o32 ${CMAKE_CURRENT_SOURCE_DIR}/bootstrap_functions.c.o
#     BYPRODUCTS ${CMAKE_CURRENT_SOURCE_DIR}/bootstrap_functions.c.o32
#     VERBATIM
# )

set(ARCH_X86_32_SRC
    bootstrap_functions.c
)

add_library(arch_x86_32 OBJECT ${ARCH_X86_32_SRC})
target_include_directories(arch_x86_32
    PRIVATE ${CMAKE_SOURCE_DIR}
    PRIVATE "${CMAKE_SOURCE_DIR}/libc/include"
    PRIVATE "${CMAKE_SOURCE_DIR}/libc++"
    PRIVATE "${CMAKE_SOURCE_DIR}/acpica/include"
)
target_compile_options(arch_x86_32 PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-m32>
)

add_custom_target(arch_x86_32_CVT
    COMMENT "Converting to x86_64"
    DEPENDS arch_x86_32
)

add_custom_command(TARGET arch_x86_32_CVT PRE_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O elf64-x86-64 $<TARGET_OBJECTS:arch_x86_32>
)

add_dependencies(momentum arch_x86_32_CVT)

set(ARCH_X86_64_SRC
    #   architecture specific files
    acpi.cpp
    apic.cpp
    # bootstrap_functions.c
    descriptor_tables.cpp
    font.cpp
    global.cpp
    interrupts.cpp
    keyboard.cpp
    mm.cpp
    multiboot2.cpp
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
    PRIVATE "${CMAKE_SOURCE_DIR}/acpica/include"
)
target_link_libraries(momentum arch_x86_32)

target_compile_options(arch_x86_64 PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:-m64 -fno-exceptions -fno-rtti -include new>
)
target_link_libraries(momentum arch_x86_64)     #   add self to momentum