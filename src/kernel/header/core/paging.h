#ifndef __PAGING_H__
#define __PAGING_H__

#include "generic/typedefs.h"

#define PG_Present  0x01
#define PG_Writable 0x02
#define PG_User     0x04
#define PG_PS       0x80

#define PG_OS_SYS 0x200

#define PG_SIZE     0x1000 // 4KB
#define PDE_SIZE    0x1000
#define PTE_PER_PDE 1024

static inline char *vir2phy(pde_t *pg_dir, char *va) {
    pde_t pde = pg_dir[(unsigned int)va >> 22];
    if (!(pde & PG_Present))
        return 0;
    if (pde & PG_PS)
        return (char *)(((unsigned int)pde & ~0xFFF) +
                        ((unsigned int)va & 0x3FFFFF));
    pte_t pte = ((pte_t *)(pde & ~0xFFF))[((unsigned int)va >> 12) & 0x3FF];
    return (char *)((pte & ~0xFFF) + ((unsigned int)va & 0xFFF));
}

#endif // __PAGING_H__
