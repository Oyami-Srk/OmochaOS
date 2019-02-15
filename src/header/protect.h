/* 保护模式所使用的相关变量或参数 */

#ifndef __OMOCHA_OS__PROTECT_H__
#define __OMOCHA_OS__PROTECT_H__

#include "interrupt.h"
#include "const.h"
#include "type.h"


static inline GDT_Descriptor* MAKE_DESC(GDT_Descriptor *pDesc,
                                        unsigned int Base,
                                        unsigned int Limit,
                                        unsigned short Attr){

  pDesc->LimitL = Limit & 0xFFFF;
  pDesc->BaseL = Base & 0xFFFF;
  pDesc->BaseM = (Base >> 16) & 0x0FF;
  pDesc->BaseH = (Base >> 24) & 0x0FF;
  pDesc->Attr1 = Attr & 0xFF;
  pDesc->LimitH_Attr2 = ((Limit >> 16) & 0x0F) | ((Attr >> 8) & 0xF0);
  return pDesc;
}

/* GDT */

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

#define DPL3 0x03
#define DPL2 0x02
#define DPL1 0x01
#define DPL0 0x00

#define DPL_USER DPL3

void kinit_gdt(cpu_env *env);

#endif
