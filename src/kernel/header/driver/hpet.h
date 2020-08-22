#ifndef __HPET_H__
#define __HPET_H__

#include <driver/acpi.h>

struct address_structure {
    u8 address_space_id; // 0 - system memory, 1 - system I/O
    u8 register_bit_width;
    u8 register_bit_offset;
    u8 reserved;
    // u64 addresss
    u32 address_lo;
    u32 address_hi;
} __attribute__((packed));

struct HPET {
    struct ACPISDTHeader     header;
    u8                       hardware_rev_id;
    u8                       comparator_count : 5;
    u8                       counter_size : 1;
    u8                       reserved : 1;
    u8                       legacy_replacement : 1;
    u16                      pci_vendor_id;
    struct address_structure address;
    u8                       hpet_number;
    u16                      minimumick;
    u8                       page_protection;
} __attribute__((packed));

void get_hpet_reg(void *base, u16 offset, u32 *lo, u32 *hi);
void set_hpet_reg(void *base, u16 offset, u32 lo, u32 hi);

#endif // __HPET_H__