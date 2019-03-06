#include "kernel/memlayout.h"
#include "kernel/protect.h"
#include "kernel/config.h"
#include "kernel/klib.h"
#include "kernel/type.h"
#include "string.h"


void kinit_gdt(cpu_env *env){
  // kernel init GDT
  uint sz = sizeof(KERN_GDT) / sizeof(KERN_GDT[0]);
  int has_tss = 0;
  int gdt_count = 0;
  for(uint i = 0; i < sz; i++)
    switch(i){
    case GDT_TSS_INDEX:
      gdt_count++;
      has_tss = 1;
      memset(&env->tss, 0, sizeof(env->tss));
      env->tss.ss0 = SEL_KDATA;
      MAKE_DESC(&env->gdt[i], (uint)V2P(&env->tss), sizeof(env->tss) - 1, DA_386TSS);
      break;
    case GDT_LDT1_INDEX: {
      ushort s_ldt = SEL_LDT1;
      for(int i = 0; i < PROC_COUNT; i++) {
        MAKE_DESC(&env->gdt[s_ldt >> 3], (uint)V2P(env->processes[i].ldts), LDT_SIZE * sizeof(Descriptor) - 1, DA_LDT);
        gdt_count++;
        s_ldt += (1 << 3);
      }
      break;
    }
    default:
      gdt_count++;
      MAKE_DESC(&env->gdt[i], KERN_GDT[i][0], KERN_GDT[i][1], KERN_GDT[i][2]);
      break;
    }
  unsigned char gdt_ptr[6];

  unsigned short *gdt_limit = (unsigned short *)(&gdt_ptr[0]);
  unsigned int *gdt_base = (unsigned int *)(&gdt_ptr[2]);
  *gdt_limit = gdt_count * sizeof(Descriptor) - 1;
  *gdt_base = (unsigned int)env->gdt;


  __asm__ __volatile__("lgdtl (%0)":: "r"(gdt_ptr));

  asm (
       "\n"
       "pushl %0\n"
       "pushl $1f\n"
       "lret \n"
       "1:\n"
       "mov %1, %%eax\n"
       "mov %%ax, %%ds\n"
       "mov %%ax, %%es\n"
       "mov %%ax, %%fs\n"
       "mov %%ax, %%gs\n"
       "mov %%ax, %%ss\n"

       :
       : "g" (0x08), "g" (0x10)
       : "eax", "memory");

  if(has_tss){
    __asm__ __volatile__("ltr %%ax"::"a"(SEL_TSS));
  }
  return;
}
