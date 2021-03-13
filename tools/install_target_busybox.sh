export SYSROOT=$1
export PATH=$PATH:"$SYSROOT/bin"
make LDLIBS="" install