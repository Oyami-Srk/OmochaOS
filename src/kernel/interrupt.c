#include "kernel/interrupt.h"
#include "kernel/memory.h"
#include "kernel/pm.h"
#include "kernel/structs.h"

#include "lib/asm.h"
#include "lib/stdlib.h"

#include "driver/vga.h"

extern void *syscall_table[];
struct interrupt_method interrupt_methods[HW_IRQ_COUNT];
uint interrupt_suscribed[HW_IRQ_COUNT];

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

extern uint beats;

void send_interrupt_msg(uint irq, uint pid) {
  assert(interrupt_methods[irq].pid < proc_table_size);

  process *p = &proc_table[pid];
  if (p->status & PROC_STATUS_RECEVING) {
    if (GET_PROC_STATUS_PID(p) == 0 || GET_PROC_STATUS_PID(p) == INTERRUPT) {
      p->p_msg->sender = INTERRUPT;
      p->p_msg->type = INTERRUPT;
      p->p_msg->major_data = irq;
      p->status &= ~PROC_STATUS_RECEVING;
      return;
    }
  }
  p->status |= PROC_STATUS_GOTINT;
}

void interrupt_handler(stack_frame *intf) {
  if (intf->trap_no <= 19) {
    cli();
    VGA_write_color_string_to_vm(0, COLOR(BLUE, BLACK),
                                 "                    "
                                 "                    "
                                 "                    "
                                 "                   ");
    char buf[128];
    sprintf(buf, "Exception %s in proc %d", exception_message[intf->trap_no],
            0);
    VGA_write_color_string_to_vm(0, COLOR(BLUE, RED), buf);
    magic_break();
    while (1)
      ;
  }

  if (intf->trap_no <= IRQ0 + HW_IRQ_COUNT && intf->trap_no > IRQ_TIMER) {
    if (interrupt_methods[intf->trap_no - IRQ0].avail == TRUE) {
      disable_irq(intf->trap_no - IRQ0);
      // you must enable irq in func to turn on
      interrupt_methods[intf->trap_no - IRQ0].func();
      if (intf->trap_no - IRQ0 < 8)
        EOI_M();
      else {
        EOI_M();
        EOI_S();
      }
    }
    if (interrupt_suscribed[intf->trap_no - IRQ0])
      send_interrupt_msg(intf->trap_no - IRQ0,
                         interrupt_suscribed[intf->trap_no - IRQ0]);
    if (intf->trap_no - IRQ0 < 8)
      EOI_M();
    else {
      EOI_M();
      EOI_S();
    }
  }

  switch (intf->trap_no) {
  case IRQ_TIMER:
    beats++;
    scheduler(0);
    EOI_M();
    break;
  case SYSCALL_INT: {
    volatile uint retval = 0;
    asm volatile("push %%edx\n\t"
                 "push %%ebx\n\t"
                 "push %%ecx\n\t"
                 "push %%eax\n\t"
                 "call %1\n\t"
                 "movl %%eax, %0\n\t"
                 "add $16, %%esp\n\t"
                 : "=r"(retval)
                 : "r"(syscall_table[intf->eax]), "c"(intf->ecx),
                   "b"(intf->ebx), "d"(intf->edx), "a"(intf)
                 : "memory");
    break;
  }
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
  for (uint i = 0; i < HW_IRQ_COUNT; i++) {
    interrupt_methods[i].func = NULL;
    interrupt_methods[i].pid = 0;
    interrupt_methods[i].avail = FALSE;
    interrupt_suscribed[i] = 0;
  }
  for (uint i = 0; i < 256; i++)
    if (i == SYSCALL_INT)
      make_gate(&idt[i], 1 << 3, (uint)vector_table[i], DPL3, GATE_INT32);
    else
      make_gate(&idt[i], 1 << 3, (uint)vector_table[i], DPL0, GATE_INT32);
  uchar idt_ptr[6];
  u16 *idt_limit = (u16 *)(&idt_ptr[0]);
  u32 *idt_base = (u32 *)(&idt_ptr[2]);
  *idt_limit = count * sizeof(Gate) - 1;
  *idt_base = (uint)idt - KERN_BASE;
  asm volatile("lidt (%0)" ::"r"(idt_ptr));
  init_8259A();
  asm("sti");
}

void enable_irq(uint irq) {
  if (irq < 8)
    outb(IO_PIC_M + 1, inb(IO_PIC_M) & ~(1 << irq));
  else
    outb(IO_PIC_S + 1, inb(IO_PIC_S) & ~(1 << irq));
}

void disable_irq(uint irq) {
  if (irq < 8)
    outb(IO_PIC_M + 1, inb(IO_PIC_M) | (1 << irq));
  else
    outb(IO_PIC_S + 1, inb(IO_PIC_S) | (1 << irq));
}
