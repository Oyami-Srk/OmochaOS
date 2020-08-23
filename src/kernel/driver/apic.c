#include <core/config.h>
#include <core/cpuid.h>
#include <core/environment.h>
#include <core/msr.h>
#include <driver/apic.h>
#include <generic/asm.h>
#include <lib/stdlib.h>
#include <lib/string.h>

#include <driver/graphic.h>

#if ACPI
#include <driver/acpi.h>
#include <driver/pci.h>
#endif
#include <driver/driver.h>
#include <driver/misc.h>

void *lapic_base;
void *ioapic_base;
BOOL  isx2apic = FALSE;

static inline u32 get_lapic_reg(void *base, u16 offset) {
    return *((volatile u32 *)(base + offset));
}
static inline void set_lapic_reg(void *base, u16 offset, u32 value) {
    *(volatile u32 *)(base + offset) = value;
}

static inline void get_ioapic_ret(void *base, u8 index, u32 *lo, u32 *hi) {
    *((u8 *)base) = index;
    mfence();
    *lo = *((u32 *)base + 0x10);
    mfence();
    *((u8 *)base) = index + 1;
    mfence();
    *hi = *((u32 *)base + 0x10);
    mfence();
}

static inline void set_ioapic_ret(void *base, u8 index, u32 lo, u32 hi) {
    // kprintf("Set IOAPIC RET index 0x%x = 0x%x 0x%x\n", index, hi, lo);
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
    void *ioapic_base = 0;
    // set ioapic id to 0x0F00 0000
    int acpi_driver_status = check_driver_exists("ACPI", 0, 1);
    if (acpi_driver_status == 0) {
        // use pci to locate rcba and ioapic base
        if (env->rcba) {
#define OIC_OFFSET 0x31F
            u16 *OIC = (u16 *)(env->rcba + OIC_OFFSET);
            kprintf("OIC Init Value: 0x%x\n", *OIC);
            *OIC |= 0x100;
            kprintf("OIC Set Value: 0x%x\n", *OIC);
        }

        struct MADT *madt = (struct MADT *)search_sdt(env, "APIC");
        if (madt) {
            struct MADT_Entry_Header *ph =
                (struct MADT_Entry_Header *)(&madt->table_start);
            kprintf(
                "MADT Addr: 0x%x, length: %d bytes,  MADT Defined LAPIC Addr: "
                "0x%x\n",
                madt, madt->header.Length, madt->lapic_addr);
            /*
    kprintf("Loop for MADT Table from 0x%x to 0x%x: \n", ph,
            ((void *)madt) + madt->header.Length);
            */
            for (; (void *)ph < ((void *)(madt)) + madt->header.Length;) {
                /*
                kprintf("| MADT Entry Type %d, length: %d bytes, at 0x%x\n",
                        ph->type, ph->length, ph);
                        */
                switch (ph->type) {
                case 0:
                    /*
                        kprintf("|-- Processor Local APIC: ACPI Processor ID
                       0x%x, " "APIC ID 0x%x, Flags 0x%x\n",
                                ph->data.type0.ACPI_Processor_ID,
                                ph->data.type0.APIC_ID, ph->data.type0.Flags);
                                */
                    break;
                case 1:
                    kprintf("|-- IO APIC: IOAPIC ID 0x%x, IOAPIC Addr 0x%x\n",
                            ph->data.type1.IOAPIC_ID,
                            ph->data.type1.IOAPIC_ADDR);
                    ioapic_base = (void *)ph->data.type1.IOAPIC_ADDR;
                    break;
                default:
                    break;
                }
                ph = ((void *)ph) + ph->length;
            }
            if (!ioapic_base)
                ioapic_base = (void *)0xFEC00000;
        } else {
            ioapic_base = (void *)0xFEC00000;
        }

    } else {
        ioapic_base = (void *)0xFEC00000;
    }
    env->base_ioapic = (void *)ioapic_base;

    *((u8 *)ioapic_base) = 0x00;
    mfence();
    *((u32 *)(ioapic_base + 0x10)) = 0x0F000000; // ioapic id
    mfence();

    // get version of ioapic
    *((u8 *)ioapic_base) = 0x01;
    mfence();
    u32 ioapic_ver_reg = *((u32 *)(ioapic_base + 0x10));
    mfence();
    *((u8 *)ioapic_base) = 0x00;
    mfence();
    kprintf("IOAPIC Version: 0x%x, RTE count: %d, ID: 0x%x\n",
            ioapic_ver_reg & 0xFF, ((ioapic_ver_reg >> 16) & 0xFF) + 1,
            (*((u32 *)(ioapic_base + 0x10)) >> 24));

    // init ret start from INT 0x20(IRQ0) to 0x40(IRQ32)
    for (uint i = 0x10; i < 0x40; i += 2) {
        // set_ioapic_ret(ioapic_base, i, 0x10000 + 0x20 + ((i - 0x10) >> 1),
        // 0);
        set_ioapic_ret(ioapic_base, i, 0x20 + ((i - 0x10) >> 1), 0);
    }
    // enable_interrupt(2);
    // enable_interrupt(1);

    kprintf("IOAPIC Initialized\n");
}

void init_lapic(struct core_env *env) {
    u32 hiaddr, loaddr;
    // enable lapic
    rdmsr(IA32_APIC_BASE_MSR, &loaddr, &hiaddr);
    if (env->lapic_feature & LAPIC_FEAT_X2APIC) {
        kprintf("enable x2APIC for supported device.\n", env->lapic_feature);
        wrmsr(IA32_APIC_BASE_MSR, loaddr | 0xC00, hiaddr);
        isx2apic = TRUE;
    } else
        wrmsr(IA32_APIC_BASE_MSR, loaddr | 0x800, hiaddr);

    BOOL is_bootcpu = (loaddr & (1 << 8)) ? TRUE : FALSE;
    env->base_lapic = (void *)(loaddr & 0xFFFFF000);
    lapic_base      = env->base_lapic;
    u32 verreg      = 0;
    if (!isx2apic) {
        verreg = get_lapic_reg(lapic_base, LAPIC_OFFSET_VER);
    } else {
        u32 gap;
        rdmsr(LAPIC_OFFSET_TO_MSR(LAPIC_OFFSET_VER), &verreg, &gap);
    }
    kprintf("Local APIC Version register: 0x%x\n", verreg);

    if (!isx2apic) {
        // enable svr[8] (and svr[12] if possible)
        if (verreg & (1 << 24))
            set_lapic_reg(lapic_base, LAPIC_OFFSET_SVR,
                          get_lapic_reg(lapic_base, LAPIC_OFFSET_SVR) | 0x1100);
        else
            set_lapic_reg(lapic_base, LAPIC_OFFSET_SVR,
                          get_lapic_reg(lapic_base, LAPIC_OFFSET_SVR) | 0x100);

        // mask all lvt
        set_lapic_reg(lapic_base, 0x2F0, 0x10000);
        set_lapic_reg(lapic_base, 0x320, 0x10000);
        set_lapic_reg(lapic_base, 0x330, 0x10000);
        set_lapic_reg(lapic_base, 0x340, 0x10000);
        set_lapic_reg(lapic_base, 0x350, 0x10000);
        set_lapic_reg(lapic_base, 0x360, 0x10000);
        set_lapic_reg(lapic_base, 0x370, 0x10000);
    } else {
        // enable svr[8] (and svr[12] if possible)
        u32 hi, lo;
        rdmsr(LAPIC_OFFSET_TO_MSR(LAPIC_OFFSET_SVR), &lo, &hi);
        if (verreg & (1 << 24))
            wrmsr(LAPIC_OFFSET_TO_MSR(LAPIC_OFFSET_SVR), lo | 0x1100, hi);
        else
            wrmsr(LAPIC_OFFSET_TO_MSR(LAPIC_OFFSET_SVR), lo | 0x100, hi);

        // mask all lvt
        wrmsr(LAPIC_OFFSET_TO_MSR(0x2F0), 0x10000, 0);
        wrmsr(LAPIC_OFFSET_TO_MSR(0x320), 0x10000, 0);
        wrmsr(LAPIC_OFFSET_TO_MSR(0x330), 0x10000, 0);
        wrmsr(LAPIC_OFFSET_TO_MSR(0x340), 0x10000, 0);
        wrmsr(LAPIC_OFFSET_TO_MSR(0x350), 0x10000, 0);
        wrmsr(LAPIC_OFFSET_TO_MSR(0x360), 0x10000, 0);
        wrmsr(LAPIC_OFFSET_TO_MSR(0x370), 0x10000, 0);
    }

    kprintf("LAPIC MSR MEM at: %x\n", loaddr);
    kprintf("LAPIC ID: %x\n", get_lapic_reg(lapic_base, 0x20));
}

/*
#if ACPI
#include <driver/hpet.h>
#endif

void init_timer(struct core_env *env) {
#if ACPI
    if (init_hpet(env))
        return;
#endif
    // init PIT
    init_8253();
}
*/

void __apic__end_interrupt(uint i) {
    if (!isx2apic)
        set_lapic_reg(lapic_base, LAPIC_OFFSET_EOI, 0x0);
    else
        wrmsr(LAPIC_OFFSET_TO_MSR(LAPIC_OFFSET_EOI), 0x00, 0x00);
}
void __apic__enable_interrupt(uint i) {
    // 0:7 = 0x20 + i    <- IVT number
    // 8:10 = 0          <- Fixed delivery mode
    // 11 = 0            <- Phy Destination mode
    // 12 = 0            <- RO delivery status
    // 16 = 0            <- disable mask
    set_ioapic_ret(ioapic_base, 0x10 + i * 2, IRQ0 + i, 0);
}
void __apic__disable_interrupt(uint i) {
    // 16 = 0            <- enable mask
    set_ioapic_ret(ioapic_base, 0x10 + i * 2, 0x10000 | (IRQ0 + i), 0);
}

int init_apic(struct core_env *env) {
    // init 8259A to remap interrupt (the same as 8259a.c)
    outb(IO_PIC_M, 0x11); // ICW 1
    io_wait();
    outb(IO_PIC_S, 0x11); // ICW 1
    io_wait();
    outb(IO_PIC_M + 1, 0x20); // 0x20 -> Master first
    io_wait();
    outb(IO_PIC_S + 1, 0x28); // 0x28 -> Salve first
    io_wait();

    outb(IO_PIC_M + 1, 0x4); // ICW 3
    io_wait();
    outb(IO_PIC_S + 1, 0x2); // ICW 3
    io_wait();
    outb(IO_PIC_M + 1, 0x1);
    io_wait();
    outb(IO_PIC_S + 1, 0x1);
    io_wait();

    // disable 8259 pic by mask all interrupt
    outb(IO_PIC_M + 1, 0xFF);
    outb(IO_PIC_S + 1, 0xFF);

    char cpu_vendor[13] = {[12] = 0};
    uint regs[4];
    regs[0] = 1;
    cpuid(regs);
    if (!(regs[3] & CPUID_FEAT_EDX_APIC))
        panic(Not support APIC);
    if (!(regs[3] & CPUID_FEAT_EDX_MSR))
        panic(Not support MSR);
    BOOL isx2apic_supported = regs[2] & CPUID_FEAT_ECX_x2APIC ? TRUE : FALSE;
    kprintf("CPUID 1: 0x%x 0x%x 0x%x 0x%x\n", regs[0], regs[1], regs[2],
            regs[3]);
    env->lapic_feature = 0;
    if (isx2apic_supported)
        env->lapic_feature |= LAPIC_FEAT_X2APIC;

    regs[0] = 0;
    cpuid(regs);
    ((u32 *)cpu_vendor)[0] = regs[1];
    ((u32 *)cpu_vendor)[1] = regs[3];
    ((u32 *)cpu_vendor)[2] = regs[2];
    kprintf("CPU Vendor: %s\n", cpu_vendor);

    // enable Local APIC
    init_lapic(env);
    // enable IOAPIC
    init_ioapic(env);
    // init timer
    // init_timer(env);
    env->end_interrupt     = __apic__end_interrupt;
    env->enable_interrupt  = __apic__enable_interrupt;
    env->disable_interrupt = __apic__disable_interrupt;
    return 0;
}

static Driver_Declaration driver_apic = {.magic       = DRIVER_DC,
                                         .name        = "APIC",
                                         .major_ver   = 0,
                                         .minor_ver   = 1,
                                         .level       = 2,
                                         .init        = init_apic,
                                         .initialized = FALSE};

ADD_DRIVER(driver_apic);