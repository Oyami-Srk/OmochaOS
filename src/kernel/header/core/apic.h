#ifndef __APIC_H__
#define __APIC_H__

#include "core/environment.h"
#include "generic/typedefs.h"

#define IOAPIC_MSR_MEM 0xFEC00000
#define LAPIC_MSR_MEM  0xFEE00000

void init_apic(struct core_env *env);
#define init_inthw init_apic
void end_interrupt(uint i);
void enable_interrupt(uint i);
void disable_interrupt(uint i);

// LAPIC OFFSET
#define LAPIC_OFFSET_ID  0x020
#define LAPIC_OFFSET_VER 0x030
#define LAPIC_OFFSET_EOI 0x0B0
#define LAPIC_OFFSET_SVR 0x0F0
#define LAPIC_OFFSET_ESR 0x280

#define LAPIC_OFFSET_TO_MSR(x) (0x800 + (((x)&0xFF0) >> 4))

#endif