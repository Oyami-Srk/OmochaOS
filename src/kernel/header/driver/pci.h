#ifndef __PCI_H__
#define __PCI_H__

#define PCI_CONF_ADDR 0xCF8
#define PCI_CONF_DATA 0xCFC

#include <generic/asm.h>
#include <generic/typedefs.h>

#include <driver/graphic.h>

#if ACPI

// offset & 2 == 0 chose the vendor (lower)
// offset & 2 != 0 chose the device (higher)
u16 pci_config_read16(u8 bus, u8 slot, u8 func, u8 offset);

// return all of register
u32 pci_config_read32(u8 bus, u8 slot, u8 func, u8 offset);

#else
#include <lib/stdlib.h>

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