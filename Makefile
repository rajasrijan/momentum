BUILD_MODE ?= debug

CXXINCLUDE := -include new
TARGET := x86_64-pc-none-elf
CFLAGS :=
CXXFLAGS := 

CFLAGS += -g -O0
CXXFLAGS += -g -O0

CFLAGS += --target=$(TARGET) -m64 -Wshadow -Wwrite-strings  -D_MOMENTUM_ -Wno-unused-function -Wredundant-decls -Wnested-externs -Winline -Wno-long-long -Woverflow --std=c11 -mno-red-zone\
	    -ffreestanding -std=c11  -mcmodel=large -masm=intel -I libc/ -I . -fno-function-sections -I acpica/include

CXXFLAGS += --target=$(TARGET) -m64 -Wall -Wextra -Wunused-result -Wshadow -Wpointer-arith -mno-red-zone -Wwrite-strings -Werror -Wno-unused-parameter -Wno-null-pointer-arithmetic -Wno-missing-braces -fno-exceptions -fno-rtti -Weffc++ -Wredundant-decls -Winline -Wno-long-long -Woverflow -mcmodel=large -masm=intel -ffreestanding -std=c++20  -D_MOMENTUM_ \
	    -I libc/ -I libc++/ -I .  -I acpica/include $(CXXINCLUDE) -fno-function-sections

LDFLAGS:= -T x86_64.ld -z max-page-size=0x1000

ASFLAGS:=-f elf64

acpica_objects := $(patsubst %.c,%.o,$(wildcard acpica/*/*.c))

OBJECT := arch/x86_64/loader.o libc/string.o libc++/string.o arch/x86_64/arch_hal.o arch/x86_64/interrupts.o \
	arch/x86_64/stage2.o arch/x86_64/descriptor_tables.o arch/x86_64/paging.o arch/x86_64/global.o arch/x86_64/acpi.o arch/x86_64/mm.o \
	arch/x86_64/video.o arch/x86_64/timer.o arch/x86_64/apic.o arch/x86_64/pci.o arch/x86_64/font.o \
	arch/x86_64/multiboot2.o arch/x86_64/keyboard.o arch/x86_64/rtc.o libc/stdio.o libc/vsprintf.o libc/stdlib.o libc/threads.o libc++/new.o \
	kernel/vfs.o kernel/vfsops.o kernel/ELFLoader.o kernel/ELFFile.o kernel/sys_info.o kernel/multitask.o kernel/gui.o \
	DDI/driver.o DDI/ddi.o DDI/pci_driver.o DDI/block_driver.o driver/disk/ata.o driver/disk/sata.o driver/disk/ide.o driver/ramdrive.o driver/fatgen.o driver/binary_loader.o\
	driver/usb/uhci.o kernel/acpica_glue.o $(acpica_objects) main.o cxxglue.o stack_protector.o

CC := clang
CXX := clang++
AS := nasm
LD := ld

all: kernel.elf
	$(MAKE) -C hosted_libc CC=$(CC)
	$(MAKE) -C tools CC=$(CC)

kernel.elf:$(OBJECT) x86_64.ld
	$(LD) $(LDFLAGS) -o $@ $(OBJECT)
	objdump -x kernel.elf > objdump.txt
	objdump -d -M intel -S kernel.elf -j .text -j .text0> kernel.s
	
clean:
	$(MAKE) -C hosted_libc clean
	$(MAKE) -C tools clean
	$(RM) $(OBJECT)
	$(RM) kernel.elf kernel.s

backup:
	tar -cf momentum.tar $(shell find -name '*.s') $(shell find -name '*.asm') $(shell find -name '*.c') $(shell find -name '*.h') $(shell find -name '*.cpp')
	
install:kernel.elf
	$(MAKE) -C hosted_libc CC=$(CC)
	$(MAKE) -C tools CC=$(CC)
	MTOOLS_SKIP_CHECK=1 mcopy -o -i momentum.raw@@1M kernel.elf ::kernel.elf
	MTOOLS_SKIP_CHECK=1 mcopy -o -i momentum.raw@@1M tools/sh ::sh
	MTOOLS_SKIP_CHECK=1 mcopy -o -i momentum.raw@@1M tools/ls ::ls

tools/%:
	$(MAKE) -C tools

analyze:
	scan-build --use-cc=$(CC) --use-c++=$(CXX) --analyzer-target=$(TARGET) make -j12

.INTERMEDIATE:x86_64.ld

%.ld:%.ld.in
	$(CC) -I . -E -x c -P -o $@ $^

%.s:%.s.in
	$(CC) -I . -E -x c -P -o $@ $^