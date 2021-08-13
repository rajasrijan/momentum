#include <stdint.h>
#include <stddef.h>
#include <arch/time.h>

uint64_t getSystemTime()
{
    asm("cli;hlt");
    return 0;
}