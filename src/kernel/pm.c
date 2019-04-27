#include "kernel/pm.h"
#include "kernel/memory.h"
#include "lib/string.h"

// temperory settings
#define LDT_COUNT 0

// kernen init gdt
void kinit_gdt(Descriptor *GDT, struct tss *tss, void *ldt) {
  size_t size = sizeof(KERN_GDT) / sizeof(KERN_GDT[0]);
  BOOL has_tss = FALSE;
  uint gdt_count = 0;
  u16 selector_tss = 0;
  for (uint i = 0; i < size; i++)
    switch (KERN_GDT[i][2]) { // check the type
    case DA_386TSS:           // tss gdt item
      gdt_count++;
      has_tss = TRUE;
      selector_tss = i << 3;
      memset(tss, 0, sizeof(struct tss));
      tss->ss0 = 2 << 3; // data selector
      make_descriptor(&GDT[i], (uint)(KV2P(tss)), sizeof(tss) - 1, DA_386TSS);
      break;
    case DA_LDT: { // ldt gdt item
      u16 selector_ldt = i << 3;
      for (uint i = 0; i < LDT_COUNT; i++) {
        ;
        gdt_count++;
        selector_ldt += (1 << 3);
      }
      break;
    }
    default:
      gdt_count++;
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