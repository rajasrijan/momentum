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

set(${MODULE_NAME}_SRC ata.cpp  ide.cpp  sata.cpp)

set(MOMENTUM_MODULE_${MODULE_NAME} "Static" CACHE STRING "Build module '${MODULE_NAME}'.")
set_property(CACHE MOMENTUM_MODULE_${MODULE_NAME} PROPERTY STRINGS "None" "Static" "Module")
if(MOMENTUM_MODULE_${MODULE_NAME} STREQUAL "Static")
    add_library(${MODULE_NAME} OBJECT ${${MODULE_NAME}_SRC})
    target_include_directories(${MODULE_NAME}
        PRIVATE ${CMAKE_SOURCE_DIR}
        PRIVATE "${CMAKE_SOURCE_DIR}/libc/include"
        PRIVATE "${CMAKE_SOURCE_DIR}/libc++"
    )
    target_compile_options(${MODULE_NAME} PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:-m64 -fno-exceptions -fno-rtti -include new>
    )
    target_link_libraries(momentum ${MODULE_NAME})
elseif(MOMENTUM_MODULE_${MODULE_NAME} STREQUAL "Module")
    add_library(${MODULE_NAME} MODULE ${MODULE_NAME}_SRC)
    target_include_directories(${MODULE_NAME}
        PRIVATE ${CMAKE_SOURCE_DIR}
        PRIVATE "${CMAKE_SOURCE_DIR}/libc/include"
        PRIVATE "${CMAKE_SOURCE_DIR}/libc++"
    )
    target_compile_options(${MODULE_NAME} PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:-m64 -fno-exceptions -fno-rtti -include new>
    )
    target_link_libraries(momentum ${MODULE_NAME})
endif()
