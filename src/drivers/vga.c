#include "driver/vga.h"
#include "lib/asm.h"
#include "lib/stdlib.h"
#include "lib/string.h"

const volatile char *vm_start = (volatile char *)0xB8000;

uint disp_pos = 0;

void kprintf(const char *fmt, ...) {
  int i;
  char buf[256];
  va_list arg = (va_list)((char *)(&fmt) + 4);
  i = vsprintf(buf, fmt, arg);
  buf[i] = 0;
  disp_pos =
      VGA_write_color_string_to_vm(disp_pos, COLOR(BLACK, WHITE | BRIGHT), buf);
}

uint VGA_write_color_string_to_vm(uint offset, ushort color, const char *str) {
  volatile char *pCh = (volatile char *)((uint)vm_start + offset);
  char ch = *str;
  while (*str != 0) {
    switch (ch) {
    case '\n':
      pCh = (volatile char *)(vm_start +
                              SCREEN_WIDTH * 2 *
                                  (((uint)pCh - (uint)vm_start) / 160 + 1));
      break;
    case '\b':
      if (pCh - 2 < vm_start)
        break;
      *(pCh - 2) = ' ';
      *(pCh - 1) = color;
      pCh -= 2;
      break;
    default:
      *pCh++ = ch;
      *pCh++ = color;
      break;
    }
    ch = *++str;
  }
  if (*(pCh + 1) == 0)
    *(pCh + 1) = color; // make cursor visible
  return (uint)pCh - (uint)vm_start;
}

void VGA_set_vm_display_addr(uint offset) {
  // cli
  outb(0x3D4, 0x0D);
  outb(0x3D5, (u16)((offset / 2) & 0xFF));
  outb(0x3D4, 0x0C);
  outb(0x3D5, (u16)(((offset / 2) >> 8) & 0xFF));
  // sti
}

void VGA_set_cursor_addr(uint offset) {
  // cli
  outb(0x3D4, 0x0F);
  outb(0x3D5, (u16)((offset / 2) & 0xFF));
  outb(0x3D4, 0x0E);
  outb(0x3D5, (u16)(((offset / 2) >> 8) & 0xFF));
  // sti
}

void VGA_init(void) {
  memset((void *)vm_start, 0, 32 * 1024); // 32KB video memory
  VGA_set_cursor_addr(0);
  VGA_set_vm_display_addr(0);
}