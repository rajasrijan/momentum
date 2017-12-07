ENTRY(loader)
OUTPUT_FORMAT(elf32-i386)
SECTIONS
{
	. += 0xC0400000;
	kernel_start = .;
	.text : AT(ADDR(.text) - 0xC0000000)
	{
		*(.text)
		*(.rodata*)
		. = ALIGN(4096);
	}
	.data ALIGN (0x1000) : AT(ADDR(.data) - 0xC0000000)
	{
		*(.data)
                . = ALIGN(4096);
	}
        __CTOR_LIST__ = .;
        .ctors ALIGN (0x1000) : AT(ADDR(.ctors) - 0xC0000000)
	{
		*(.ctors)
	}
        __CTOR_END__ = .;
        .dtors ALIGN (0x1000) : AT(ADDR(.dtors) - 0xC0000000)
	{
		*(.dtors)
	}
	.bss : AT(ADDR(.bss) - 0xC0000000)
	{
		_sbss = .;
		*(COMMON)
		*(.bss)
		_ebss = .;
		. = ALIGN(4096);
	}
	.eh_frame ALIGN (0x1000) : AT(ADDR(.eh_frame) - 0xC0000000)
	{
		*(.eh_frame)
		. = ALIGN(4096);
	}
	kernel_end = .;
}
