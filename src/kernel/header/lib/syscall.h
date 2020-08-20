#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#include <generic/typedefs.h>
#include <lib/string.h>

#define SYSCALL_INT   0xE9
#define MSG_INTERRUPT 0x7FFFFFFF
#define MSG_EXCEPTION 0x7EEEEEEE
#ifndef PROC_ANY
#define PROC_ANY 0xFFFFFFFE
#endif
#ifndef PROC_INTERRUPT
#define PROC_INTERRUPT 0xFFFFFFFF
#endif

// True syscall

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

static inline uint send_msg(message *msg) {
    volatile uint rv;
    asm volatile("movl %%ecx, %%ecx\n\t" ::"c"(msg));
    asm volatile("movl $1, %%eax\n\t"
                 "int %1\n\t"
                 "movl %%eax, %0"
                 : "=r"(rv)
                 : "g"(SYSCALL_INT)
                 : "memory", "%eax");
    return rv;
}

static inline uint recv_msg(message *msg, uint recv_from) {
    msg->major = 0;
    volatile uint rv;
    asm volatile("movl %%ecx, %%ecx\n\t" ::"c"(msg));
    asm volatile("movl %%ebx, %%ebx\n\t" ::"b"(recv_from));
    asm volatile("movl $2, %%eax\n\t"
                 "int %1\n\t"
                 "movl %%eax, %0"
                 : "=r"(rv)
                 : "g"(SYSCALL_INT)
                 : "memory", "%eax");
    return rv;
}

#define SEND_BACK(msg)                                                         \
    ((msg).receiver = (msg).sender);                                           \
    send_msg(&(msg))

#endif
