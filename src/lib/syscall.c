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