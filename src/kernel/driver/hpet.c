#include "driver/hpet.h"
#include "core/acpi.h"
#include "driver/misc.h"
#include "generic/asm.h"

#include "driver/graphic.h"

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

static inline void get_hpet_reg(void *base, u16 offset, u32 *lo, u32 *hi) {
    *lo = ((u32 *)base + offset)[0];
    mfence();
    *hi = ((u32 *)base + offset)[1];
    mfence();
}

static inline void set_hpet_reg(void *base, u16 offset, u32 lo, u32 hi) {
    ((u32 *)(base + offset))[0] = lo;
    mfence();
    ((u32 *)(base + offset))[1] = hi;
    mfence();
}

BOOL init_hpet(struct core_env *env) {
    kprintf("Initializing HPET\n");
    struct HPET *hpet = (struct HPET *)search_sdt(env, "HPET");
    if (!hpet)
        return FALSE;
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
    disable_8253();

    set_hpet_reg(hpet_base, 0xF0, 0x1, 0x0);
    set_hpet_reg(hpet_base, 0x100,
                 0x2 << 9 /*IOAPIC Route(irq number)*/ | 0x004c,
                 0); // set timer 0 conf_cap reg
#define fsconst 1000000000000UL
    u64                v          = fsconst * BEATS_MS;
    unsigned long long comparator = divide_64_by_32(&v, general_cap_reg_hi);
    set_hpet_reg(hpet_base, 0x108, (((u32 *)&v))[0],
                 (((u32 *)&v))[1]); // set timer 0 comparator value

    set_hpet_reg(hpet_base, 0x10, 0x1, 0); // enable

    return TRUE;
}