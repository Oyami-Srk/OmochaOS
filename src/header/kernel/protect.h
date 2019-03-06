/* 保护模式所使用的相关变量或参数 */

#ifndef __OMOCHA_OS__PROTECT_H__
#define __OMOCHA_OS__PROTECT_H__

#include "interrupt.h"
#include "const.h"
#include "type.h"

static inline Descriptor* MAKE_DESC(Descriptor *pDesc,
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

#define DA_LDT 0x82
#define DA_TaskGate 0x85
#define DA_386TSS 0x89
#define DA_386CGate 0x8C
#define DA_386IGate 0x8E
#define DA_386TGate 0x8F

#define SA_RPL_MASK 0xFFFC
#define SA_RPL0 0
#define SA_RPL1 1
#define SA_RPL2 2
#define SA_RPL3 3

#define	SA_TI_MASK	0xFFFB
#define	SA_TIG		0
#define	SA_TIL		4

void kinit_gdt(cpu_env *env);

#endif
