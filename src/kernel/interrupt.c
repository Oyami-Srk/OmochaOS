#include "interrupt.h"
#include "asm.h"

void interrupt_handler() {
  while(1);
  return;
}

void init_8259A(){
  outb(IO_PIC_M, 0x11); // ICW 1
  outb(IO_PIC_S, 0x11); // ICW 1
  outb(IO_PIC_M + 1, 0x20); // 0x20 -> Master first
  outb(IO_PIC_S + 1, 0x28); // 0x28 -> Salve first

  outb(IO_PIC_M + 1, 0x4); // ICW 3
  outb(IO_PIC_S + 1, 0x2); // ICW 3
  outb(IO_PIC_M + 1, 0x1);
  outb(IO_PIC_S + 1, 0x1);

  outb(IO_PIC_M + 1, 0xFF);
  outb(IO_PIC_S + 1, 0xFF);
}

