#include "syscall/syscall.h"
#include "const.h"

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

uint add(uint v2, uint v3, uint v4) {
  volatile uint rv;
  asm volatile("movl %%ecx, %%ecx\n\t" ::"c"(v2));
  asm volatile("movl %%ebx, %%ebx\n\t" ::"b"(v3));
  asm volatile("movl %%edx, %%edx\n\t" ::"d"(v4));
  asm volatile("movl $1, %%eax\n\t"
               "int $0xE9\n\t"
               "movl %0, %%eax\n\t"
               : "=r"(rv)
               :
               : "memory");
  return rv;
}