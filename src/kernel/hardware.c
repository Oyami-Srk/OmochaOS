#include "hardware.h"
#include "asm.h"
#include "config.h"

// Clock PIT
void _init_clock(){
  outb(0x43, 0x34);
  outb(0x40, (ushort)(1193180/10));
  outb(0x40, (ushort)((1193180/10) >> 8));
}

void _init_8259A() {
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
  __asm__("sti");
}


// Initialize hardware
void kinit_hardware(uint hardware){
  switch(hardware){
  case HW_CLOCK:
    _init_clock();
    break;
  case HW_8259A:
    _init_8259A();
    break;
  }
}
