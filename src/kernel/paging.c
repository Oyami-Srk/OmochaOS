#include "paging.h"
#include "memlayout.h"
#include "klib.h"

#define ADDR_TO_DIR(va) (((unsigned int)(va) >> 22) & 0x3FF)
#define ADDR_TO_TBL(va) (((unsigned int)(va) >> 12) & 0x3FF)
#define ALIGN_DOWN(a) (((a)) & ~(4096 - 1))

unsigned int* found_tbl_by_va(unsigned int *va){
  unsigned int *pde = &((unsigned int*)(KERN_PAGE_DIR_BASE))[ADDR_TO_DIR(va)];
  if(!(*pde & PG_P))
    /* *pde = (KERN_PAGE_TBL_BASE + (ADDR_TO_DIR(va) * 1024 + ADDR_TO_TBL(va)) << 12) | PG_P | PG_U | PG_W; */
    *pde = KERN_PAGE_TBL_BASE;
    /* return 0; */
  return *pde;
  /* return &((unsigned int*)(((unsigned int)*pde & ~0xFFF)))[ADDR_TO_TBL(va)]; */
}

void vm_map(unsigned int *vstart,
            unsigned int length,
            unsigned int *pstart){
  // if pstart + length > memsize then panic fi
  unsigned int *i = (unsigned int*)(ALIGN_DOWN((unsigned int)vstart));
  unsigned int *last = (unsigned int*)(ALIGN_DOWN((unsigned int)vstart + length - 1));
  unsigned int *pte;
  char buf[16];
  itoa((unsigned int)found_tbl_by_va(i), buf, 16);
  write_string(0x0F, buf);
  write_string(0x0F, "\n");
  for(;i <= last;i+=4096,last+=4096){
    pte = found_tbl_by_va(i);
    *pte = (unsigned int)pstart | PG_U | PG_W | PG_P;
  }
}


void kinit_paging(unsigned int sec, SMAP_entry *first){
  // Kernel init paging
  // Found memory size
  unsigned int memory_size = 0;
  SMAP_entry *mem_info = first;
  for(; mem_info < first + sec; mem_info++)
    if((mem_info->BaseH << 16 | mem_info->BaseL) + (mem_info->LengthH << 16 | mem_info->LengthL) > memory_size)
      memory_size = (mem_info->BaseH << 16 | mem_info->BaseL) + (mem_info->LengthH << 16 | mem_info->LengthL);

  write_string(0x0F, "Memory size is ");
  char buf[16];
  itoa(memory_size / 1024 / 1024, buf, 10);
  write_string(0x0F, buf);
  write_string(0x0F, " MB\n");

  // allocating page table dir
  unsigned int memory_size_per_dir = 4096 * 1024; // 10bit present dir->tbl, 1024 tbl per dir
  unsigned int dir_count = memory_size / memory_size_per_dir +
    (memory_size % memory_size_per_dir?1:0);
  // phys addr directly map to linar addr
  unsigned int *dir = (unsigned int *)KERN_PAGE_DIR_BASE;
  unsigned int template_dir = KERN_PAGE_TBL_BASE - KERN_BASE | PG_P | PG_W | PG_U;
  itoa(dir_count, buf, 10);
  write_string(0x0F, "We need ");
  write_string(0x0F, buf);
  write_string(0x0F, " page dirs to map all memory\n");
  for(int i = 0; i < dir_count; i++){
    *dir++ = template_dir;
    template_dir += 4096; // add 12 bit which contains table base
  }

  // allocating page table
  unsigned int *tbl = (unsigned int *)KERN_PAGE_TBL_BASE;
  unsigned int template_tbl = 0x00000000 | PG_P | PG_W | PG_U;
  for(int i = 0; i < dir_count * 1024; i++){
    *tbl++ = template_tbl;
    template_tbl += 4096;
  }

  write_string(0x0F, "Mapping kernel addr\n");
  // apply kernel addredd 0x80000000 - 0x8F000000
  vm_map((unsigned int*)KERN_BASE, KERN_END - KERN_BASE, 0x0);



  write_string(0x0F, "Write cr3: 0x");
  itoa(KERN_PAGE_DIR_BASE - KERN_BASE, buf, 16);
  write_string(0x0F, buf);
  write_string(0x0F, "\n");

  __asm__ __volatile__("mov %%eax, %%cr3"::"a"(KERN_PAGE_DIR_BASE - KERN_BASE));
}
