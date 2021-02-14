export SYSROOT="$(pwd)/sysroot"
export PREFIX=x86_64-momentum-elf
rm -rf build_binutils
git clone --depth=1 -b binutils-2_36-branch git://sourceware.org/git/binutils-gdb.git
mkdir -p build_binutils
cd build_binutils
../binutils-gdb/configure --prefix="$SYSROOT" --target=x86_64-momentum-elf --with-sysroot="$SYSROOT" --disable-werror
make all -j32
make install