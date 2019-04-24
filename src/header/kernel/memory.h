#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "kernel/type.h"

#ifndef __BL_OTHER__
#define BL_MEMINFO 0x8000
#define BL_MEMINCO_N (BL_MEMINFO - 4)
#endif // __BL_OTHER__

#define KERN_BASE 0x80000000
#define KP2V(a) ((void *)(((char *)(a)) + KERN_BASE))
#define KV2P(a) ((void *)(((char *)(a)) - KERN_BASE))

extern char KERN_END[]; // defined in kernel.ld

#define PG_SIZE 0x1000
#define PDE_SIZE 0x1000

void kinit_mem(void *vstart, void *vend);
void kfree(void *v);
char *kalloc(void);

#endif // __MEMORY_H__