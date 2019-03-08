#ifndef __OMOCHA_OS__INTERRUPT_H__
#define __OMOCHA_OS__INTERRUPT_H__

#include "type.h"

extern uint vector_table[]; // generated in vector_table.asm

#define GATE_TASK   0x5
#define GATE_INT16  0x6
#define GATE_INT32  0xE
#define GATE_TRAP16 0x7
#define GATE_TRAP32 0xF

static inline Gate *MAKE_GATE(Gate *pGate,
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


#define IO_PIC_M 0x20
#define IO_PIC_S 0xA0

#define IRQ0 32
#define IRQ_TIMER (IRQ0 + 0)
#define IRQ_KBD (IRQ0 + 1)

#define HW_IRQ_TIMER 0
#define HW_IRQ_KBD 1

#define HW_IRQ_COUNT 16

void kinit_interrupt(cpu_env *env);
void enable_irq(uint irq);
void disable_irq(uint irq);
void kreload_process();

extern ushort interrupt_map[2][HW_IRQ_COUNT];

#endif
