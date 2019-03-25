/*
module:
  name: Monitor
  author: Shiroko
  summary: Module for manipulate monitor to display something
  module_task: Task_Monitor
*/

#include "modules/modules.h"
#include "kernel/klib.h"
#include "Monitor.h"

#define VM_SIZE (32 * 1024)
#define CON_SIZE (SCREEN_SIZE * 2)
#define CON_COUNT (VM_SIZE / CON_SIZE)
console cons[CON_COUNT];

extern void delay_ms(uint);

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
}

void puts_monitor(console *con, char *s);
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

void Task_Monitor(){
  disp_color = COLOR(BLACK, WHITE);
  message msg;
  if(register_proc("TaskMonitor") != 0)
    kprintf("Cannot register proc as TaskMonitor");
  while(1){
    recv_msg(&msg, ANY);
    switch(msg.type){
    case MSG_MONITOR_INIT:
      initialize_monitor();
      flush_scr(&cons[0]);
      break;
    case MSG_MONITOR_FLUSH:
      if(msg.major_data < CON_COUNT)
        flush_scr(&cons[msg.major_data]);
      break;
    case MSG_MONITOR_SETCOLOR:
      disp_color = (ushort)msg.major_data;
      break;
    case MSG_MONITOR_PUTC:
      putc_monitor(&cons[msg.major_data], msg.data.m3[0]);
      break;
    case MSG_MONITOR_PUTS:
      puts_monitor(&cons[msg.major_data], (char*)msg.data.m1.d1);
      break;
    case MSG_MONITOR_CLRSCR:
      memset(cons[msg.major_data].vm_start, 0, 80 * 25 * 2);
      break;
    case MSG_MONITOR_SWITCH:{
      console *pCur = &cons[msg.major_data];
      pCur->vm_disp = pCur->vm_disp == pCur->vm_start ? pCur->vm_start + 80 * 25 * 2 : pCur->vm_start;
      break;
    }
    }
  }
}
