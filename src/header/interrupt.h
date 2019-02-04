#ifndef __OMOCHA_OS__INTERRUPT_H__
#define __OMOCHA_OS__INTERRUPT_H__

#include "type.h"

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

#define GATE_TASK   0x5
#define GATE_INT16  0x6
#define GATE_INT32  0xE
#define GATE_TRAP16 0x7
#define GATE_TRAP32 0xF

static inline Gate_Descriptor *MAKE_GATE(Gate_Descriptor *pGate,
                                         uint Offset,
                                         uint DPL,
                                         uint Type){
  pGate->offset_low = (uint)Offset & 0xFFFF;
  pGate->offset_high = (uint)Offset >> 16;
  pGate->some_thing_should_be_zero = 0;
  pGate->type = Type;
  pGate->s = 0;
  pGate->dpl = DPL;
  pGate->p = 1;
  return pGate;
}

#endif
