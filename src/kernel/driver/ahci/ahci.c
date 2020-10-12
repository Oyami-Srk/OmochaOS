#include <driver/ahci/ahci.h>
#include <driver/driver.h>
#include <driver/pci.h>
#include <lib/stdlib.h>

#include <driver/graphic.h>

/*
static const AHCI_Device_Info KnownDevices[] = {
    {0x8086, 0x2922, "Intel ICH9 (AHCI mode)"},
    {0x8086, 0x2923, "Intel ICH9 (AHCI mode)"},
    {0x8086, 0x2924, "Intel ICH9"},
    {0x8086, 0x2925, "Intel ICH9"},
    {0x8086, 0x2927, "Intel ICH9"},
    {NULL, NULL, NULL},
};
*/

static const AHCI_Vendor_Info AHCI_IntelDevices[] = {
    {0x2922, "ICH9 (AHCI mode)"},
    {0x2923, "ICH9 (AHCI mode)"},
    {0x2924, "ICH9"},
    {0x2925, "ICH9"},
    {0x2927, "ICH9"},
    {},
};

static const AHCI_Vendor_Info AHCI_KnownVendors[] = {
    {0x8086, "Intel", (AHCI_Device_Info *)AHCI_IntelDevices},
    {},
};

// 0 is found, otherwise is not found
int find_ahci_pci(u8 *bus, u8 *slot, const char **vendor_name,
                  const char **device_name) {
    u16 vendor, device;
    for (int _bus = 0; _bus < 256; _bus++) {
        for (int _slot = 0; _slot < 32; _slot++) {
            vendor = pci_config_read16(_bus, _slot, 0, (0x00 | 0x0));
            for (AHCI_Vendor_Info *v = (AHCI_Vendor_Info *)AHCI_KnownVendors;
                 v->vendor; v++) {
                if (v->vendor == vendor) {
                    device = pci_config_read16(_bus, _slot, 0, (0x00 | 0x02));
                    for (AHCI_Device_Info *d = v->devices; d->device; d++) {
                        if (d->device == device) {
                            *bus         = _bus;
                            *slot        = _slot;
                            *vendor_name = v->vendor_name;
                            *device_name = d->device_name;
                            return 0;
                        }
                    }
                }
            }
        }
    }
    return 1;
}

int ahci_init(struct core_env *env) {
    kprintf("AHCI Initializing...\n");

    u8          bus, slot;
    const char *vendor_name, *device_name;
    if (find_ahci_pci(&bus, &slot, &vendor_name, &device_name) != 0) {
        kprintf("AHCI Controller is not found.\n");
        return 1;
    }
    kprintf("ACHI Controller\"%s %s\" at bus %d slot %d.\n", vendor_name,
            device_name, bus, slot);
    u16 class_code = pci_config_read16(bus, slot, 0, 0x08 | 0x02);
    kprintf("Class code is 0x%x, Subclass code is 0x%x\n", class_code >> 8,
            class_code & 0xFF);

    while (1)
        ;
    return 0;
}

static Driver_Declaration driver_ahci = {.magic       = DRIVER_DC,
                                         .name        = "AHCI",
                                         .major_ver   = 0,
                                         .minor_ver   = 1,
                                         .level       = 3,
                                         .init        = ahci_init,
                                         .initialized = FALSE};

ADD_DRIVER(driver_ahci);