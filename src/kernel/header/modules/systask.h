#ifndef __MODULE_SYSTASK_H__
#define __MODULE_SYSTASK_H__

#include "generic/typedefs.h"
#include "lib/syscall.h"

#define SYSTASK_PID 1

#define GET_TICKS      1
#define REG_PROC       2
#define UNREG_PROC     3
#define QUERY_PROC     4
#define REG_INT_FUNC   5
#define UNREG_INT_FUNC 6
#define REG_INT_MSG    7
#define UNREG_INT_MSG  8
#define PEEK_MSG       9

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

#endif // __MODULE_SYSTASK_H__
