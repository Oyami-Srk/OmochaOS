#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#include "kernel/structs.h"
#include "kernel/type.h"

uint get_ticks();
uint send_msg(message *msg);
uint recv_msg(message *msg, uint recv_from);

// SysTask provide

#define SYSTASK_PID 1

#define GET_TICKS 1
#define GET_PID 2
#define REG_PROC 3
#define UNREG_PROC 4
#define QUERY_PROC 5
uint get_ticks_msg();
uint get_pid();

#endif // __SYSCALL_H__