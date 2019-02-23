ENTRY(loader)
TARGET(elf64-x86-64)

SECTIONS
{
	. = 0x100000;
	kernel_start = . + 0xC0000000;
	.text0 ALIGN (0x1000) : AT(ADDR(.text0))
	{
		*(.multiboot)
		*(.text0*)
	}
	. += 0xC0000000;
	.text ALIGN (0x1000) : AT(ADDR(.text) - 0xC0000000)
	{
		*(.text*)
		*(.rodata*)
		. = ALIGN(4096);
	}
	
	
	.data ALIGN (0x1000) : AT(ADDR(.data) - 0xC0000000)
	{
		*(.data)
        . = ALIGN(4096);
	}
	__MOD_LIST__ = .;
    .mod_init ALIGN (0x1000) : AT(ADDR(.mod_init) - 0xC0000000)
	{
		*(.mod_init)
	}
	__MOD_END__ = .;
    .ctors ALIGN (0x1000) : AT(ADDR(.ctors) - 0xC0000000)
	{
		__CTOR_LIST__ = .;
		*(.ctors)
		*(.init_array)
	}
    __CTOR_END__ = .;
    .dtors ALIGN (0x1000) : AT(ADDR(.dtors) - 0xC0000000)
	{
		*(.dtors)
		*(.fini_array)
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
