CXXINCLUDE := -include new
#TARGET := --target=x86_64-pc-none-elf
CFLAGS := $(TARGET) -m64 -Wshadow\
	    -Wwrite-strings  -D_MOMENTUM_ -Wno-unused-function -fstack-protector-all\
	    -Wredundant-decls -Wnested-externs -Winline -Wno-long-long -Woverflow -mno-red-zone\
	    -ffreestanding -std=c11  -mcmodel=large -masm=intel \
	    -I libc/ -g -O0 -fno-function-sections -I acpica/include

CXXFLAGS := $(TARGET) -m64 -Wshadow -Wpointer-arith -mno-red-zone -Werror\
	    -Wwrite-strings -fno-exceptions -fno-rtti -fstack-protector-all -Weffc++\
	    -Wredundant-decls -Winline -Wno-long-long -Woverflow -mcmodel=large -masm=intel\
	    -ffreestanding -std=c++17  -D_MOMENTUM_ \
	    -I libc/ -I libc++/ -I .  -I acpica/include $(CXXINCLUDE) -g -O0 -fno-function-sections

LDFLAGS:= -T x86_64.ld -z max-page-size=0x1000

ASFLAGS:=-f elf64

PATH:=$(PATH):/usr/local/bin:/usr/local/lib:/bin

acpica_objects := $(patsubst %.c,%.o,$(wildcard acpica/*/*.c))

OBJECT := libc/string.o libc++/string.o arch/x86_64/loader.o arch/x86_64/arch_hal.o arch/x86_64/interrupts.o \
	arch/x86_64/stage2.o arch/x86_64/descriptor_tables.o arch/x86_64/paging.o arch/x86_64/global.o arch/x86_64/acpi.o arch/x86_64/mm.o \
	arch/x86_64/video.o arch/x86_64/timer.o arch/x86_64/apic.o arch/x86_64/pci.o \
	arch/x86_64/multiboot2.o arch/x86_64/keyboard.o \
	libc/stdio.o libc/stdlib.o libc/threads.o libc++/new.o \
	kernel/vfs.o kernel/vfsops.o kernel/ELFLoader.o kernel/ELFFile.o kernel/sys_info.o kernel/multitask.o\
	DDI/driver.o DDI/ddi.o DDI/pci_driver.o DDI/block_driver.o\
	driver/ata.o driver/ramdrive.o driver/fatgen.o driver/binary_loader.o\
	driver/usb/uhci.o kernel/acpica_glue.o $(acpica_objects) \
	main.o cxxglue.o stack_protector.o

CC := clang-7
CXX := clang++-7
AS := nasm
LD := ld

all: kernel.elf

kernel.elf:$(OBJECT)
	$(LD) $(LDFLAGS) -o $@ $^
	objdump -x kernel.elf > objdump.txt
	objdump -d -M intel -S kernel.elf -j .text -j .text0> kernel.s
	
clean:
	# $(MAKE) -C tools clean
	$(RM) $(OBJECT)
	$(RM) kernel.elf

backup:
	tar -cf momentum.tar $(shell find -name '*.s') $(shell find -name '*.asm') $(shell find -name '*.c') $(shell find -name '*.h') $(shell find -name '*.cpp')
	
install:kernel.elf tools/hello_world.elf
	MTOOLS_SKIP_CHECK=1 mcopy -o -i momentum.raw@@1M kernel.elf ::kernel.elf
	MTOOLS_SKIP_CHECK=1 mcopy -o -i momentum.raw@@1M tools/hello_world.elf ::hello_world.elf

tools/%:
	$(MAKE) -C tools

	#analyze
	#scan-build-7 --use-cc=/usr/bin/clang-7 --use-c++=/usr/bin/clang++-7 --analyzer-target=x86_64-pc-none-elf make -j12