#include <driver/pci.h>

#include <driver/driver.h>
#include <driver/graphic.h>
#include <lib/stdlib.h>

// offset & 2 == 0 chose the vendor (lower)
// offset & 2 != 0 chose the device (higher)
u16 pci_config_read16(u8 bus, u8 slot, u8 func, u8 offset) {
    u32 address = (bus << 16) | ((slot) << 11) | ((func) << 8) |
                  (offset & 0xFC) | 0x80000000;
    // kprintf("Read PCI config from 0x%x\n", address);
    outl(PCI_CONF_ADDR, address);
    io_wait();
    volatile u16 tmp = ((inl(PCI_CONF_DATA) >> ((offset & 2) * 8)) & 0xFFFF);
    return (tmp);
}

// return all of register
u32 pci_config_read32(u8 bus, u8 slot, u8 func, u8 offset) {
    u32 address = (bus << 16) | ((slot) << 11) | ((func) << 8) |
                  (offset & 0xFC) | 0x80000000;
    // kprintf("Read PCI config from 0x%x\n", address);
    outl(PCI_CONF_ADDR, address);
    io_wait();
    return inl(PCI_CONF_DATA);
}

static Driver_Declaration driver_pci = {.magic       = DRIVER_DC,
                                        .name        = "PCI",
                                        .major_ver   = 0,
                                        .minor_ver   = 1,
                                        .level       = 3,
                                        .init        = NULL,
                                        .initialized = FALSE};

ADD_DRIVER(driver_pci);