all:hello_world

CFLAGS := --target=x86_64-pc-none-elf -O0 --std=c11 -g -o -nostdlib -nostdinc -isystem ./host_libc/include
ASFLAGS:=-f elf64

CC:=clang-7
AS := nasm

hello_world:hello_world.o
	$(LD) -o hello_world.elf hello_world.o
	
clean:
	$(RM) hello_world.elf hello_world.o