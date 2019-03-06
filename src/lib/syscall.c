#include "syscall.h"
#include "kernel/klib.h"

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
}
