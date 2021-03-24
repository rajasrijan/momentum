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
export LIBC="$(pwd)/../libc"
export LIBC_LINK="$(pwd)/../hosted_libc"
export CC="clang"
export CFLAGS="--target=x86_64-pc-momentum-elf --static --sysroot=$SYSROOT -I$LIBC -L$LIBC_LINK"
rm -rf build_coreutils
mkdir -p build_coreutils
git clone --depth=1 https://github.com/coreutils/coreutils.git
cd coreutils
./bootstrap
cd ../build_coreutils
../coreutils/configure --host=x86_64-momentum-elf
