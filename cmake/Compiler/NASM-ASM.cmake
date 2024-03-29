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

set(CMAKE_ASM_SOURCE_FILE_EXTENSIONS s;S;asm)
if(${CMAKE_SYSTEM_NAME} STREQUAL "Momentum")
    set(CMAKE_ASM_COMPILE_OBJECT  "<CMAKE_ASM_COMPILER> -f elf64 -o <OBJECT> <SOURCE>")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "EFI")
    set(CMAKE_ASM_COMPILE_OBJECT  "<CMAKE_ASM_COMPILER> -f win64 -o <OBJECT> <SOURCE>")
else()
    message(ERROR "Unknown system!")
endif()
