all:sh ls fusepart

CC := clang

CFLAGS := --target=x86_64-pc-momentum-elf -O0 --std=c11 -g -o -nostdlib -nostdlibinc -fno-builtin -masm=intel -I ../libc/include

LDFLAGS := -L ../hosted_libc/ -lc
LDFLAGS += -z stack-size=2097152

ASFLAGS:=-f elf64

sh:sh.o ../hosted_libc/libc.a
	$(LD) -o sh sh.o $(LDFLAGS)

ls:ls.o ../hosted_libc/libc.a
	$(LD) -o ls ls.o $(LDFLAGS)
	objdump -s -D -M intel ls > ls.s

../hosted_libc/libc.a:
	$(MAKE) -C ../hosted_libc

clean:
	$(RM) sh sh.s ls ls.s fusepart *.o

install:ls sh
	cp sh ../root_mnt/
	cp ls ../root_mnt/

shell:
	git clone --depth=1 --branch bash-5.0 git://git.savannah.gnu.org/bash.git bash
	CC='clang' CFLAGS='--target=x86_64-pc-momentum-eabi -nostdlib -nostdinc -fno-builtin -I/home/srijan/Documents/momentum/libc/ -I/usr/lib/llvm-10/lib/clang/10.0.0/include' LDFLAGS='-L /home/srijan/Documents/momentum/hosted_libc/ -lc -static' ./configure --host=x86_64-pc-momentum-eabi --enable-minimal-config
	$(MAKE) -c bash

coreutils:
	git clone --depth=1 --branch v8.32 git://git.sv.gnu.org/coreutils.git
	CC='clang' CFLAGS='--target=x86_64-pc-momentum-eabi -nostdlib -nostdinc -fno-builtin -I/home/srijan/Documents/momentum/libc/ -I/usr/lib/llvm-10/lib/clang/10.0.0/include' LDFLAGS='-L /home/srijan/Documents/momentum/hosted_libc/ -lc -static' ./configure --host=x86_64-pc-momentum-eabi

fusepart:fusepart.c
	$(CC) -o fusepart fusepart.c $(shell pkg-config fuse --cflags --libs)
