CFLAGS := --target=x86_64-pc-momentum-elf -O0 --std=c11 -nostdlib -nostdlibinc -masm=intel -I ../libc/ -I .. -g
CXXFLAGS := --target=x86_64-pc-momentum-elf -O0 --std=c++11 -nostdlib -nostdinc++ -masm=intel -I ../libc/ -I .. -g
ASFLAGS:=-f elf64

libc_src = $(wildcard ../libc/*.c)
libc_obj = $(patsubst ../libc/%.c,../hosted_libc/%.o,$(libc_src)) crt.o


libc.a:$(libc_obj)
	$(AR) rcs libc.a $(libc_obj)

clean:
	$(RM) libc.a *.o

../hosted_libc/%.o:../libc/%.c
	$(CC) $(CFLAGS) -c -o $@ $<