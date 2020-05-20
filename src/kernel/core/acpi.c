#include "core/acpi.h"
#include "lib/stdlib.h"
#include "lib/string.h"

#include "driver/graphic.h"

static int rsdp_checksum(char *p) {
    u8 sum = 0;
    for (uint i = 0; i < sizeof(struct RSDPDescriptor); i++)
        sum += p[i];
    return sum;
}

static struct RSDPDescriptor *search_rsdp(char *start, size_t max_offset) {
    const char   rsdp_sig[] = "RSD PTR ";
    const size_t rsdp_bound = 16;
    char *       p          = start;
    for (; p < start + max_offset; p += rsdp_bound) {
        if (memcmp(p, rsdp_sig, 8) == 0 && rsdp_checksum(p) == 0)
            return (struct RSDPDescriptor *)p;
    }
    return NULL;
}

static int sdt_checksum(struct ACPISDTHeader *p) {
    u8 sum = 0;
    for (uint i = 0; i < p->Length; i++)
        sum += ((char *)p)[i];
    return sum;
}

static void foreach_rsdt(struct RSDT *pRSDT) {
    kprintf("=======  RSDT Foreach =======\n");
    size_t table_count = RSDT_TABLECOUNT((struct ACPISDTHeader *)pRSDT);
    char   buf[16];
    if (memcmp("RSDT", pRSDT->h.Signature, 4) != 0 ||
        sdt_checksum((struct ACPISDTHeader *)pRSDT) != 0)
        panic(Not a valid RSDT);
    memset(buf, 0, 16);
    memcpy(buf, pRSDT->h.OEMTableID, 8);
    kprintf("  RSDT Length: %d bytes, sizeof ACPISDTHeader is %d bytes\n",
            pRSDT->h.Length, sizeof(struct ACPISDTHeader));
    kprintf("  Total %d tables. OEMTableID is: %s\n", table_count, buf);
    kprintf("  RSDT at 0x%x, Table at 0x%x\n", pRSDT, pRSDT->table);
    for (uint i = 0; i < table_count; i++) {
        kprintf("  Searching for %d item of table, at 0x%x:", i,
                pRSDT->table[i]);
        struct ACPISDTHeader *ph = (struct ACPISDTHeader *)(pRSDT->table[i]);
        if (sdt_checksum(ph) == 0) {
            char sig_buf[5] = {[4] = 0};
            memcpy(sig_buf, ph->Signature, 4);
            kprintf("  %s, Length: %d bytes\n", sig_buf, ph->Length);
        } else {
            kprintf("  Checksum failed at 0x%x\n", ph);
        }
    }
    kprintf("=======RSDT ENDForeach=======\n");
}

struct ACPISDTHeader *search_sdt(struct core_env *env, char *sig) {
    struct RSDT *pRSDT       = env->rsdt;
    size_t       table_count = RSDT_TABLECOUNT((struct ACPISDTHeader *)pRSDT);
    char         buf[16];
    if (memcmp("RSDT", pRSDT->h.Signature, 4) != 0 ||
        sdt_checksum((struct ACPISDTHeader *)pRSDT) != 0)
        panic(Not a valid RSDT);
    memset(buf, 0, 16);
    memcpy(buf, pRSDT->h.OEMTableID, 8);
    for (uint i = 0; i < table_count; i++) {
        struct ACPISDTHeader *ph = (struct ACPISDTHeader *)(pRSDT->table[i]);
        if (sdt_checksum(ph) == 0 && memcmp(sig, ph->Signature, 4) == 0) {
            return ph;
        }
    }
    return NULL;
}

void init_acpi(struct core_env *env) {
    struct RSDPDescriptor *rsdp =
        search_rsdp((char *)0xe0000, 0xfffff - 0xe0000);
    if (!rsdp)
        rsdp = search_rsdp((char *)0x40E, 0x1000);
    if (!rsdp)
        panic(Cannot find RSDP);
    char buf[16];
    memset(buf, 0, 16);
    memcpy(buf, rsdp->OEMID, 6);
    kprintf("Found rsdp at 0x%x, version: %d, OEM: %s\n", rsdp, rsdp->Revision,
            buf);
    foreach_rsdt((struct RSDT *)rsdp->RsdtAddress);
    env->rsdt         = (struct RSDT *)rsdp->RsdtAddress;
    env->ACPI_version = rsdp->Revision;
}