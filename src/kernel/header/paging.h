#ifndef __PAGING_H__
#define __PAGING_H__

#define PG_Present   0x01
#define PG_Writeable 0x02
#define PG_User      0x04
#define PG_PS        0x08

#define PG_SIZE      0x1000
#define PDE_SIZE     0x1000

#endif // __PAGING_H__
