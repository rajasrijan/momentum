#ifndef _CONFIG_H_
#define _CONFIG_H_

#ifndef KERNEL_BASE_PTR
#define KERNEL_BASE_PTR (0x000000C000000000)
#endif // KERNEL_BASE_PTR

#if KERNEL_BASE_PTR < 0x100000
#error Kernel has to be above 1MB in RAM.
#endif

#if (KERNEL_BASE_PTR & 0xFFFF000000000000) != 0
#error kernel base pointer cannot be beyond mappable region. Change the value of KERNEL_BASE_PTR.
#endif

#if KERNEL_BASE_PTR < 0x100000000
#warning placing kernel lower than 4GB mark can lead to collision with memory mapped IO.
#endif

#ifndef KERNEL_HEAP_PTR
#define KERNEL_HEAP_PTR (0x00000000D0000000)
#endif // KERNEL_HEAP_PTR

#ifndef KERNEL_MAXIMUM_PHYSICAL_RAM
//  256GB
#define KERNEL_MAXIMUM_PHYSICAL_RAM (0x0000004000000000)
#endif // KERNEL_MAXIMUM_PHYSICAL_RAM

#ifndef KERNEL_MINIMUM_PHYSICAL_RAM
//  128MB
#define KERNEL_MINIMUM_PHYSICAL_RAM (0x0000000008000000)
#endif // KERNEL_MINIMUM_PHYSICAL_RAM

#ifndef KERNEL_TEMP_PAGE_TABLE_LOCATION
//  At 64MB.
//  Bug: collision risk with memory mapped io.
#define KERNEL_TEMP_PAGE_TABLE_LOCATION (0x0000000004000000)
#endif // KERNEL_TEMP_PAGE_TABLE_LOCATION

#ifndef KERNEL_GRUB_VIDEO
#define KERNEL_GRUB_VIDEO (1)
#endif

#endif //_CONFIG_H_
