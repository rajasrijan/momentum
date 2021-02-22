BUILD_MODE ?= debug

CXXINCLUDE := -include new
TARGET := x86_64-pc-none-elf
CFLAGS :=
CXXFLAGS := 

CFLAGS += -g -O0
CFLAGS_32 += -g -O0
CXXFLAGS += -g -O0

CFLAGS += --target=$(TARGET) -m64 -Wshadow -Wwrite-strings  -D_MOMENTUM_ -fstack-protector-all -Wno-unused-function -Wredundant-decls -Wnested-externs -Winline -Wno-long-long -Woverflow --std=c11 -mno-red-zone\
	    -ffreestanding -std=c11  -mcmodel=large -masm=intel -I libc/include -I . -fno-function-sections -I acpica/include -mssse3

CFLAGS_32 += --target=$(TARGET) -m32 -Wshadow -Wwrite-strings  -D_MOMENTUM_ -Wno-unused-function -Wredundant-decls -Wnested-externs -Winline -Wno-long-long -Woverflow --std=c11 -mno-red-zone\
	    -ffreestanding -std=c11  -mcmodel=large -masm=intel -I libc/include -I . -fno-function-sections -I acpica/include

CXXFLAGS += --target=$(TARGET) -m64 -Wall -Wextra -Wunused-result -Wshadow -Wpointer-arith -fstack-protector-all -mno-red-zone -Wwrite-strings -Werror -Wno-unused-parameter -Wno-null-pointer-arithmetic -Wno-missing-braces -fno-exceptions -fno-rtti -Weffc++ -Wredundant-decls -Winline -Wno-long-long -Woverflow -mcmodel=large -masm=intel -ffreestanding -std=c++20  -D_MOMENTUM_ \
	    -I libc/ -I libc++/ -I .  -I acpica/include $(CXXINCLUDE) -fno-function-sections -mssse3

LDFLAGS:= -T x86_64.ld -z max-page-size=0x1000

ASFLAGS:=-f elf64

acpica_objects := $(patsubst %.c,%.o,$(wildcard acpica/*/*.c))

OBJECT := arch/x86_64/loader.o libc/string.o libc++/string.o arch/x86_64/arch_hal.o arch/x86_64/interrupts.o \
	arch/x86_64/stage2.o arch/x86_64/descriptor_tables.o arch/x86_64/paging.o arch/x86_64/global.o arch/x86_64/acpi.o arch/x86_64/mm.o \
	arch/x86_64/video.o arch/x86_64/timer.o arch/x86_64/apic.o arch/x86_64/pci.o arch/x86_64/font.o \
	arch/x86_64/multiboot2.o arch/x86_64/keyboard.o arch/x86_64/rtc.o libc/stdio.o libc/vsprintf.o libc/stdlib.o libc/threads.o libc++/new.o \
	kernel/vfs.o kernel/vfsops.o kernel/ELFLoader.o kernel/ELFFile.o kernel/sys_info.o kernel/multitask.o kernel/gui.o \
	DDI/driver.o DDI/ddi.o DDI/pci_driver.o DDI/block_driver.o driver/disk/ata.o driver/disk/sata.o driver/disk/ide.o driver/ramdrive.o driver/fatgen.o kernel/binary_loader.o \
	driver/ext/ext.o driver/ext/ext_vfs.o driver/ext/ext_vnode.o \
	driver/usb/uhci.o kernel/acpica_glue.o $(acpica_objects) main.o cxxglue.o stack_protector.o arch/x86_64/trampolin.blob.o kernel/logging.o

OBJECT_32 := arch/x86_64/bootstrap_functions.o

BLOBS := arch/x86_64/trampolin.bin

CC := clang
CXX := clang++
AS := nasm
LD := ld

all: kernel.elf
	$(MAKE) -C hosted_libc CC=$(CC)
	$(MAKE) -C tools CC=$(CC)

kernel.elf: $(OBJECT) $(OBJECT_32) x86_64.ld
	$(LD) $(LDFLAGS) -o $@ $(OBJECT) $(OBJECT_32)
	objdump -x kernel.elf > objdump.txt
	objdump -d -M intel -S kernel.elf -j .text -j .text0> kernel.s
	
clean:
	$(MAKE) -C hosted_libc clean
	$(MAKE) -C tools clean
	$(umount root_mnt)
	$(RM) -rf root_mnt
	$(umount drive_mnt)
	$(RM) -rf drive_mnt
	$(RM) -rf $(OBJECT) $(OBJECT_32) $(BLOBS)
	$(RM) kernel.elf kernel.s

backup:
	tar -cf momentum.tar $(shell find -name '*.s') $(shell find -name '*.asm') $(shell find -name '*.c') $(shell find -name '*.h') $(shell find -name '*.cpp')
	
install:kernel.elf tools/fusepart
	mkdir root_mnt
	mkdir drive_mnt
	tools/fusepart momentum.raw drive_mnt
	fuseext2 -o rw+ drive_mnt/disk_part0 root_mnt
	cp kernel.elf root_mnt/kernel.elf
	$(MAKE) -C hosted_libc install
	$(MAKE) -C tools install
	sync root_mnt
	umount root_mnt
	sync root_mnt
	sync drive_mnt
	umount drive_mnt
	sync drive_mnt
	$(RM) -rf root_mnt drive_mnt

tools/%:
	$(MAKE) -C tools

analyze:
	scan-build --use-cc=$(CC) --use-c++=$(CXX) --analyzer-target=$(TARGET) make -j12

.INTERMEDIATE:x86_64.ld

%.ld:%.ld.in
	$(CC) -I . -E -x c -P -o $@ $^

%.s:%.s.in
	$(CC) -I . -E -x c -P -o $@ $^

%.blob:%.s
	nasm -f bin -o $@ $^

%.blob.cpp:%.blob
	xxd -i $^ > $@

#	add target for 32bit files
$(OBJECT_32): %.o: %.c
	$(CC) $(CFLAGS_32) -c -o $@.tmp $^
	objcopy -O elf64-x86-64 $@.tmp $@
	$(RM) $@.tmp
