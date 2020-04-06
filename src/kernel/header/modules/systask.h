#ifndef __MODULE_SYSTASK_H__
#define __MODULE_SYSTASK_H__

#include "generic/typedefs.h"
#include "lib/syscall.h"

#define SYSTASK_PID 2

#define QUERY_PROC_TIMEOUT 5000 // in ms

#define GET_TICKS      1
#define REG_PROC       2
#define UNREG_PROC     3
#define QUERY_PROC     4
#define REG_INT_FUNC   5
#define UNREG_INT_FUNC 6
#define REG_INT_MSG    7
#define UNREG_INT_MSG  8
#define PEEK_MSG       9
#define QUERY_ENV      10
#define EXIT_PROC      11
#define MODIFY_PROC    12
#define ALLOC_PROC     13
#define GET_PROC       14
#define PROC_VIR2PHY   15

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

uint query_proc(const char *name);

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

#define ENV_KEY_MEMORY_LOWER 1
#define ENV_KEY_MEMORY_UPPER 2
#define ENV_KEY_BOOT_INFO    3
#define ENV_KEY_MMAP         4
#define ENV_KEY_MEMORY_USAGE 5

static inline uint query_env(unsigned int KEY, ubyte *buf, size_t buf_size) {
    message msg;
    msg.type             = QUERY_ENV;
    msg.receiver         = SYSTASK_PID;
    msg.major            = KEY;
    msg.data.uint_arr.d1 = (uint)buf;
    msg.data.uint_arr.d2 = (uint)buf_size;
    send_msg(&msg);
    recv_msg(&msg, SYSTASK_PID);
    return msg.major;
}

static inline uint exit_proc() {
    message msg;
    msg.type     = EXIT_PROC;
    msg.receiver = SYSTASK_PID;
    send_msg(&msg);
    return 0; // actually you cannot run into this line
}

#define MOD_PROC_CR3 1

static inline uint modify_proc(uint pid, uint KEY, uint major, ubyte *buf,
                               size_t buf_size) {
    message msg;
    msg.type             = MODIFY_PROC;
    msg.receiver         = SYSTASK_PID;
    msg.major            = KEY;
    msg.data.uint_arr.d1 = (uint)major;
    msg.data.uint_arr.d2 = (uint)buf;
    msg.data.uint_arr.d3 = (uint)buf_size;
    msg.data.uint_arr.d4 = (uint)pid;
    send_msg(&msg);
    recv_msg(&msg, SYSTASK_PID);
    return msg.major;
}

// return a free proc slot with pid set
static inline process *alloc_proc() {
    message msg;
    msg.type     = ALLOC_PROC;
    msg.receiver = SYSTASK_PID;
    send_msg(&msg);
    recv_msg(&msg, SYSTASK_PID);
    return (process *)msg.major;
}

static inline process *get_proc(uint pid) {
    message msg;
    msg.type     = GET_PROC;
    msg.major    = pid;
    msg.receiver = SYSTASK_PID;
    send_msg(&msg);
    recv_msg(&msg, SYSTASK_PID);
    return (process *)msg.major;
}

static inline char *proc_vir2phy(pid_t pid, char *va) {
    message msg;
    msg.type             = PROC_VIR2PHY;
    msg.major            = pid;
    msg.data.uint_arr.d1 = (uint)va;
    msg.receiver         = SYSTASK_PID;
    send_msg(&msg);
    recv_msg(&msg, SYSTASK_PID);
    return (char *)msg.major;
}

#endif // __MODULE_SYSTASK_H__
