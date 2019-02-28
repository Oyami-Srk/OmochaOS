#include "hardware.h"
#include "asm.h"
#include "config.h"

// Clock PIT
void _init_clock(){
  outb(0x43, 0x34);
  outb(0x40, (ushort)(1193180/10));
  outb(0x40, (ushort)((1193180/10) >> 8));
}

// Initialize hardware
void kinit_hardware(uint hardware){
  switch(hardware){
  case HW_CLOCK:
    _init_clock();
    break;
  }
}
