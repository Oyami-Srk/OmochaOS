#include "kernel/hardware.h"
#include "const.h"
#include "lib/asm.h"

void _init_clock() {
  outb(0x43, 0x34);
  outb(0x40, (ushort)((FREQ_8253 / SYSTEM_CLOCK) & 0xFF));
  outb(0x40, (ushort)(((FREQ_8253 / SYSTEM_CLOCK) >> 8) & 0xFF));
}

void kinit_hardware() {
  _init_clock();
  return;
}