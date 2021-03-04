CURRENT_DIR=$(pwd)
export SYSROOT="$(pwd)/sysroot"
export ROOT_FS="$(pwd)/root_fs"
export LIBC="$(pwd)/../libc"
export LIBC_LINK="$(pwd)/../hosted_libc"
export PATH=$PATH:"$SYSROOT/bin"

export CFLAGS="-Wno-error=suggest-attribute=const -Wno-error=implicit-function-declaration -Wno-error"
rm -rf build_coreutils
mkdir -p build_coreutils
#git clone --depth=1 https://github.com/coreutils/coreutils.git
cd coreutils
#./bootstrap
cd ../build_coreutils
# twice coz coreutils is stupid
"$CURRENT_DIR/coreutils/configure" --without-selinux --prefix="$ROOT_FS"
make src/ls -j12
make clean
# twice coz coreutils crosscompile is broken
"$CURRENT_DIR/coreutils/configure" --build=x86_64-linux-gnu --host=x86_64-momentum-elf --without-selinux --prefix="$ROOT_FS"
make src/ls -j12
#make install