#include "config.h"
#include "kmem.h"
#include "paging.h"
#include "memlayout.h"
#include "protect.h"
#include "klib.h"
#include "interrupt.h"

cpu_env cpu;

int main(void){
  cpu.cpu_id = 0;

  unsigned int *mem_infos_count = (unsigned int*)BOOT_LOADER_MEM_INFO_COUNT;
  SMAP_entry *mem_infos = (SMAP_entry*)BOOT_LOADER_MEM_INFO;

  kinit_gdt(&cpu);
  kinit_interrupt(&cpu);
  kinit_mem(KERN_END, P2V(4 * 1024 * 1024));
  /* kinit_paging(*mem_infos_count, mem_infos, &cpu); */

  clr_scr();

  write_string(0x0F, "Hello world!\n");
  char buf[32];
  itoa((uint)KERN_END, buf, 16);
  write_string(0x0F, "Kern end addr is ");
  write_string(0x0F, buf);
  write_string(0x0F, "\n");
  kprint_meminfo(mem_infos, *mem_infos_count);

  panic("test panic");

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

