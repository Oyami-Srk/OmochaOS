#include "memlayout.h"
#include "protect.h"
#include "config.h"
#include "klib.h"

void kinit_gdt(cpu_env *env){
  // kernel init GDT
  uint sz = sizeof(KERN_GDT) / sizeof(KERN_GDT[0]);
  for(uint i = 0; i < sz; i++)
    MAKE_DESC(&env->gdt[i], KERN_GDT[i][0], KERN_GDT[i][1], KERN_GDT[i][2]);

  unsigned char gdt_ptr[6];

  unsigned short *gdt_limit = (unsigned short *)(&gdt_ptr[0]);
  unsigned int *gdt_base = (unsigned int *)(&gdt_ptr[2]);
  *gdt_limit = sizeof(env->gdt) * sizeof(GDT_Descriptor) - 1;
  *gdt_base = (unsigned int)env->gdt;

  char buf[16];
  itoa(sizeof(env->gdt), buf, 10);
  write_string(0x0F, buf);

  __asm__ __volatile__("lgdtl (%0)":: "r"(gdt_ptr));
  return;
}
