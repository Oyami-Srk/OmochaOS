#ifndef __OMOCHA_OS__INTERRUPT_H__
#define __OMOCHA_OS__INTERRUPT_H__

#include "type.h"

extern uint vector_table[]; // generated in vector_table.asm

#define GATE_TASK   0x5
#define GATE_INT16  0x6
#define GATE_INT32  0xE
#define GATE_TRAP16 0x7
#define GATE_TRAP32 0xF

static inline Gate_Descriptor *MAKE_GATE(Gate_Descriptor *pGate,
                                         uint sel,
                                         uint Offset,
                                         uint DPL,
                                         uint Type){
  pGate->offset_low = (uint)Offset & 0xFFFF;
  pGate->offset_high = (uint)Offset >> 16;
  pGate->sel = sel;
  pGate->some_thing_should_be_zero = 0;
  pGate->type = Type;
  pGate->s = 0;
  pGate->dpl = DPL;
  pGate->p = 1;
  return pGate;
}

typedef struct {
  ushort gs;
  ushort padding1;
  ushort fs;
  ushort padding2;
  ushort es;
  ushort padding3;
  ushort ds;
  ushort padding4;

  uint edi;
  uint esi;
  uint ebp;
  uint oesp;
  uint ebx;
  uint edx;
  uint ecx;
  uint eax;

  uint trapno; // push by vector_table
  uint err;

  uint eip;
  ushort cs;
  ushort padding5;
  uint eflags;
  uint esp;
  ushort ss;
  ushort padding6;

} interrupt_frame;

#define IO_PIC_M 0x20
#define IO_PIC_S 0xA0

#define IRQ0 32
#define IRQ_TIMER (IRQ0 + 0)
#define IRQ_KBD (IRQ0 + 1)

void kinit_interrupt(cpu_env *env);

#endif
