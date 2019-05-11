#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#include "kernel/interrupt.h"
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
#define REG_INT_FUNC 6
#define UNREG_INT_FUNC 7
#define REG_INT_MSG 8
#define UNREG_INT_MSG 9
#define PEEK_MSG 10
uint get_ticks_msg();
uint get_pid();
uint reg_proc(const char *name);
uint unreg_proc();
uint query_proc(const char *name);
uint reg_int_func(uint irq, fp_v_v func);
uint unreg_int_func(uint irq);
uint reg_int_msg(uint irq);
uint unreg_int_msg();
uint peek_msg();
int printf(const char *fmt, ...);

#endif // __SYSCALL_H__