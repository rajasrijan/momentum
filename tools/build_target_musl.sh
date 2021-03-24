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

CURRENT_DIR=$(pwd)
export SYSROOT="$(pwd)/sysroot"
export ROOT_FS="$(pwd)/root_fs"
export LIBC="$(pwd)/../libc"
export LIBC_LINK="$(pwd)/../hosted_libc"
export PATH=$PATH:"$SYSROOT/bin"

#export CFLAGS="-Wno-error=suggest-attribute=const -Wno-error=implicit-function-declaration -Wno-error"
rm -rf build_musl
mkdir -p build_musl
git clone --depth=1 git://git.musl-libc.org/musl
cd musl
cd ../build_musl
"$CURRENT_DIR/musl/configure" --build=x86_64-linux-gnu --host=x86_64-momentum-elf --disable-shared --prefix="$ROOT_FS/usr"
make -j12
make install -j12
cp -r "$ROOT_FS/usr" "$SYSROOT"