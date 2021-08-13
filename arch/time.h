#ifndef TIME_H_COMMON
#define TIME_H_COMMON
#ifdef _arch_x86_64_
#include <arch/x86_64/timer.h>
#endif

uint64_t getSystemTime(void);
#endif // TIME_H_COMMON