#ifndef __PCI_H__
#define __PCI_H__

#define PCI_CONF_ADDR 0xCF8
#define PCI_CONF_DATA 0xCFC

#include "generic/asm.h"
#include "generic/typedefs.h"

#if ACPI

// offset & 2 == 0 chose the vendor (lower)
// offset & 2 != 0 chose the device (higher)
static inline u32 pci_config_read16(u8 bus, u8 slot, u8 func, u8 offset) {
    u32 address = (bus << 16) | ((slot) << 11) | ((func) << 8) |
                  (offset & 0xFC) | 0x80000000;
    outl(PCI_CONF_ADDR, address);
    volatile u16 tmp = ((inl(PCI_CONF_DATA) >> ((offset & 2) * 8)) & 0xFFFF);
    return (tmp);
}

// return all of register
static inline u32 pci_config_read32(u8 bus, u8 slot, u8 func, u8 offset) {
    u32 address = (bus << 16) | ((slot) << 11) | ((func) << 8) |
                  (offset & 0xFC) | 0x80000000;
    outl(PCI_CONF_ADDR, address);
    return inl(PCI_CONF_DATA);
}

#else
#include "lib/stdlib.h"

static inline u32 pci_config_read16(u8 bus, u8 slot, u8 func, u8 offset) {
    panic(Enable ACPI to Use PCI);
    return 0;
}

static inline u32 pci_config_read32(u8 bus, u8 slot, u8 func, u8 offset) {
    panic(Enable ACPI to Use PCI);
    return 0;
}
#endif

#endif // __PCI_H__