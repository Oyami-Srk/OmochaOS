#include <driver/ahci/ahci.h>
#include <driver/driver.h>
#include <driver/pci.h>
#include <lib/stdlib.h>
#include <lib/string.h>
#include <core/paging.h>

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

int cmdslots;

static const AHCI_Device_Info AHCI_IntelDevices[] = {
    {0x2922, "ICH9 (AHCI mode)"},
    {0x2923, "ICH9 (AHCI mode)"},
    {0x2924, "ICH9"},
    {0x2925, "ICH9"},
    {0x2927, "ICH9"},
    {0x2829, "Intel ICH8M (AHCI mode)"},
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

//
#define SATA_SIG_ATA   0x00000101 // SATA drive
#define SATA_SIG_ATAPI 0xEB140101 // SATAPI drive
#define SATA_SIG_SEMB  0xC33C0101 // Enclosure management bridge
#define SATA_SIG_PM    0x96690101 // Port multiplier

#define AHCI_DEV_NULL   0
#define AHCI_DEV_SATA   1
#define AHCI_DEV_SEMB   2
#define AHCI_DEV_PM     3
#define AHCI_DEV_SATAPI 4

#define HBA_PORT_IPM_ACTIVE  1
#define HBA_PORT_DET_PRESENT 3

// Check device type
static int check_type(HBA_PORT *port) {
    uint32_t ssts = port->ssts;

    uint8_t ipm = (ssts >> 8) & 0x0F;
    uint8_t det = ssts & 0x0F;

    if (det != HBA_PORT_DET_PRESENT) // Check drive status
        return AHCI_DEV_NULL;
    if (ipm != HBA_PORT_IPM_ACTIVE)
        return AHCI_DEV_NULL;

    switch (port->sig) {
    case SATA_SIG_ATAPI:
        return AHCI_DEV_SATAPI;
    case SATA_SIG_SEMB:
        return AHCI_DEV_SEMB;
    case SATA_SIG_PM:
        return AHCI_DEV_PM;
    case SATA_SIG_ATA:
        return AHCI_DEV_SATA;
    default:
        return AHCI_DEV_NULL;
    }
}

void probe_port(HBA_MEM *abar) {
    // Search disk in implemented ports
    uint32_t pi = abar->pi;
    int      i  = 0;
    while (i < 32) {
        if (pi & 1) {
            int dt = check_type(&abar->ports[i]);
            if (dt == AHCI_DEV_SATA) {
                kprintf("SATA drive found at port %d\n", i);
            } else if (dt == AHCI_DEV_SATAPI) {
                kprintf("SATAPI drive found at port %d\n", i);
            } else if (dt == AHCI_DEV_SEMB) {
                kprintf("SEMB drive found at port %d\n", i);
            } else if (dt == AHCI_DEV_PM) {
                kprintf("PM drive found at port %d\n", i);
            } else {
                kprintf("No drive found at port %d\n", i);
            }
        }

        pi >>= 1;
        i++;
    }
}

#define AHCI_BASE 0x800000 // 8M

#define HBA_PxCMD_ST  0x0001
#define HBA_PxCMD_FRE 0x0010
#define HBA_PxCMD_FR  0x4000
#define HBA_PxCMD_CR  0x8000

// Start command engine
void start_cmd(HBA_PORT *port) {
    // Wait until CR (bit15) is cleared
    while (port->cmd & HBA_PxCMD_CR)
        ;

    // Set FRE (bit4) and ST (bit0)
    port->cmd |= HBA_PxCMD_FRE;
    port->cmd |= HBA_PxCMD_ST;
}

// Stop command engine
void stop_cmd(HBA_PORT *port) {
    // Clear ST (bit0)
    port->cmd &= ~HBA_PxCMD_ST;

    // Clear FRE (bit4)
    port->cmd &= ~HBA_PxCMD_FRE;

    // Wait until FR (bit14), CR (bit15) are cleared
    while (1) {
        if (port->cmd & HBA_PxCMD_FR)
            continue;
        if (port->cmd & HBA_PxCMD_CR)
            continue;
        break;
    }
}

void port_rebase(HBA_PORT *port, int portno) {
    stop_cmd(port); // Stop command engine

    // Command list offset: 1K*portno
    // Command list entry size = 32
    // Command list entry maxim count = 32
    // Command list maxim size = 32*32 = 1K per port
    port->clb  = AHCI_BASE + (portno << 10);
    port->clbu = 0;
    memset((void *)(port->clb), 0, 1024);

    // FIS offset: 32K+256*portno
    // FIS entry size = 256 bytes per port
    port->fb  = AHCI_BASE + (32 << 10) + (portno << 8);
    port->fbu = 0;
    memset((void *)(port->fb), 0, 256);

    // Command table offset: 40K + 8K*portno
    // Command table size = 256*32 = 8K per port
    HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER *)(port->clb);
    for (int i = 0; i < 32; i++) {
        cmdheader[i].prdtl = 8; // 8 prdt entries per command table
                                // 256 bytes per command table, 64+16+48+16*8
        // Command table offset: 40K + 8K*portno + cmdheader_index*256
        cmdheader[i].ctba  = AHCI_BASE + (40 << 10) + (portno << 13) + (i << 8);
        cmdheader[i].ctbau = 0;
        memset((void *)cmdheader[i].ctba, 0, 256);
    }

    start_cmd(port); // Start command engine
}

#define ATA_DEV_BUSY 0x80
#define ATA_DEV_DRQ  0x08

// Find a free command list slot
int find_cmdslot(HBA_PORT *port) {
    // If not set in SACT and CI, the slot is free
    uint32_t slots = (port->sact | port->ci);

    for (int i = 0; i < cmdslots; i++) {
        if ((slots & 1) == 0)
            return i;
        slots >>= 1;
    }
    kprintf("Cannot find free command list entry\n");
    return -1;
}

#define HBA_PxIS_TFES       (1 << 30) /* TFES - Task File Error Status */
#define ATA_CMD_READ_DMA_EX 0x25

BOOL read(HBA_PORT *port, uint32_t startl, uint32_t starth, uint32_t count,
          uint16_t *buf) {
    port->is = (uint32_t)-1; // Clear pending interrupt bits
    int spin = 0;            // Spin lock timeout counter
    int slot = find_cmdslot(port);
    if (slot == -1) {
        kprintf("Cannot find cmdslot!\n");
        return FALSE;
    }

    HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER *)port->clb;
    cmdheader += slot;
    cmdheader->cfl = sizeof(FIS_REG_H2D) / sizeof(uint32_t); // Command FIS size
    cmdheader->w   = 0;                                      // Read from device
    cmdheader->prdtl = (uint16_t)((count - 1) >> 4) + 1; // PRDT entries count

    HBA_CMD_TBL *cmdtbl = (HBA_CMD_TBL *)(cmdheader->ctba);
    memset(cmdtbl, 0,
           sizeof(HBA_CMD_TBL) +
               (cmdheader->prdtl - 1) * sizeof(HBA_PRDT_ENTRY));

    // 8K bytes (16 sectors) per PRDT
    int i;
    for (i = 0; i < cmdheader->prdtl - 1; i++) {
        cmdtbl->prdt_entry[i].dba = (uint32_t)buf;
        cmdtbl->prdt_entry[i].dbc =
            8 * 1024 - 1; // 8K bytes (this value should always be set to 1 less
                          // than the actual value)
        cmdtbl->prdt_entry[i].i = 1;
        buf += 4 * 1024; // 4K words
        count -= 16;     // 16 sectors
    }
    kprintf("[[%d]]", i);
    // Last entry
    cmdtbl->prdt_entry[i].dba = (uint32_t)buf;
    cmdtbl->prdt_entry[i].dbc = (count << 9) - 1; // 512 bytes per sector
    cmdtbl->prdt_entry[i].i   = 1;

    // Setup command
    FIS_REG_H2D *cmdfis = (FIS_REG_H2D *)(&cmdtbl->cfis);

    cmdfis->fis_type = FIS_TYPE_REG_H2D;
    cmdfis->c        = 1; // Command
    cmdfis->command  = ATA_CMD_READ_DMA_EX;

    cmdfis->lba0   = (uint8_t)startl;
    cmdfis->lba1   = (uint8_t)(startl >> 8);
    cmdfis->lba2   = (uint8_t)(startl >> 16);
    cmdfis->device = 1 << 6; // LBA mode

    cmdfis->lba3 = (uint8_t)(startl >> 24);
    cmdfis->lba4 = (uint8_t)starth;
    cmdfis->lba5 = (uint8_t)(starth >> 8);

    cmdfis->countl = count & 0xFF;
    cmdfis->counth = (count >> 8) & 0xFF;

    // The below loop waits until the port is no longer busy before issuing a
    // new command
    while ((port->tfd & (ATA_DEV_BUSY | ATA_DEV_DRQ)) && spin < 1000000) {
        spin++;
    }
    if (spin == 1000000) {
        kprintf("Port is hung\n");
        return FALSE;
    }

    port->ci = 1 << slot; // Issue command

    // Wait for completion
    while (1) {
        // In some longer duration reads, it may be helpful to spin on the DPS
        // bit in the PxIS port field as well (1 << 5)
        if ((port->ci & (1 << slot)) == 0)
            break;
        if (port->is & HBA_PxIS_TFES) // Task file error
        {
            kprintf("Read disk error\n");
            return FALSE;
        }
    }

    // Check again
    if (port->is & HBA_PxIS_TFES) {
        kprintf("Read disk error\n");
        return FALSE;
    }

    return TRUE;
}

//
HBA_MEM *hba;

int ahci_init(struct core_env *env) {
    kprintf("AHCI Initializing...\n");

    // Check PCI Buses
    u8          bus, slot;
    const char *vendor_name, *device_name;
    if (find_ahci_pci(&bus, &slot, &vendor_name, &device_name) != 0) {
        kprintf("AHCI Controller is not found.\n");
        return 1;
    }
    kprintf("ACHI Controller\"%s %s\" at bus %d slot %d.\n", vendor_name,
            device_name, bus, slot);
    u16 class_code = pci_config_read16(bus, slot, 0, 0x08 | 0x02);
    // note: 0x0106: 01 Massive Storage 06 SATA Controller
    kprintf("Class code is 0x%x, Subclass code is 0x%x\n", class_code >> 8,
            class_code & 0xFF);
    u32 abar = pci_config_read32(bus, slot, 0, 0x24);
    kprintf("ABAR is located at: 0x%x\n", abar);

    hba = (HBA_MEM *)abar;
    // map abar uncacheable

    env->page_dir[abar >> 22] =
        ((abar)&0xFFC00000) | PG_PCD | PG_Writable | PG_Present | PG_PS;
    // reload cr3
    asm volatile("movl %%cr3, %%eax\n\t"
                 "movl %%eax, %%cr3\n\t" ::
                     : "eax", "memory");

    kprintf("CAP: 0x%x, Ver: 0x%x, Pi: 0x%x\n", hba->cap, hba->vs, hba->pi);
    probe_port(hba);
    cmdslots = (hba->cap & 0x0f00) >> 8; // Bit 8-12
    kprintf("CMDSlots: %d; \n", cmdslots);

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