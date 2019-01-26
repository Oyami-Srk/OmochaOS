/* 保护模式所使用的相关变量或参数 */

#ifndef __OMOCHA_OS__PROTECT_H__
#define __OMOCHA_OS__PROTECT_H__

typedef struct {
  unsigned short LimitL;
  unsigned short BaseL;
  unsigned char BaseM;
  unsigned char Attr1;
  unsigned char LimitH_Attr2;
  unsigned char BaseH;
}__attribute__((packed)) GDT_Descriptor;

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

#endif
