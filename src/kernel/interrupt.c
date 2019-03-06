#include "kernel/interrupt.h"
#include "kernel/asm.h"
#include "kernel/klib.h"
#include "kernel/process.h"
#include "kernel/protect.h"

#include "syscall.h"
extern void* syscall_table[];


#define __EXCEPTION_CASE__                                                     \
  case INT_F_DE:                                                               \
    panic("#DE");                                                              \
  case INT_FT_DB:                                                              \
    panic("#DB");                                                              \
  case INT_I_UCI:                                                              \
    panic("Undisabled Interrupt");                                             \
  case INT_T_BP:                                                               \
    panic("#BP");                                                              \
  case INT_T_OF:                                                               \
    panic("#OF");                                                              \
  case INT_F_BR:                                                               \
    panic("#BR");                                                              \
  case INT_F_UD:                                                               \
    panic("#UD");                                                              \
  case INT_F_NM:                                                               \
    panic("#NM");                                                              \
  case INT_A_DF:                                                               \
    panic("#DF");                                                              \
  case INT_F_RESV:                                                             \
    panic("Resv-9");                                                           \
  case INT_F_TS:                                                               \
    panic("#TS");                                                              \
  case INT_F_NP:                                                               \
    panic("#NP");                                                              \
  case INT_F_SS:                                                               \
    panic("#SS");                                                              \
  case INT_F_GP:                                                               \
    panic("#GP");                                                              \
  case INT_F_PF:                                                               \
    panic("#PF");                                                              \
  case INT_F_RESV2:                                                            \
    panic("Resv-15");                                                          \
  case INT_F_MF:                                                               \
    panic("#MF");                                                              \
  case INT_F_AC:                                                               \
    panic("#AC");                                                              \
  case INT_A_MC:                                                               \
    panic("#MC");                                                              \
  case INT_F_XF:                                                               \
    panic("#XF");

static inline void EOI_M(void) { outb(IO_PIC_M, 0x20); }

static inline void EOI_S(void) { outb(IO_PIC_S, 0x20); }

extern void switch_to(process *p);
extern cpu_env cpu;

void interrupt_handler(stack_frame *intf) {
  switch (intf->trapno) {
    __EXCEPTION_CASE__
  case IRQ_TIMER: {
    cpu.beats++;
    volatile char *ch = (volatile char *)(0xB8000);
    *ch = *ch + 1;
    kreload_process();
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
  if(!cpu.current_running_proc)
    return;

  do {
    i++;
    if(i >= PROC_COUNT)
      i = 0;
  }while(cpu.processes[i].status & UNRUNABLE);
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

  outb(IO_PIC_M + 1, 0xFE);
  outb(IO_PIC_S + 1, 0xFF);
}

void kinit_interrupt(cpu_env *env) {
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
