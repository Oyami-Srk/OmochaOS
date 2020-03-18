#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#include "generic/typedefs.h"

#define SYSCALL_INT 0xE9

static inline uint get_ticks() {
    volatile uint beats;
    asm volatile("movl $0, %%eax\n\t"
                 "int %1\n\t"
                 "movl %%eax, %0"
                 : "=r"(beats)
                 : "g"(SYSCALL_INT)
                 : "memory", "%eax");
    return beats;
}

#endif
