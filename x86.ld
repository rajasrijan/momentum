ENTRY(start)

SECTIONS
{
	. = 0x100000;
        kernel_start = .;
	.stext :
	{
		*(.stext)
                *(.rodata*)
	}
        .sdata :
	{
		*(.sdata)
	}
	. += 0xC0000000;

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

	.bss : AT(ADDR(.bss) - 0xC0000000)
	{
                
		_sbss = .;
		*(COMMON)
		*(.bss)
		_ebss = .;
. = ALIGN(4096);
	}
	kernel_end = .;
}
