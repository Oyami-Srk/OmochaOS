#ifndef __ACPI_H__
#define __ACPI_H__

#include "core/environment.h"
#include "generic/typedefs.h"

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
} __attribute__((packed));

struct RSDT {
    struct ACPISDTHeader h;
    // u32                  PointerToOtherSDT[(h.Length - sizeof(h)) / 4];
    u32 table[];
} __attribute__((packed));

#define RSDT_TABLECOUNT(x)                                                     \
    ((((x)->Length) - sizeof(struct ACPISDTHeader)) / sizeof(u32))

void                  init_acpi(struct core_env *env);
struct ACPISDTHeader *search_sdt(struct core_env *env, char *sig);

#endif // __ACPI_H__