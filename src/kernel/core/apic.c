#include "core/apic.h"
#include "core/cpuid.h"
#include "core/environment.h"
#include "core/msr.h"
#include "generic/asm.h"
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
    memset(buf, 0, 16);
    memcpy(buf, pRSDT->h.OEMTableID, 8);
    kprintf("  Total %d tables. OEMTableID is: %s\n", table_count, buf);
    kprintf("  RSDT at 0x%x, Table start at 0x%x\n", pRSDT, pRSDT->table);
    void *p = pRSDT->table;
    for (uint i = 0; i < table_count; i++) {
        struct ACPISDTHeader *ph = p;
        if (sdt_checksum(p) == 0) {
            char sig_buf[5] = {[4] = 0};
            memcpy(sig_buf, ph->Signature, 4);
            kprintf("  %s, Length: %d bytes\n", sig_buf, ph->Length);
        } else {
            kprintf("  Checksum failed at 0x%x\n", p);
        }
        p += ph->Length;
    }
    kprintf("=======RSDT ENDForeach=======\n");
}

u32 get_lapic_reg(void *base, u8 offset) { return *((u32 *)(base + offset)); }
u32 set_lapic_reg(void *base, u8 offset, u32 value) {
    return (*((u32 *)(base + offset)) = value);
}

void init_apic(struct core_env *env) {
    char cpu_vendor[13] = {[12] = 0};
    uint regs[4];
    regs[0] = 1;
    cpuid(regs);
    if (!(regs[3] & CPUID_FEAT_EDX_APIC))
        panic(Not support APIC);
    regs[0] = 0;
    cpuid(regs);
    ((u32 *)cpu_vendor)[0] = regs[1];
    ((u32 *)cpu_vendor)[1] = regs[3];
    ((u32 *)cpu_vendor)[2] = regs[2];
    kprintf("CPU Vendor: %s\nNow Searching rsdp...", cpu_vendor);
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

    u32 hiaddr, loaddr;
    rdmsr(IA32_APIC_BASE_MSR, &loaddr, &hiaddr);
    loaddr &= 0xFFFFF000;
    env->base_lapic  = (void *)loaddr;
    env->base_ioapic = (void *)0xFEC00000;

    kprintf("LAPIC MSR MEM at: %x\n", loaddr);
    kprintf("Local APIC: %x\n", *((u32 *)(0xFEE00030)));

    // disable 8259 pic
    outb(IO_PIC_M + 1, 0xFF);
    outb(IO_PIC_S + 1, 0xFF);
}