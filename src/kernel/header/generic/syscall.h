#ifndef __SYSCALL_H__
#define __SYSCALL_H__

//#include "generic/syscall.h"
#include "generic/systask.h"
#include "generic/typedefs.h"
#include "lib/string.h"

#define SYSCALL_INT   0xE9
#define MSG_INTERRUPT 0x7FFFFFFF

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

// Packaged syscall

static inline uint get_ticks_msg() {
    message msg;
    msg.type     = GET_TICKS;
    msg.receiver = SYSTASK_PID;
    send_msg(&msg);
    recv_msg(&msg, SYSTASK_PID);
    return msg.major;
}

static inline uint get_pid() {
    message msg;
    msg.receiver = 0;
    send_msg(&msg);
    recv_msg(&msg, 0);
    return msg.receiver;
}

static inline uint reg_proc(const char *name) {
    message msg;
    msg.type     = REG_PROC;
    msg.receiver = SYSTASK_PID;
    if (strlen(name) > 16)
        return 1;
    strcpy(msg.data.b16, (char *)name);
    send_msg(&msg);
    recv_msg(&msg, SYSTASK_PID);
    return msg.major;
}

static inline uint unreg_proc() {
    message msg;
    msg.type     = UNREG_PROC;
    msg.receiver = SYSTASK_PID;
    send_msg(&msg);
    recv_msg(&msg, SYSTASK_PID);
    return msg.major;
}

static inline uint query_proc(const char *name) {
    message msg;
    msg.type     = QUERY_PROC;
    msg.receiver = SYSTASK_PID;
    if (strlen(name) > 16)
        return 0;
    strcpy(msg.data.b16, (char *)name);
    send_msg(&msg);
    recv_msg(&msg, SYSTASK_PID);
    return msg.major;
}

static inline uint reg_int_func(uint irq, void *func) {
    message msg;
    msg.type     = REG_INT_FUNC;
    msg.receiver = SYSTASK_PID;
    if (!func)
        return 1;
    msg.major            = irq;
    msg.data.uint_arr.d1 = (uint)func;
    send_msg(&msg);
    recv_msg(&msg, SYSTASK_PID);
    return msg.major;
}

static inline uint unreg_int_func(uint irq) {
    message msg;
    msg.type     = UNREG_INT_FUNC;
    msg.receiver = SYSTASK_PID;
    msg.major    = irq;
    send_msg(&msg);
    recv_msg(&msg, SYSTASK_PID);
    return msg.major;
}

static inline uint reg_int_msg(uint irq) {
    message msg;
    msg.type     = REG_INT_MSG;
    msg.receiver = SYSTASK_PID;
    msg.major    = irq;
    send_msg(&msg);
    recv_msg(&msg, SYSTASK_PID);
    return msg.major;
}

static inline uint unreg_int_msg() {
    message msg;
    msg.type     = UNREG_INT_MSG;
    msg.receiver = SYSTASK_PID;
    send_msg(&msg);
    recv_msg(&msg, SYSTASK_PID);
    return msg.major;
}

static inline uint peek_msg() {
    message msg;
    msg.type     = PEEK_MSG;
    msg.receiver = SYSTASK_PID;
    send_msg(&msg);
    recv_msg(&msg, SYSTASK_PID);
    return msg.major;
}

#endif
