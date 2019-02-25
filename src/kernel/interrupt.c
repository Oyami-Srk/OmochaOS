#include "interrupt.h"
#include "klib.h"
#include "protect.h"
#include "asm.h"

static inline void EOI_M(void){
  outb(IO_PIC_M, 0x20);
}

static inline void EOI_S(void){
  outb(IO_PIC_S, 0x20);
}

extern void switch_to(process *p);
uint i = 0;
extern cpu_env cpu;

void interrupt_handler(stack_frame *intf) {
  switch(intf->trapno){
  case INT_F_DE:
    panic("#DE");
  case INT_FT_DB:
    panic("#DB");
  case INT_I_UCI:
    panic("Undisabled Interrupt");
  case INT_T_BP:
    panic("#BP");
  case INT_T_OF:
    panic("#OF");
  case INT_F_BR:
    panic("#BR");
  case INT_F_UD:
    panic("#UD");
  case INT_F_NM:
    panic("#NM");
  case INT_A_DF:
    panic("#DF");
  case INT_F_RESV:
    panic("Resv-9");
  case INT_F_TS:
    panic("#TS");
  case INT_F_NP:
    panic("#NP");
  case INT_F_SS:
    panic("#SS");
  case INT_F_GP:
    panic("#GP");
  case INT_F_PF:
    panic("#PF");
  case INT_F_RESV2:
    panic("Resv-15");
  case INT_F_MF:
    panic("#MF");
  case INT_F_AC:
    panic("#AC");
  case INT_A_MC:
    panic("#MC");
  case INT_F_XF:
    panic("#XF");
  case IRQ_TIMER:{
    volatile char *ch = (volatile char*)(0xB8000);
    *ch = *ch + 1;
    cpu.current_running_proc = (uint)&cpu.processes[i];
    i++;
    if(i >= PROC_COUNT)
      i = 0;
    EOI_M();
    break;
  }
  }
  return;
}


void init_8259A(){
  outb(IO_PIC_M, 0x11); // ICW 1
  outb(IO_PIC_S, 0x11); // ICW 1
  outb(IO_PIC_M + 1, IRQ0); // 0x20 -> Master first
  outb(IO_PIC_S + 1, IRQ0 + 8); // 0x28 -> Salve first

  outb(IO_PIC_M + 1, 0x4); // ICW 3
  outb(IO_PIC_S + 1, 0x2); // ICW 3
  outb(IO_PIC_M + 1, 0x1);
  outb(IO_PIC_S + 1, 0x1);

  outb(IO_PIC_M + 1, 0xFE);
  outb(IO_PIC_S + 1, 0xFF);
}


 void kinit_interrupt(cpu_env *env){
   for(int i = 0; i < IDT_COUNT; i++)
     MAKE_GATE(&env->idt[i], SEL_KCODE, vector_table[i], DPL0, GATE_INT32);


   unsigned char idt_ptr[6];

   unsigned short *idt_limit = (unsigned short*)(&idt_ptr[0]);
   unsigned int *idt_base = (unsigned int*)(&idt_ptr[2]);
   *idt_limit = sizeof(env->idt) * sizeof(Gate) - 1;
   *idt_base = (unsigned int)env->idt;

   __asm__ __volatile__("lidt (%0)" :: "r"(idt_ptr));
   init_8259A();
   __asm__ __volatile__("sti"); // open interrupt closed in boot

 }

