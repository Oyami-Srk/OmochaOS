#ifndef __STRUCT_H__
#define __STRUCT_H__

#include "kernel/type.h"

typedef struct {
  u16 gs, pd1, fs, pd2, es, pd3, ds, pd4;
  u32 edi, esi, ebp, oesp, ebx, edx, ecx, eax;
  uint trap_no;
  uint err_code;
  u32 eip;
  u16 cs, pd5;
  u32 eflags, esp;
  u16 ss, pd6;
} __attribute__((packed)) stack_frame;

struct tss {
  u32 backlink;
  u32 esp0, ss0, esp1, ss1, esp2, ss2, cr3, eip, flags, eax, ecx, edx, ebx, esp,
      ebp, esi, edi, es, cs, ss, ds, fs, gs, ldt;
  u16 trap;
  u16 iobase; /* I/O位图基址大于或等于TSS段界限，就表示没有I/O许可位图 */
  // char iomap[2];
} __attribute__((packed));

typedef struct {
  u16 limit_l;
  u16 base_l;
  u8 base_m;
  u8 attr1;
  u8 limit_h_attr2;
  u8 base_h;
} __attribute__((packed)) Descriptor;

typedef struct {
  u16 offset_l;
  u16 selector;
  u8 zero;
  uint type : 4;
  uint s : 1; // system, zero
  uint dpl : 2;
  uint p : 1; // present
  u16 offset_h;
} __attribute__((packed)) Gate;

#endif // __STRUCT_H__