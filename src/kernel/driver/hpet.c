#include <driver/acpi.h>
#include <driver/hpet.h>
#include <driver/misc.h>
#include <generic/asm.h>

#include <driver/driver.h>
#include <driver/graphic.h>
#include <lib/stdlib.h>

void *hpet_base;

// from linux
typedef unsigned long long u64;
typedef long long          i64;
static u32                 divide_64_by_32(u64 *n, u32 base) {
    u64 rem = *n;
    u64 b   = base;
    u64 res, d = 1;
    u32 high = rem >> 32;

    /* Reduce the thing a bit first */
    res = 0;
    if (high >= base) {
        high /= base;
        res = (u64)high << 32;
        rem -= (u64)(high * base) << 32;
    }

    while ((i64)b > 0 && b < rem) {
        b = b + b;
        d = d + d;
    }

    do {
        if (rem >= b) {
            rem -= b;
            res += d;
        }
        b >>= 1;
        d >>= 1;
    } while (d);

    *n = res;
    return rem;
}

void get_hpet_reg(void *base, u16 offset, u32 *lo, u32 *hi) {
    u32 addr = (u32)base + offset;
    *lo      = *((volatile u32 *)addr);
    mfence();
    *hi = *((volatile u32 *)(addr + 0x4));
    mfence();
}

void set_hpet_reg(void *base, u16 offset, u32 lo, u32 hi) {
    u32 addr = (u32)base + offset;
    // kprintf("[HPET] Set 0x%x = 0x%x 0x%x\n", addr, hi, lo);
    *((volatile u32 *)(addr)) = lo;
    mfence();
    *((volatile u32 *)(addr + 0x4)) = hi;
    mfence();
}

int init_hpet(struct core_env *env) {
    kprintf("Initializing HPET\n");
    struct HPET *hpet = (struct HPET *)search_sdt(env, "HPET");
    if (!hpet)
        return 1;
    kprintf("HPET rev_id: %d, address: 0x%x, vendor_id: 0x%x\n",
            hpet->hardware_rev_id, hpet->address.address_lo,
            hpet->pci_vendor_id);
    hpet_base = (void *)hpet->address.address_lo;
    // use HPTC if RCBA uasble
    if (env->rcba) {
#define HPTC_OFFSET 0x3404
        u32 *hptc = (u32 *)(env->rcba + HPTC_OFFSET);
        kprintf("HPTC value: 0x%x\n", *hptc);
        u8 hpet_map = (((u32)hpet_base) & 0xF000) >> 12;
        *hptc       = 0x80 | (hpet_map);
        kprintf("HPTC value set: 0x%x\n", *hptc);
    }

    u32 general_cap_reg_lo;
    u32 general_cap_reg_hi; // preics
    get_hpet_reg(hpet_base, 0x0, &general_cap_reg_lo, &general_cap_reg_hi);
    kprintf("HPET general_cap_reg: 0x%x 0x%x\n", general_cap_reg_hi,
            general_cap_reg_lo);

    // disable 8253
// disable_8253();
#define I8253_CTR_PORT 0x43
#define I8253_CNL0     0x40
    outb(I8253_CTR_PORT, 0x32); // one-shot
    outb(I8253_CNL0, 0);
    outb(I8253_CNL0, 0);

    set_hpet_reg(hpet_base, 0x10, 0x0, 0); // disable if BIOS has enabled it
    set_hpet_reg(hpet_base, 0xF0, 0, 0);   // clear the counter
    u32 counter_lo = 0, counter_hi = 0;
    set_hpet_reg(hpet_base, 0x10, 0x1, 0); // enable

    // test if counter works
    // delay a while
    for (volatile int i = 0; i < 100000; i++)
        asm volatile("nop\n\t");
    // read the counter
    set_hpet_reg(hpet_base, 0x10, 0x0, 0); // disable
    counter_lo = 0, counter_hi = 0;
    get_hpet_reg(hpet_base, 0xF0, &counter_lo, &counter_hi);
    kprintf("HPET Counter test: 0x%x 0x%x\n", counter_hi, counter_lo);
    set_hpet_reg(hpet_base, 0xF0, 0x0, 0x0); // resrt counter
    if (counter_lo || counter_hi) {
        get_hpet_reg(hpet_base, 0xF0, &counter_lo, &counter_hi);
        kprintf("HPET Counter test: 0x%x 0x%x\n", counter_hi, counter_lo);

        // check timer 0 if support periodic
        u32 caplo = 0, caphi = 0;
        get_hpet_reg(hpet_base, 0x100, &caplo, &caphi);
        kprintf("HPET Timer 0 cap reg: 0x%x 0x%x\n", caphi, caplo);
        if (!(caplo & (1 << 4))) {
            kprintf("HPET Timer 0 doesn't support peridoic type!\n");
            return 1;
        }

        set_hpet_reg(hpet_base, 0x100,
                     0x2 << 9 // IOAPIC Route(irq number)
                         | 0x004c | caplo,
                     0); // set timer 0 conf_cap reg

#define fsconst 1000000000000UL
        u64                v          = fsconst * BEATS_MS;
        unsigned long long comparator = divide_64_by_32(&v, general_cap_reg_hi);
        kprintf("HPET Timer0 comparator set to: 0x%x 0x%x\n", ((u32 *)&v)[1],
                ((u32 *)&v)[0]);

        u32 addr = (u32)hpet_base + 0x108;
        *((volatile u32 *)(addr)) =
            ((u32 *)&v)[0]; // set timer 0 comparator value lower 32
        mfence();

        set_hpet_reg(hpet_base, 0x100,
                     0x2 << 9 // IOAPIC Route(irq number)
                         | 0x004c | caplo,
                     0); // reenable Tn_VAL_SET_CNF for cmp write

        *((volatile u32 *)(addr + 0x4)) =
            ((u32 *)&v)[1]; // set timer 0 comparator value higher 32
        mfence();

        set_hpet_reg(hpet_base, 0x10, 0x3, 0); // enable for legency
        return 0;
    } else {
        kprintf("HPET Counter doesn't work.\n");
        return 1;
    }
    return 0;
}

static Driver_Declaration driver_hpet = {.magic       = DRIVER_DC,
                                         .name        = "HPET",
                                         .major_ver   = 0,
                                         .minor_ver   = 1,
                                         .level       = 4,
                                         .init        = init_hpet,
                                         .initialized = FALSE};

ADD_DRIVER(driver_hpet);