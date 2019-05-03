#include "kernel/pm.h"
#include "kernel/memory.h"
#include "lib/string.h"

// temperory settings
#define LDT_COUNT 0

// kernen init gdt
void kinit_gdt(Descriptor *GDT, size_t gdt_count, struct tss *tss,
               process *procs, size_t proc_count) {
  size_t size = sizeof(KERN_GDT) / sizeof(KERN_GDT[0]);
  BOOL has_tss = FALSE;
  uint gdt_n = 0;
  u16 selector_tss = 0;

  for (uint i = 0; i < size; i++)
    switch (KERN_GDT[i][2]) {
    case DA_LDT:
      gdt_n += proc_count;
      break;
    default:
      gdt_n++;
      break;
    }
  if (gdt_n > gdt_count)
    while (1)
      ; // panic

  gdt_n = 0;

  for (uint i = 0; i < size; i++)
    switch (KERN_GDT[i][2]) { // check the type
    case DA_386TSS:           // tss gdt item
      gdt_n++;
      has_tss = TRUE;
      selector_tss = i << 3;
      memset(tss, 0, sizeof(struct tss));
      tss->ss0 = 2 << 3; // data selector
      make_descriptor(&GDT[i], (uint)(KV2P(tss)), sizeof(struct tss) - 1,
                      DA_386TSS);
      break;
    case DA_LDT: { // ldt gdt item
      u16 selector_ldt = i << 3;
      for (uint i = 0; i < proc_count; i++) {
        make_descriptor(&GDT[selector_ldt >> 3], (u32)(KV2P(procs[i].ldts)),
                        sizeof(Descriptor) * LDT_SIZE_PER_PROC - 1, DA_LDT);
        procs[i].selector_ldt = selector_ldt;
        gdt_n++;
        selector_ldt += (1 << 3);
      }
      break;
    }
    default:
      gdt_n++;
      make_descriptor(&GDT[i], KERN_GDT[i][0], KERN_GDT[i][1], KERN_GDT[i][2]);
      break;
    }

  uchar gdt_ptr[6];
  u16 *gdt_limit = (u16 *)(&gdt_ptr[0]);
  u32 *gdt_base = (u32 *)(&gdt_ptr[2]);
  *gdt_limit = gdt_count * sizeof(Descriptor) - 1;
  *gdt_base = (u32)GDT;

  asm volatile("lgdtl (%0)" ::"r"(gdt_ptr));
  asm("\n"
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
      : "g"(0x08), "g"(0x10)
      : "eax", "memory"); // flush the cpu
  if (has_tss)
    asm volatile("ltr %%ax" ::"a"(selector_tss));
  return;
}