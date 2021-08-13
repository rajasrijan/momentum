
#ifdef _arch_x86_64_
#include <arch/x86_64/paging.h>
#elif _arch_efi_
#include <arch/efi/paging.h>
#endif

int init_paging(void);