# Copyright 2009-2021 Srijan Kumar Sharma
#
# This file is part of Momentum.
#
# Momentum is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Momentum is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Momentum.  If not, see <http://www.gnu.org/licenses/>.

export SYSROOT=$1
export PATH=$PATH:"$SYSROOT/bin"
cp .config .config.old
sed -i "s|CONFIG_SYSROOT=\"|CONFIG_SYSROOT=\"$SYSROOT|g" .config
#sed -i "s|CONFIG_EXTRA_CFLAGS=\"|CONFIG_EXTRA_CFLAGS=\"-O0|g" .config
sed -i 's/CONFIG_CROSS_COMPILER_PREFIX="/CONFIG_CROSS_COMPILER_PREFIX="x86_64-momentum-elf-/g' .config
sed -i 's/# CONFIG_LS is not set/CONFIG_LS=y/g' .config
sed -i 's/# CONFIG_DEBUG is not set/CONFIG_DEBUG=y/g' .config
sed -i 's/# CONFIG_STATIC is not set/CONFIG_STATIC=y/g' .config
sed -i 's/# CONFIG_DEBUG_PESSIMIZE is not set/CONFIG_DEBUG_PESSIMIZE=y/g' .config
#sed -i 's/# CONFIG_BUSYBOX is not set/CONFIG_BUSYBOX=y/g' .config
sed -i 's/CONFIG_INSTALL_APPLET_SYMLINKS=y/CONFIG_INSTALL_APPLET_SYMLINKS=n/g' .config
make LDLIBS=""