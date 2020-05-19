#ifndef __MSR_H__
#define __MSR_H__

#include "generic/typedefs.h"

#define IA32_APIC_BASE_MSR 0x1B

static inline void rdmsr(u32 msr, u32 *lo, u32 *hi) {
    asm volatile("rdmsr\n\t" : "=a"(*lo), "=d"(*hi) : "c"(msr));
}

static inline void wrmsr(u32 msr, u32 lo, u32 hi) {
    asm volatile("wrmsr\n\t" : : "a"(lo), "d"(hi), "c"(msr));
}

#endif