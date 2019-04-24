#include "kernel/memory.h"
#include "kernel/paging.h"
#include "kernel/type.h"

int main(void) {
  kinit_mem(KERN_END, KP2V(4 * 1024 * 1024));
  while (1)
    ;
}

/*
  一开始使用4M的内存页，节约开销和麻烦
 */
__attribute__((__aligned__(PG_SIZE))) unsigned int temp_page_dir[PDE_SIZE] = {
    [0] = (0) | PG_Present | PG_Writeable | PG_PS,
    [KERN_BASE >> 22] = (0) | PG_Present | PG_Writeable | PG_PS};
