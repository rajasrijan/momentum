all:hello_world

CFLAGS := --target=x86_64-pc-momentum-elf -O0 --std=c11 -g -o -nostdlib -nostdlibinc -masm=intel -I ../libc/
LDFLAGS := -L ../hosted_libc/ -lc
ASFLAGS:=-f elf64

CC := clang-7
CXX := clang++-7
AS := nasm

hello_world:hello_world.o ../hosted_libc/libc.a
	$(LD) -o hello_world.elf hello_world.o $(LDFLAGS)

../hosted_libc/libc.a:
	(MAKE) -C ../hosted_libc

clean:
	$(RM) hello_world.elf *.o