#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include "kernel/pm.h"
#include "kernel/type.h"

extern uint vector_table[]; // auto-generated

#define GATE_TASK 0x5
#define GATE_INT16 0x6
#define GATE_INT32 0xE
#define GATE_TRAP16 0x7
#define GATE_TRAP32 0xF

#define IO_PIC_M 0x20
#define IO_PIC_S 0xA0

#define HW_IRQ_TIMER 0
#define HW_IRQ_KBD 1
#define HW_IRQ_AT 14

#define HW_IRQ_COUNT 16

#define IRQ0 32
#define IRQ_TIMER (IRQ0 + HW_IRQ_TIMER)
#define IRQ_KBD (IRQ0 + HW_IRQ_KBD)
#define IRQ_AT (IRQ0 + HW_IRQ_AT)

void kinit_interrupt(Gate *idt, size_t count);
void enable_irq(uint irq);
void disable_irq(uint irq);

struct interrupt_method {
  uint pid;
  fp_v_v func;
  ushort avail;
} __attribute__((packed));

extern struct interrupt_method interrupt_methods[HW_IRQ_COUNT];
extern uint interrupt_suscribed[HW_IRQ_COUNT];

#endif // __INTERRUPT_H__