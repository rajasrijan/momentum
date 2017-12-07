SRCS = $(shell find -name '*.s') $(shell find -name '*.asm') $(shell find -name '*.c')
OBJS = $(addsuffix .o,$(basename $(SRCS)))

CXXINCLUDE := -include new

CFLAGS :=   -m64 -Wshadow -Wpointer-arith -Wcast-align -fno-leading-underscore\
	    -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations  -march=corei7 \
	    -Wredundant-decls -Wnested-externs -Winline -Wno-long-long -Woverflow -mno-red-zone\
	    -ffreestanding -Wstrict-prototypes -std=c11  -mcmodel=large -masm=intel\
	    -I libc/ -g -fno-function-sections -DNDEBUG --verbose

CXXFLAGS :=   -m64 -Wshadow -Wpointer-arith -Wcast-align -fno-leading-underscore -mno-red-zone\
	    -Wwrite-strings -Wmissing-declarations  -fno-exceptions -fno-rtti -march=corei7 \
	    -Wredundant-decls -Winline -Wno-long-long -Woverflow -mcmodel=large -masm=intel\
	    -ffreestanding -std=c++11 --verbose \
	    -I libc/ -I libc++/ -I . $(CXXINCLUDE) -g -fno-function-sections -DNDEBUG

LDFLAGS:= -T x86_64.ld -z max-page-size=0x1000

ASFLAGS:=-f elf64

PATH:=$(PATH):/usr/local/bin:/usr/local/lib:/bin


OBJECT := arch/x86_64/loader.o arch/x86_64/arch_hal.o arch/x86_64/interrupts.o \
	arch/x86_64/stage2.o arch/x86_64/descriptor_tables.o \
	arch/x86_64/paging.o arch/x86_64/global.o arch/x86_64/acpi.o arch/x86_64/mm.o \
	arch/x86_64/video.o arch/x86_64/timer.o arch/x86_64/apic.o arch/x86_64/pci.o \
	arch/x86_64/multitask.o arch/x86_64/multiboot2.o arch/x86_64/keyboard.o \
	libc/stdio.o libc/string.o libc/stdlib.o libc/threads.o \
	kernel/lists.o kernel/vfs.o kernel/vfsops.o kernel/ELFLoader.o kernel/ELFFile.o \
	DDI/driver.o DDI/ddi.o DDI/pci_driver.o DDI/block_driver.o\
	driver/ata.o driver/ramdrive.o driver/fatgen.o\
	driver/usb/uhci.o\
	main.o cxxglue.o 

CC := x86_64-elf-gcc
CXX := x86_64-elf-g++
AS := nasm
LD := x86_64-elf-ld

run_command = $(if $(V),$(2),@echo $(1);$(2))

all: kernel.elf

kernel.elf:$(OBJECT)
	$(LD) $(LDFLAGS) -o $@ $^
	x86_64-elf-objdump -x kernel.elf > objdump.txt
	x86_64-elf-objdump -d -M intel -S kernel.elf -j .text -j .text0> kernel.s
#	./ImFat momentum.vhd create kernel.elf < kernel.elf
source.elf:source.o
	
#%.o:%.cpp
#	$(CXX) $(CXXFLAGS) -c -o $@ $^

clean:
	$(RM) $(OBJECT)
	$(RM) kernel.elf

backup:
	tar -cf momentum.tar $(shell find -name '*.s') $(shell find -name '*.asm') $(shell find -name '*.c') $(shell find -name '*.h') $(shell find -name '*.cpp')
