#include "syscall.h"
#include "kernel/klib.h"
#include "modules/modules.h"

uint get_ticks() {
  // this is reserve for some reason
  volatile uint beats;
  __asm__("movl $0, %%eax\n\t"
          "int $0xE9\n\t"
          "movl %0, %%eax"
          : "=r"(beats)
          :
          : "memory");
  return beats;
}

uint send_msg(message *msg) {
  volatile uint rv;
  __asm__ __volatile__("movl %%ecx, %%ecx\n\t" ::"c"(msg));
  __asm__ __volatile__("movl $1, %%eax\n\t"
                       "int $0xE9\n\t"
                       "movl %0, %%eax"
                       : "=r"(rv)
                       :
                       : "memory");
  return rv;
}

uint recv_msg(message *msg, uint recv_from) {
  volatile uint rv;
  __asm__ __volatile__("movl %%ecx, %%ecx\n\t" ::"c"(msg));
  __asm__ __volatile__("movl %%ebx, %%ebx\n\t" ::"b"(recv_from));
  __asm__ __volatile__("movl $2, %%eax\n\t"
                       "int $0xE9\n\t"
                       "movl %0, %%eax"
                       : "=r"(rv)
                       :
                       : "memory");
  return rv;
}

// fake syscall
uint get_ticks_msg() {
  message msg;
  msg.type = SC_GET_TICKS;
  msg.major_data = 0;
  msg.receiver = 1;
  send_msg(&msg);
  recv_msg(&msg, 1);
  if(msg.type == SC_DONE)
    return msg.major_data;
  else
    panic("Failed get msg from pid 1(systask)");
  return 0;
}


int printf(char *fmt, ...){
  int i;
  char buf[256];
  va_list arg = (va_list)((char *)(&fmt) + 4);
  i = vsprintf(buf, fmt, arg);
  message msg;
  msg.receiver = 1;
  msg.type = SC_WRITE;
  msg.major_data = 0;
  msg.data.m1.d1 = (uint)buf;
  send_msg(&msg);
  return i;
}

