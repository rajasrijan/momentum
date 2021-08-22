/*
 * Copyright 2009-2021 Srijan Kumar Sharma
 *
 * This file is part of Momentum.
 *
 * Momentum is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Momentum is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Momentum.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CPU_H
#define CPU_H

#define MSR_IA32_EFER 0xC0000080
#define MSR_IA32_STAR 0xC0000081
#define MSR_IA32_LSTAR 0xC0000082

#ifdef __cplusplus
extern "C"
{
#endif

#define cpuid(in, a, b, c, d) __asm__ volatile("cpuid" : "=a"(a), "=b"(b), "=c"(c), "=d"(d) : "a"(in));

    static inline void rdmsr(uint32_t msr, uint64_t *val)
    {
        asm volatile("rdmsr" : "=a"(((uint32_t *)val)[0]), "=d"(((uint32_t *)val)[1]) : "c"(msr));
    }

    static inline void wrmsr(uint32_t msr, uint64_t val)
    {
        asm volatile("wrmsr" : : "a"(val & 0xFFFFFFFF), "d"((val >> 32) & 0xFFFFFFFF), "c"(msr));
    }

#ifdef __cplusplus
}
#endif

#endif /* CPU_H */
