all:sh ls

CFLAGS := --target=x86_64-pc-momentum-elf -O0 --std=c11 -g -o -nostdlib -nostdlibinc -masm=intel -I ../libc/

LDFLAGS := -L ../hosted_libc/ -lc
LDFLAGS += -z stack-size=2097152

ASFLAGS:=-f elf64

sh:sh.o ../hosted_libc/libc.a
	$(LD) -o sh sh.o $(LDFLAGS)

ls:ls.o ../hosted_libc/libc.a
	$(LD) -o ls ls.o $(LDFLAGS)
	objdump -s -D -M intel ls > ls.s

../hosted_libc/libc.a:
	(MAKE) -C ../hosted_libc

clean:
	$(RM) sh sh.s ls ls.s *.o