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