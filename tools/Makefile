all:sh

CFLAGS := --target=x86_64-pc-momentum-elf -O0 --std=c11 -g -o -nostdlib -nostdlibinc -masm=intel -I ../libc/
LDFLAGS := -L ../hosted_libc/ -lc
ASFLAGS:=-f elf64

sh:sh.o ../hosted_libc/libc.a
	$(LD) -o sh.elf sh.o $(LDFLAGS)

../hosted_libc/libc.a:
	(MAKE) -C ../hosted_libc

clean:
	$(RM) sh.elf *.o