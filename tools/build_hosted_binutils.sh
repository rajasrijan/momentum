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

export SYSROOT="$(pwd)/sysroot"
export PREFIX=x86_64-momentum-elf
rm -rf build_binutils
git clone --depth=1 -b binutils-2_36-branch git://sourceware.org/git/binutils-gdb.git
mkdir -p build_binutils
cd build_binutils
../binutils-gdb/configure --prefix="$SYSROOT" --target=x86_64-momentum-elf --with-sysroot="$SYSROOT" --disable-werror
make all -j32
make install