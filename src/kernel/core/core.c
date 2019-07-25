#include "memory_map.h"
#include "paging.h"

void core_entry() {
    while(1);
}

__attribute__ ((__aligned__ (PG_SIZE))) unsigned int entry_page_dir[PDE_SIZE] = {
    [0] = (0) | PG_Present | PG_Writeable | PG_PS,
    [KERN_BASE >> 22] = (0) | PG_Present | PG_Writeable | PG_PS
};
