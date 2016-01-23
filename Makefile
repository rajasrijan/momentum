SRCS = $(shell find -name '*.s') $(shell find -name '*.asm') $(shell find -name '*.c')
OBJS = $(addsuffix .o,$(basename $(SRCS)))

CXXINCLUDE := -include new

CFLAGS :=   -m32 -Wshadow -Wpointer-arith -Wcast-align -fno-leading-underscore\
	    -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
	    -Wredundant-decls -Wnested-externs -Winline -Wno-long-long -Woverflow \
	    -Wconversion -ffreestanding -Wstrict-prototypes -std=c11  \
	    -I libc/ -g

CXXFLAGS :=   -m32 -Wshadow -Wpointer-arith -Wcast-align -fno-leading-underscore\
	    -Wwrite-strings -Wmissing-declarations  -fno-exceptions -fno-rtti\
	    -Wredundant-decls -Winline -Wno-long-long -Woverflow \
	    -Wconversion -ffreestanding -std=c++11 \
	    -I libc/ -I libc++/ -I . $(CXXINCLUDE) -g

LDFLAGS:= -T x86.ld -m elf_i386

ASFLAGS:=-f elf32

PATH:=$(PATH):/usr/local/bin:/usr/local/lib:/bin


OBJECT := arch/x86/loader.o arch/x86/arch_hal.o arch/x86/interrupts.o \
	arch/x86/stage2.o arch/x86/descriptor_tables.o \
	arch/x86/paging.o arch/x86/global.o arch/x86/acpi.o arch/x86/mm.o \
	arch/x86/video.o arch/x86/timer.o arch/x86/apic.o arch/x86/pci.o \
	arch/x86/multitask.o \
	arch/x86/keyboard.o \
	libc/stdio.o libc/string.o libc/stdlib.o libc/threads.o \
	kernel/lists.o kernel/vfs.o kernel/vfsops.o\
	DDI/ddi.o DDI/pci_driver.o DDI/block_driver.o DDI/filesystem.o\
	driver/ata.o driver/ataio.o driver/driver.o driver/ramdrive.o driver/fatgen.o\
	main.o cxxglue.o 

CC := i686-elf-gcc
CXX := i686-elf-g++
AS := nasm
LD := i686-elf-ld

run_command = $(if $(V),$(2),@echo $(1);$(2))

all: kernel.elf
	$(MAKE) -C tools
#	$(RM) ramdrive.raw
#	tools/ImCopy.exe momentum.raw 0 kernel.elf
#	tools/AppendRamDrive.exe ramdrive.raw gpl.txt
#	tools/AppendRamDrive.exe ramdrive.raw main.c
#	tools/AppendRamDrive.exe ramdrive.raw kernel.elf
#	$(RM) $(OBJECT)
kernel.elf:$(OBJECT)
	$(LD) $(LDFLAGS) -o $@ $^

clean:
	$(RM) $(OBJECT)
	$(RM) kernel.elf
	$(MAKE) -C tools clean

backup:
	tar -cf momentum.tar $(shell find -name '*.s') $(shell find -name '*.asm') $(shell find -name '*.c') $(shell find -name '*.h') $(shell find -name '*.cpp')
