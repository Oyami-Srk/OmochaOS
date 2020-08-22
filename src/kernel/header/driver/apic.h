#ifndef __APIC_H__
#define __APIC_H__

#include <core/environment.h>
#include <generic/typedefs.h>

#define IOAPIC_MSR_MEM 0xFEC00000
#define LAPIC_MSR_MEM  0xFEE00000

#include <driver/acpi.h>

struct MADT_Entry_Header {
    u8 type;
    u8 length;
    union {
        struct {
            u8  ACPI_Processor_ID;
            u8  APIC_ID;
            u32 Flags; // bit0: processor enabled, bit1: online capable
        } __attribute__((packed)) type0; // local apic
        struct {
            u8  IOAPIC_ID;
            u8  resv;
            u32 IOAPIC_ADDR;
            u32 GlobalSysInterruptBase;
        } __attribute__((packed)) type1; // io apic
        struct {
            u8  BusSource;
            u8  IRQSource;
            u32 GlobalSysInterrupt;
            u16 Flags;
        } __attribute__((packed)) type2; // interrupt source override
        struct {
            u8  ACPI_Processor_ID; // 0xFF means all
            u16 Flags;
            u8  LINT_Num;                // 0 or 1
        } __attribute__((packed)) type4; // nmi
        struct {
            u16 resv;
            u32 APIC_phyaddr_override_lo;
            u32 APIC_phyaddr_override_hi;
        } __attribute__((packed)) type5; // Local APIC Address Override
    } data;
} __attribute__((packed));

struct MADT {
    struct ACPISDTHeader header;
    u32                  lapic_addr;
    u32                  flags;
    u32                  table_start;
} __attribute__((packed));

// void init_apic(struct core_env *env);
// #define init_inthw init_apic
// void end_interrupt(uint i);
// void enable_interrupt(uint i);
// void disable_interrupt(uint i);

// LAPIC OFFSET
#define LAPIC_OFFSET_ID  0x020
#define LAPIC_OFFSET_VER 0x030
#define LAPIC_OFFSET_EOI 0x0B0
#define LAPIC_OFFSET_SVR 0x0F0
#define LAPIC_OFFSET_ESR 0x280

#define LAPIC_OFFSET_TO_MSR(x) (0x800 + (((x)&0xFF0) >> 4))

#endif