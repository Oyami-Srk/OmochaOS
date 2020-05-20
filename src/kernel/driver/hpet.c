#include "driver/hpet.h"
#include "core/acpi.h"
#include "generic/asm.h"

#include "driver/graphic.h"

void *hpet_base;

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
    u32 general_cap_reg_lo;
    u32 general_cap_reg_hi;
    get_hpet_reg(hpet_base, 0x0, &general_cap_reg_lo, &general_cap_reg_hi);
    kprintf("HPET general_cap_reg: 0x%x 0x%x\n", general_cap_reg_hi,
            general_cap_reg_lo);
    return TRUE;
}