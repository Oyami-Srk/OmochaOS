#ifndef __PAGING_H__
#define __PAGING_H__

#define PG_Present   0x01
#define PG_Writeable 0x02
#define PG_User      0x04
#define PG_PS        0x80

#define PG_SIZE          0x1000 // 4KB
#define PDE_SIZE_STARTUP 0x1000
#define PTE_PER_PDE      1024

#endif // __PAGING_H__
