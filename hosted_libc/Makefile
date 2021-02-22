CFLAGS := --target=x86_64-pc-momentum-elf -m64 -O0 --std=c11 -nostdlib -nostdinc -fno-builtin -I/home/srijan/Documents/momentum/libc/include -I/usr/lib/llvm-11/lib/clang/11.0.0/include -masm=intel -I ../libc/ -I .. -g
CXXFLAGS := --target=x86_64-pc-momentum-elf -m64 -O0 --std=c++11 -nostdlib -nostdinc++ -masm=intel -I ../libc/ -I .. -g
LDFLAGS := -L /home/srijan/Documents/momentum/hosted_libc/ -lc -static
ASFLAGS:=-f elf64

AS := nasm
CC := clang

libc_src = $(wildcard ../libc/*.c)
libc_obj = $(patsubst ../libc/%.c,../hosted_libc/%.o,$(libc_src))
libc_obj += crt.o
libc_obj += syscall.o

tests_src = $(wildcard ./tests/*.c)
tests_tgt = $(patsubst ./tests/%.c,./tests/%,$(tests_src))

all: libc.a tests

libc.a:$(libc_obj)
	$(AR) rcs --target=elf64-x86-64 libc.a $(libc_obj)

tests:$(tests_tgt)

tests/%:tests/%.c libc.a
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	$(RM) libc.a *.o $(tests_tgt)

../hosted_libc/%.o:../libc/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

install:libc.a tests
	$(shell mkdir ../root_mnt/tests/)
	cp tests/file_tests ../root_mnt/tests/