#include "config.h"
#include "paging.h"
#include "memlayout.h"

typedef struct {
  unsigned int BaseL;
  unsigned int BaseH;
  unsigned int LengthL;
  unsigned int LengthH;
  unsigned int Type;
  unsigned int ACPI;
}__attribute__((packed)) SMAP_entry;

int main(void){
  unsigned int *mem_infos_count = BOOT_LOADER_MEM_INFO_COUNT;
  SMAP_entry *mem_infos = BOOT_LOADER_MEM_INFO;

  while(1);
}

/*
  一开始使用4M的内存页，节约开销和麻烦
 */
__attribute__((__aligned__(PG_SIZE)))
unsigned int temp_page_dir[PDE_SIZE] = {
                           [0] = (0) | PG_P | PG_W | PG_PS,
                           [KERN_BASE >> 22] = (0) | PG_P | PG_W | PG_PS
};

