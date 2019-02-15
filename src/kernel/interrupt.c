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

void interrupt_handler(interrupt_frame *intf) {
  switch(intf->trapno){
  case IRQ_TIMER:{
    volatile char *ch = (volatile char*)(0xB8000);
    *ch = *ch + 1;
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
     MAKE_GATE(&env->idt[i], SEL_CODE, vector_table[i], DPL0, GATE_INT32);


   unsigned char idt_ptr[6];

   unsigned short *idt_limit = (unsigned short*)(&idt_ptr[0]);
   unsigned int *idt_base = (unsigned int*)(&idt_ptr[2]);
   *idt_limit = sizeof(env->idt) * sizeof(Gate_Descriptor) - 1;
   *idt_base = (unsigned int)env->idt;

   __asm__ __volatile__("lidt (%0)" :: "r"(idt_ptr));
   init_8259A();
   __asm__ __volatile__("sti"); // open interrupt closed in boot

 }

