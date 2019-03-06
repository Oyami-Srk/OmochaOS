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

#define INT_F_DE 0
#define INT_FT_DB 1
#define INT_I_UCI 2
#define INT_T_BP 3
#define INT_T_OF 4
#define INT_F_BR 5
#define INT_F_UD 6
#define INT_F_NM 7
#define INT_A_DF 8
#define INT_F_RESV 9
#define INT_F_TS 10
#define INT_F_NP 11
#define INT_F_SS 12
#define INT_F_GP 13
#define INT_F_PF 14
#define INT_F_RESV2 15
#define INT_F_MF 16
#define INT_F_AC 17
#define INT_A_MC 18
#define INT_F_XF 19

#define IRQ0 32
#define IRQ_TIMER (IRQ0 + 0)
#define IRQ_KBD (IRQ0 + 1)

void kinit_interrupt(cpu_env *env);
void kreload_process();

#endif
