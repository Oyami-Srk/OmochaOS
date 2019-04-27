#ifndef __PM_H__
#define __PM_H__

#include "kernel/structs.h"
#include "kernel/type.h"

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

static inline Descriptor *make_descriptor(Descriptor *pDesc, u32 Base,
                                          u32 Limit, u16 Attr) {
  pDesc->limit_l = Limit & 0xFFFF;
  pDesc->base_l = Base & 0xFFFF;
  pDesc->base_m = (Base >> 16) & 0x0FF;
  pDesc->base_h = (Base >> 24) & 0x0FF;
  pDesc->attr1 = Attr & 0xFF;
  pDesc->limit_h_attr2 = ((Limit >> 16) & 0x0F) | ((Attr >> 8) & 0xF0);
  return pDesc;
}

static inline Gate *make_gate(Gate *pGate, u16 Selector, u32 Offset, uint DPL,
                              uint Type) {
  pGate->offset_l = Offset & 0xFFFF;
  pGate->offset_h = (Offset >> 16) & 0xFFFF;
  pGate->selector = Selector;
  pGate->zero = 0;
  pGate->type = Type;
  pGate->s = 0;
  pGate->dpl = DPL;
  pGate->p = 1;
  return pGate;
}

// Descriptor Attribute
#define DA_32 0x4000
#define DA_4K 0x8000
#define DA_DPL0 0x00
#define DA_DPL1 0x20
#define DA_DPL2 0x40
#define DA_DPL3 0x60

#define DA_DR 0x90
#define DA_DRW 0x92
#define DA_DRWA 0x93
#define DA_C 0x98
#define DA_CR 0x9A
#define DA_CCO 0x9C
#define DA_CCOR 0x9E

#define DA_LDT 0x82
#define DA_TaskGate 0x85
#define DA_386TSS 0x89
#define DA_386CGate 0x8C
#define DA_386IGate 0x8E
#define DA_386TGate 0x8

// Selector Attribute
#define SA_RPL_MASK 0xFFFC
#define SA_RPL0 0
#define SA_RPL1 1
#define SA_RPL2 2
#define SA_RPL3 3

#define SA_TI_MASK 0xFFFB
#define SA_TIG 0
#define SA_TIL 4

#define DPL0 0x00
#define DPL1 0x01
#define DPL2 0x02
#define DPL3 0x03

#define DPL_USER DPL3

static uint KERN_GDT[][3] = {
    {0, 0, 0},                               // dummy null descriptor
    {0, 0xFFFFFFFF, DA_32 | DA_4K | DA_C},   // Code descriptor
    {0, 0xFFFFFFFF, DA_32 | DA_4K | DA_DRW}, // Data descriptor
    {0x0, 0x0, DA_386TSS},                   // TSS descriptor
    {0x0, 0x0, DA_LDT}                       // LDT descriptor
                       // Cannot define more descriptors below ldt
}; // static but not const

void kinit_gdt(Descriptor *GDT, struct tss *tss, void *ldt);

#endif // __PM_H__
