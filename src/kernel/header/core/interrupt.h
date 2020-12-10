#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include <core/config.h>
#include <core/protect.h>
#include <generic/typedefs.h>

#define GATE_TASK   0x5
#define GATE_INT16  0x6
#define GATE_INT32  0xE
#define GATE_TRAP16 0x7
#define GATE_TRAP32 0xF

#define IO_PIC_M 0x20
#define IO_PIC_S 0xA0

#if APIC
// IOAPIC remap
#define HW_IRQ_TIMER 2
#else
#define HW_IRQ_TIMER 0
#endif
#define HW_IRQ_KBD 1
#define HW_IRQ_AT  14

#define HW_IRQ_COUNT    16
#define EXCEPTION_COUNT 32

#define IRQ0      32
#define IRQ_TIMER (IRQ0 + HW_IRQ_TIMER)
#define IRQ_KBD   (IRQ0 + HW_IRQ_KBD)
#define IRQ_AT    (IRQ0 + HW_IRQ_AT)

#define EXCEPTION_DE  0
#define EXCEPTION_DB  1
#define EXCEPTION_NMI 2
#define EXCEPTION_BP  3
#define EXCEPTION_OF  4
#define EXCEPTION_BR  5
#define EXCEPTION_UD  6
#define EXCEPTION_NM  7
#define EXCEPTION_DF  8
#define EXCEPTION_TS  10
#define EXCEPTION_NP  11
#define EXCEPTION_SS  12
#define EXCEPTION_GP  13
#define EXCEPTION_PF  14
#define EXCEPTION_MF  16
#define EXCEPTION_AC  17
#define EXCEPTION_MC  18
#define EXCEPTION_XM  19
#define EXCEPTION_VE  20
#define EXCEPTION_SX  30

struct interrupt_method {
    pid_t pid;
    void *func;
    void *data; // provided to func
    uint  avail;
} __attribute__((packed));

struct interrupt_data {
    uint avail;
    uint data;
} __attribute__((packed));

// void enable_interrupt(uint irq);
// void disable_interrupt(uint irq);
// void end_interrupt(uint irq);
extern interrupt_func enable_interrupt;
extern interrupt_func disable_interrupt;
extern interrupt_func end_interrupt;

#endif // __INTERRUPT_H__
