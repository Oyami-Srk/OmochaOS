#include "kernel/interrupt.h"
#include "kernel/memory.h"
#include "kernel/pm.h"
#include "kernel/structs.h"
#include "lib/asm.h"

const char *exception_message[] = {"#DE: Divide-by-zero Error",
                                   "#DB: Debug",
                                   "#--: Non-maskable Interrupt",
                                   "#BP: Break Point",
                                   "#OF: Overflow",
                                   "#BR: Bound Range Exceeded",
                                   "#UD: Invalid Opcode",
                                   "#NM: Device Not Avaliable",
                                   "#DF: Double Fault",
                                   "#RESV: Reserved",
                                   "#TS: Invalid TSS",
                                   "#NP: Segment Not Present",
                                   "#SS: Stack-Segment Fault",
                                   "#GP: General Protection Fault",
                                   "#PF: Page Fault",
                                   "#RESV: Reserved2",
                                   "#MF: x87 Floating-Point Exception",
                                   "#AC: Alignment Check",
                                   "#MC: Machine Check",
                                   "#XF: SIMD Floating-Point Exception"

};

static inline void EOI_M(void) { outb(IO_PIC_M, 0x20); }

static inline void EOI_S(void) { outb(IO_PIC_S, 0x20); }

void interrupt_handler(stack_frame *intf) {
  if (intf->trap_no == 32) {
    volatile char *ch = (volatile char *)(0xb8000);
    *ch = *ch + 1;
    scheduler();
    EOI_M();
  }
  return;
}

void init_8259A() {
  outb(IO_PIC_M, 0x11);     // ICW 1
  outb(IO_PIC_S, 0x11);     // ICW 1
  outb(IO_PIC_M + 1, 0x20); // 0x20 -> Master first
  outb(IO_PIC_S + 1, 0x28); // 0x28 -> Salve first

  outb(IO_PIC_M + 1, 0x4); // ICW 3
  outb(IO_PIC_S + 1, 0x2); // ICW 3
  outb(IO_PIC_M + 1, 0x1);
  outb(IO_PIC_S + 1, 0x1);

  outb(IO_PIC_M + 1, 0xFE);
  outb(IO_PIC_S + 1, 0xFF);
}

void kinit_interrupt(Gate *idt, size_t count) {
  for (uint i = 0; i < 256; i++)
    make_gate(&idt[i], 1 << 3, (uint)vector_table[i], 0, GATE_INT32);
  uchar idt_ptr[6];
  u16 *idt_limit = (u16 *)(&idt_ptr[0]);
  u32 *idt_base = (u32 *)(&idt_ptr[2]);
  *idt_limit = count * sizeof(Gate) - 1;
  *idt_base = (uint)idt - KERN_BASE;
  asm volatile("lidt (%0)" ::"r"(idt_ptr));
  init_8259A();
  asm("sti");
}