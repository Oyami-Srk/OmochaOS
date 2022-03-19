#ifndef __PAGING_H__
#define __PAGING_H__

#include <generic/typedefs.h>

#define PG_Present  0x0001
#define PG_Writable 0x0002
#define PG_User     0x0004
#define PG_PWT      0x0008
#define PG_PCD      0x0010
#define PG_Accessed 0x0020
#define PG_Dirty    0x0040
#define PG_PS       0x0080
#define PG_Global   0x0100

#define PG_OS_SYS   0x0200
#define PG_OS_RSV1  0x0400
#define PG_OS_RSV2  0x0800
#define PG_PAT      0x1000

#define PG_SIZE     0x1000 // 4KB
#define EXT_PG_SIZE 0x400000 // 4MB

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
