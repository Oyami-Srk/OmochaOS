#include "kernel/klib.h"
#include "kernel/asm.h"
#include "string.h"
#include "Monitor.h"

console cons[CON_COUNT];

extern volatile char* pDisp;
ushort disp_color;

void set_cursor(console *con) {
  push_cli();
  uint pos = ((uint)con->pCh - con->vm_start) / 2;
  outb(0x3D4, 0x0F);
  outb(0x3D5, (ushort)(pos & 0xFF));
  outb(0x3D4, 0x0E);
  outb(0x3D5, (ushort)((pos >> 8) & 0xFF));
  push_sti();
}

void set_vm_disp(console *con) {
  push_cli();
  uint pos = (con->vm_disp - 0xB8000) / 2;
  outb(0x3D4, 0xD);
  outb(0x3D5, (ushort)(pos & 0xFF));
  outb(0x3D4, 0xC);
  outb(0x3D5, (ushort)((pos >> 8) & 0xFF));
  push_sti();
}

void flush_scr(console *con){
  set_cursor(con);
  set_vm_disp(con);
}

void initialize_monitor(){
  memset(cons, 0, sizeof(console) * CON_COUNT);
  cons[0].vm_disp = cons[0].vm_start = 0xB8000;
  cons[0].vm_end = cons[0].vm_start + SCREEN_SIZE * 2 * 2;
  cons[0].pCh = (uchar *)0xB8000;
  for(uint i = 1; i < CON_COUNT; i++) {
    cons[i].vm_disp = cons[i].vm_start = cons[i - 1].vm_end;
    cons[i].vm_end = cons[i].vm_start + SCREEN_SIZE * 2 * 2;
    cons[i].pCh = (uchar *)cons[i].vm_start;
  }
  memset((uint*)cons[0].vm_start, 0, VM_SIZE);
  flush_scr(&cons[0]);
  disp_color = COLOR(BLACK, WHITE);
}

void putc_monitor(console *con, char c){
  char buf[2];
  buf[0] = c;
  buf[1] = 0;
  puts_monitor(con, buf);
}

void puts_monitor(console *con, char *s){
  char ch = *s;
  uchar *pCh = con->pCh;
  while(*s != 0){
    if(pCh >= con->vm_end){
      pCh = con->vm_start;
      memset(pCh, 0, SCREEN_SIZE * 2);
    }
    switch(ch){
    case '\n':
      pCh = (uchar*)con->vm_start +
        SCREEN_WIDTH * 2 * (((int)pCh - con->vm_start) / 160 + 1);
      break;
    case '\b':
      *(pCh-2) = ' ';
      *(pCh-1) = disp_color;
      pCh-=2;
      break;
    default:
      *pCh++ = ch;
      *pCh++ = disp_color;
    }
    ch = *++s;
  }
  if(*(pCh+1) == 0)
    *(pCh + 1) = disp_color;
  con->pCh = pCh;
}

int monitor_printf(uint con_cr, const char *fmt, ...){
  int i;
  char buf[256];
  va_list arg = (va_list)((char*)(&fmt) + 4);
  i = vsprintf(buf, fmt, arg);
  puts_monitor(&cons[con_cr], buf);
  return i;
}

console* get_con(unsigned int nr_con){
  return &cons[nr_con];
}

void switch_monitor(console *con){
  con->vm_disp = con->vm_disp == con->vm_start ? con->vm_start + 80 * 25 * 2 : con->vm_start;
}
