export SYSROOT="$(pwd)/sysroot"
export PREFIX=x86_64-momentum-elf
rm -rf build_gcc
git clone --depth=1 -b releases/gcc-10.2.0 git://gcc.gnu.org/git/gcc.git
mkdir -p build_gcc
cd build_gcc
../gcc/configure --prefix="$SYSROOT" --target=x86_64-momentum-elf --with-sysroot="$SYSROOT" --disable-werror  --enable-languages=c,c++
make all-gcc -j 32
make all-target-libgcc -j 32
make install-gcc -j 32
make install-target-libgcc -j 32