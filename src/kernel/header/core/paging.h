#ifndef __PAGING_H__
#define __PAGING_H__

#define PG_Present   0x01
#define PG_Writeable 0x02
#define PG_User      0x04
#define PG_PS        0x80

#define PG_SIZE     0x1000 // 4KB
#define PDE_SIZE    0x1000
#define PTE_PER_PDE 1024

typedef unsigned int pte_t; // page table entity
typedef unsigned int pde_t; // page dir entity

#endif // __PAGING_H__
