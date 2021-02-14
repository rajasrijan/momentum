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
