CURRENT_DIR=$(pwd)
export SYSROOT="$(pwd)/sysroot"
export ROOT_FS="$(pwd)/root_fs"
export LIBC="$(pwd)/../libc"
export LIBC_LINK="$(pwd)/../hosted_libc"
export PATH=$PATH:"$SYSROOT/bin"
git clone --depth=1 -b "1_33_stable" git://git.busybox.net/busybox
cd busybox
make clean
make defconfig
cp .config .config.old
sed -i -r 's/CONFIG_([a-zA-Z0-9_]*)=y/CONFIG_\1=n/g' .config
sed -i 's/CONFIG_CROSS_COMPILER_PREFIX="/CONFIG_CROSS_COMPILER_PREFIX="x86_64-momentum-elf-/g' .config
sed -i 's/CONFIG_DESKTOP=n/CONFIG_DESKTOP=y/g' .config
sed -i 's/CONFIG_LS=n/CONFIG_LS=y/g' .config
make -j12