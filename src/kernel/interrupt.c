#include "kernel/interrupt.h"
#include "kernel/asm.h"
#include "kernel/klib.h"
#include "kernel/process.h"
#include "kernel/protect.h"

#include "modules/modules.h"

#include "syscall.h"

const char* exception_message[] = {
                                   "#DE: Divide-by-zero Error",
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

extern void* syscall_table[];

static inline void EOI_M(void) { outb(IO_PIC_M, 0x20); }

static inline void EOI_S(void) { outb(IO_PIC_S, 0x20); }

extern void switch_to(process *p);
extern cpu_env cpu;
struct interrupt_method interrupt_methods[HW_IRQ_COUNT];

uint send_int_to_proc(uint irq, uchar data){ // timer is 0, return 1 means no proc is assinged
  if(interrupt_methods[irq].avail == 0)
    return 1;
  if(interrupt_methods[irq].method == INT_HANDLE_METHOD_DIRECT)
    return 1;
  assert(interrupt_methods[irq].pid < PROC_COUNT);
  disable_irq(irq);

  if(interrupt_methods[irq].method == INT_HANDLE_METHOD_DISPATCH){
    process *p = &cpu.processes[interrupt_methods[irq].pid];
    if(p->status & PROC_STATUS_RECEVING){
      if(GET_PROC_STATUS_PID(p) == 0 || GET_PROC_STATUS_PID(p) == INTERRUPT){
        p->p_msg->sender = INTERRUPT;
        p->p_msg->type = INTERRUPT;
        p->p_msg->major_data = INTERRUPT - 1;
        p->status &= ~PROC_STATUS_RECEVING;
      }else{
        ; // 虽然正在接受消息但是不是接受任意或者中断消息
      }
      ; // 正在接受，所以这里要填充消息
      return 0;
    }else if(p->status & PROC_STATUS_SENDING){
      ; // 这里不需要任何，标记一下就行
      goto mark;
    }else {
      ; // 没在接受，直接标记goto mark;
    }
  mark:
    p->status |= PROC_STATUS_GOTINT;
    /* kprintf("!%d!", interrupt_map[irq]); */
    return 0;
  } else if(interrupt_methods[irq].method == INT_HANDLE_METHOD_CIRCULAR_BUFFER){
    write_circular_buffer_b((Circular_BufferB*)interrupt_methods[irq].data, data);
    enable_irq(irq);
    return 0;
  }
  return 1;
}

void interrupt_handler(stack_frame *intf) {
  if (intf->trapno <= 19) {
    cli();
    extern volatile char *pDisp;
    pDisp = (volatile char *)0xB8000;
    for (uint i = 0; i < 80; i++)
      write_string(0x1C, " ");
    pDisp = (volatile char *)0xB8000;
    ckprintf(0x1C, "Exception %s in proc %d", exception_message[intf->trapno],
             ((uint)intf - (uint)cpu.processes) / sizeof(process));
    while (1)
      ;
    return;
  }
  switch (intf->trapno) {
  case IRQ_TIMER: {
    cpu.beats++;
    volatile char *ch = (volatile char *)(0xB8000);
    *ch = *ch + 1;
    kreload_process();
    EOI_M();
    break;
  }
  case IRQ_KBD: {
    if (send_int_to_proc(IRQ_KBD - IRQ0, inb(0x60)) == 1) {
      /* uchar data = inb(0x60); */
      /* kprintf("0x%02x ", data); */
      ;
    }
    EOI_M();
    break;
  }
  case SYSCALL_INT: {
    volatile uint ret_val = 0;
    __asm__ __volatile__("push %%edx\n\t"
                         "push %%ebx\n\t"
                         "push %%ecx\n\t"
                         "push %%eax\n\t"
                         "call %1\n\t"
                         "movl %%eax, %0\n\t"
                         "add $16, %%esp"
                         : "=r"(ret_val)
                         : "r"(syscall_table[intf->eax]), "c"(intf->ecx),
                           "b"(intf->ebx), "d"(intf->edx), "a"(intf)
                         : "memory");
    break;
  }
  }
  return;
}

int i = -1;
#define UNRUNABLE (PROC_STATUS_RECEVING | PROC_STATUS_SENDING)

void kreload_process() {
  if (!cpu.current_running_proc)
    return;

  /* if(0){ */
  /*   kprintf("["); */
  /*   for(int i = 0; i < PROC_COUNT; i++) */
  /*     kprintf("%x,", cpu.processes[i].status & 0xFF); */
  /*   kprintf("] "); */
  /* } */

  do {
    i++;
    if (i >= PROC_COUNT)
      i = 0;
  } while ((cpu.processes[i].status & 0xFF) & UNRUNABLE);
  cpu.current_running_proc = (uint)&cpu.processes[i];

  /* if (cpu.processes[i].status & PROC_STATUS_RUNNING) */
  /*   cpu.current_running_proc = (uint)&cpu.processes[i]; */
  /* i++; */
  /* if (i >= PROC_COUNT) */
  /*   i = 0; */
}

void init_8259A() {
  outb(IO_PIC_M, 0x11);         // ICW 1
  outb(IO_PIC_S, 0x11);         // ICW 1
  outb(IO_PIC_M + 1, IRQ0);     // 0x20 -> Master first
  outb(IO_PIC_S + 1, IRQ0 + 8); // 0x28 -> Salve first

  outb(IO_PIC_M + 1, 0x4); // ICW 3
  outb(IO_PIC_S + 1, 0x2); // ICW 3
  outb(IO_PIC_M + 1, 0x1);
  outb(IO_PIC_S + 1, 0x1);

  outb(IO_PIC_M + 1, 0xFF);
  outb(IO_PIC_S + 1, 0xFF);
}

void kinit_interrupt(cpu_env *env) {
  memset(&interrupt_methods, 0, sizeof(struct  interrupt_method) * HW_IRQ_COUNT);
  for (int i = 0; i < IDT_COUNT; i++)
    if (i == SYSCALL_INT)
      MAKE_GATE(&env->idt[i], SEL_KCODE, vector_table[i], DPL3, GATE_INT32);
    else
      MAKE_GATE(&env->idt[i], SEL_KCODE, vector_table[i], DPL0, GATE_INT32);

  unsigned char idt_ptr[6];
  unsigned short *idt_limit = (unsigned short *)(&idt_ptr[0]);
  unsigned int *idt_base = (unsigned int *)(&idt_ptr[2]);
  *idt_limit = sizeof(env->idt) * sizeof(Gate) - 1;
  *idt_base = (unsigned int)env->idt;

  __asm__ __volatile__("lidt (%0)" ::"r"(idt_ptr));
  init_8259A();
  __asm__ __volatile__("sti"); // open interrupt closed in boot
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
