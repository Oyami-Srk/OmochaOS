#include "syscall/syscall.h"
#include "const.h"
#include "kernel/structs.h"

#define STR(x) #x

uint get_ticks() {
  volatile uint beats;
  asm("movl $0, %%eax\n\t"
      "int %1\n\t"
      "movl %0, %%eax"
      : "=r"(beats)
      : "g"(SYSCALL_INT)
      : "memory");
  return beats;
}

uint send_msg(message *msg) {
  volatile uint rv;
  asm volatile("movl %%ecx, %%ecx\n\t" ::"c"(msg));
  asm volatile("movl $1, %%eax\n\t"
               "int %1\n\t"
               "movl %0, %%eax"
               : "=r"(rv)
               : "g"(SYSCALL_INT)
               : "memory");
  return rv;
}

uint recv_msg(message *msg, uint recv_from) {
  volatile uint rv;
  asm volatile("movl %%ecx, %%ecx\n\t" ::"c"(msg));
  asm volatile("movl %%ebx, %%ebx\n\t" ::"b"(recv_from));
  asm volatile("movl $2, %%eax\n\t"
               "int %1\n\t"
               "movl %0, %%eax"
               : "=r"(rv)
               : "g"(SYSCALL_INT)
               : "memory");
  return rv;
}