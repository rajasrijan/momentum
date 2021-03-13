export SYSROOT=$1
export PATH=$PATH:"$SYSROOT/bin"
cp .config .config.old
sed -i "s|CONFIG_SYSROOT=\"|CONFIG_SYSROOT=\"$SYSROOT|g" .config
#sed -i "s|CONFIG_EXTRA_CFLAGS=\"|CONFIG_EXTRA_CFLAGS=\"-g|g" .config
sed -i 's/CONFIG_CROSS_COMPILER_PREFIX="/CONFIG_CROSS_COMPILER_PREFIX="x86_64-momentum-elf-/g' .config
sed -i 's/CONFIG_LS=n/CONFIG_LS=y/g' .config
sed -i 's/# CONFIG_DEBUG is not set/CONFIG_DEBUG=y/g' .config
make LDLIBS="" V=1