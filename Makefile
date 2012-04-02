SRCS = $(shell find -name '*.s') $(shell find -name '*.asm') $(shell find -name '*.c')
OBJS = $(addsuffix .o,$(basename $(SRCS)))

CFLAGS :=     -m32 -Wshadow -Wpointer-arith -Wcast-align -fno-leading-underscore\
              -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
              -Wredundant-decls -Wnested-externs -Winline -Wno-long-long \
              -Wconversion -ffreestanding -Wstrict-prototypes -std=c99 \
		-I libc


LDFLAGS:= -T x86.ld -m elf_i386

ASFLAGS:=-f elf32

PATH:=$(PATH):/usr/local/bin:/usr/local/lib


OBJECT := arch/x86/loader.o arch/x86/arch_hal.o arch/x86/interrupts.o \
	arch/x86/stage2.o arch/x86/descriptor_tables.o \
	arch/x86/paging.o arch/x86/global.o arch/x86/acpi.o arch/x86/mm.o \
	arch/x86/video.o arch/x86/timer.o arch/x86/apic.o arch/x86/pci.o \
	arch/x86/multitask.o \
	arch/x86/keyboard.o \
	libc/stdio.o libc/string.o libc/stdlib.o \
	kernel/vfs.o kernel/lists.o\
	main.o

CC := i686-elf-gcc
AS := nasm
LD := i686-elf-ld

run_command = $(if $(V),$(2),@echo $(1);$(2))

all: kernel.elf

udi/udi.a:
	cd udi && $(MAKE) all

kernel.elf:$(OBJECT)
	$(LD) $(LDFLAGS) -o $@ $^

clean:
	$(RM) $(OBJECT)

backup:
	tar -cf momentum.tar $(shell find -name '*.s') $(shell find -name '*.asm') $(shell find -name '*.c') $(shell find -name '*.h')

	
	
