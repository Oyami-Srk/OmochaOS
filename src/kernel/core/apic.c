#include "core/config.h"

#if USE_APIC
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

u32 get_lapic_reg(void *base, u16 offset) { return *((u32 *)(base + offset)); }
u32 set_lapic_reg(void *base, u16 offset, u32 value) {
    return (*((u32 *)(base + offset)) = value);
}

void get_ioapic_ret(void *base, u8 index, u32 *lo, u32 *hi) {
    *((u8 *)base) = index;
    mfence();
    *lo = *((u32 *)base + 0x10);
    mfence();
    *((u8 *)base) = index + 1;
    mfence();
    *hi = *((u32 *)base + 0x10);
    mfence();
}

void set_ioapic_ret(void *base, u8 index, u32 lo, u32 hi) {
    *((u8 *)base) = index;
    mfence();
    *((u32 *)(base + 0x10)) = lo;
    mfence();
    *((u8 *)base) = index + 1;
    mfence();
    *((u32 *)(base + 0x10)) = hi;
    mfence();
}

void init_ioapic(struct core_env *env) {
    // set ioapic id to 0x0F00 0000
    void *ioapic_base    = env->base_ioapic;
    *((u8 *)ioapic_base) = 0x00;
    mfence();
    *((u32 *)(ioapic_base + 0x10)) = 0x0F000000;
    mfence();

    // get version of ioapic
    *((u8 *)ioapic_base) = 0x01;
    mfence();
    u32 ioapic_ver_reg = *((u32 *)(ioapic_base + 0x10));
    mfence();
    kprintf("IOAPIC Version: 0x%x, RTE count: %d\n", ioapic_ver_reg & 0xFF,
            ((ioapic_ver_reg >> 16) & 0xFF) + 1);

    // init ret start from INT 0x20
    for (uint i = 0x10; i < 0x40; i += 2) {
        set_ioapic_ret(ioapic_base, i, 0x10000 + 0x20 + ((i - 0x10) >> 1), 0);
    }

    // open ret int 0x21 for kbd
    set_ioapic_ret(ioapic_base, 0x12, 0x21, 0);
}

void init_lapic(struct core_env *env) {
    u32 hiaddr, loaddr;
    // enable lapic
    rdmsr(IA32_APIC_BASE_MSR, &loaddr, &hiaddr);
    if (env->lapic_feature & LAPIC_FEAT_X2APIC) {
        kprintf("enable x2APIC for supported device.\n");
        wrmsr(IA32_APIC_BASE_MSR, loaddr | 0xC00, hiaddr);
    } else
        wrmsr(IA32_APIC_BASE_MSR, loaddr | 0x800, hiaddr);

    BOOL is_bootcpu  = (loaddr & (1 << 8)) ? TRUE : FALSE;
    env->base_lapic  = (void *)(loaddr & 0xFFFFF000);
    env->base_ioapic = (void *)0xFEC00000;

    // enable svr[8] and svr[11]
    set_lapic_reg(env->base_lapic, 0xF0,
                  get_lapic_reg(env->base_lapic, 0xF0) | 0x1100);

    // mask all lvt
    set_lapic_reg(env->base_lapic, 0x2F0, 0x10000);
    set_lapic_reg(env->base_lapic, 0x320, 0x10000);
    set_lapic_reg(env->base_lapic, 0x330, 0x10000);
    set_lapic_reg(env->base_lapic, 0x340, 0x10000);
    set_lapic_reg(env->base_lapic, 0x350, 0x10000);
    set_lapic_reg(env->base_lapic, 0x360, 0x10000);
    set_lapic_reg(env->base_lapic, 0x370, 0x10000);

    kprintf("LAPIC MSR MEM at: %x\n", loaddr);
    kprintf("LAPIC ID: %x\n", get_lapic_reg(env->base_lapic, 0x20));
}

void init_apic(struct core_env *env) {
    // disable 8259 pic
    outb(IO_PIC_M + 1, 0xFF);
    outb(IO_PIC_S + 1, 0xFF);

    char cpu_vendor[13] = {[12] = 0};
    uint regs[4];
    regs[0] = 1;
    cpuid(regs);
    if (!(regs[3] & CPUID_FEAT_EDX_APIC))
        panic(Not support APIC);
    BOOL isx2apic_supported = regs[3] & CPUID_FEAT_ECX_x2APIC ? TRUE : FALSE;
    env->lapic_feature      = 0;
    if (isx2apic_supported)
        env->lapic_feature |= LAPIC_FEAT_X2APIC;

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

    // enable Local APIC
    init_lapic(env);
    // enable IOAPIC
    init_ioapic(env);
}
#endif