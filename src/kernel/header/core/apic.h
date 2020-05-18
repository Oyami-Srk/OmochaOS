#ifndef __APIC_H__
#define __APIC_H__

#include "core/environment.h"
#include "generic/typedefs.h"

#define IOAPIC_MSR_MEM 0xFEC00000
#define LAPIC_MSR_MEM  0xFEE00000

struct RSDPDescriptor {
    char Signature[8];
    u8   Checksum;
    char OEMID[6];
    u8   Revision;
    u32  RsdtAddress;
} __attribute__((packed));

struct RSDPDescriptor20 {
    struct RSDPDescriptor firstPart;

    u32 Length;
    u32 XsdtAddress[2];
    u8  ExtendedChecksum;
    u8  reserved[3];
} __attribute__((packed));

struct ACPISDTHeader {
    char Signature[4];
    u32  Length;
    u8   Revision;
    u8   Checksum;
    char OEMID[6];
    char OEMTableID[8];
    u32  OEMRevision;
    u32  CreatorID;
    u32  CreatorRevision;
};

struct RSDT {
    struct ACPISDTHeader h;
    // u32                  PointerToOtherSDT[(h.Length - sizeof(h)) / 4];
    void *table;
};

#define RSDT_TABLECOUNT(x)                                                     \
    ((((x)->Length) - sizeof(struct ACPISDTHeader)) / sizeof(u32))

void init_apic(struct core_env *env);

#endif