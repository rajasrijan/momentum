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

foreach(MODULE_NAME ${MOMENTUM_MODULE_LIST})
    add_subdirectory(${MODULE_NAME})
    if(TARGET ${MODULE_NAME})
        if(TARGET momentum)
            target_link_libraries(momentum ${MODULE_NAME})
        endif()
        if(TARGET BOOTX64)
            target_link_libraries(BOOTX64 ${MODULE_NAME})
        endif()
    endif()
endforeach()
