#ifndef __MSR_H__
#define __MSR_H__

#include "generic/typedefs.h"

#define IA32_APIC_BASE_MSR 0x1B

void rdmsr(u32 msr, u32 *lo, u32 *hi) {
    asm volatile("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
}

void wrmsr(u32 msr, u32 lo, u32 hi) {
    asm volatile("wrmsr" : : "a"(lo), "d"(hi), "c"(msr));
}

#endif