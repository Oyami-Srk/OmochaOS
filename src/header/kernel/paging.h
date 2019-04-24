#ifndef __PAGING_H__
#define __PAGING_H__

#include "kernel/memory.h"
#include "kernel/type.h"

#define PG_Present 0x01
#define PG_Writeable 0x02
#define PG_User 0x04
#define PG_PS 0x80

void kinit_paging();

#endif // __PAGING_H__