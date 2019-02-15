#ifndef __OMOCHA_OS__TYPE_H__
#define __OMOCHA_OS__TYPE_H__

#include "const.h"

typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;

typedef unsigned int pte; // for hint
typedef unsigned int pde;

typedef struct {
  unsigned short LimitL;
  unsigned short BaseL;
  unsigned char BaseM;
  unsigned char Attr1;
  unsigned char LimitH_Attr2;
  unsigned char BaseH;
}__attribute__((packed)) GDT_Descriptor;

typedef struct {
  uint offset_low : 16;
  uint sel : 16;
  // attr
  uint some_thing_should_be_zero : 8;
  uint type : 4;
  uint s: 1; // system , be zero
  uint dpl: 2; // 描述符特权级
  uint p: 1; // present
  // attr end
  uint offset_high: 16;
}__attribute__((packed)) Gate_Descriptor;


typedef struct {
  uint cpu_id;
  Gate_Descriptor idt[IDT_COUNT];
  GDT_Descriptor gdt[GDT_COUNT];
  pde *pgdir;
} cpu_env;

typedef char* va_list;

#endif
