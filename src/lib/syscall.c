#include "syscall/syscall.h"
#include "const.h"
#include "kernel/structs.h"
#include "lib/string.h"

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

// SysTask provide

uint get_ticks_msg() {
  message msg;
  msg.type = GET_TICKS;
  msg.receiver = SYSTASK_PID;
  send_msg(&msg);
  recv_msg(&msg, SYSTASK_PID);
  return msg.major_data;
}

uint get_pid() {
  message msg;
  msg.type = GET_PID;
  msg.receiver = SYSTASK_PID;
  send_msg(&msg);
  recv_msg(&msg, SYSTASK_PID);
  return msg.major_data;
}
uint reg_proc(const char *name) {
  message msg;
  msg.type = REG_PROC;
  msg.receiver = SYSTASK_PID;
  if (strlen(name) > 16)
    return 1;
  strcpy(msg.data.m3, (char *)name);
  send_msg(&msg);
  recv_msg(&msg, SYSTASK_PID);
  return msg.major_data;
}

uint unreg_proc() {
  message msg;
  msg.type = UNREG_PROC;
  msg.receiver = SYSTASK_PID;
  send_msg(&msg);
  recv_msg(&msg, SYSTASK_PID);
  return msg.major_data;
}
uint query_proc(const char *name) {
  message msg;
  msg.type = QUERY_PROC;
  msg.receiver = SYSTASK_PID;
  if (strlen(name) > 16)
    return 1;
  strcpy(msg.data.m3, (char *)name);
  send_msg(&msg);
  recv_msg(&msg, SYSTASK_PID);
  return msg.major_data;
}
uint reg_int_func(uint irq, fp_v_v func) {
  message msg;
  msg.type = REG_INT_FUNC;
  msg.receiver = SYSTASK_PID;
  if (!func)
    return 1;
  msg.major_data = irq;
  msg.data.m1.d1 = (uint)func;
  send_msg(&msg);
  recv_msg(&msg, SYSTASK_PID);
  return msg.major_data;
}
uint unreg_int_func(uint irq) {
  message msg;
  msg.type = UNREG_INT_FUNC;
  msg.receiver = SYSTASK_PID;
  msg.major_data = irq;
  send_msg(&msg);
  recv_msg(&msg, SYSTASK_PID);
  return msg.major_data;
}

uint reg_int_msg(uint irq) {
  message msg;
  msg.type = REG_INT_MSG;
  msg.receiver = SYSTASK_PID;
  msg.major_data = irq;
  send_msg(&msg);
  recv_msg(&msg, SYSTASK_PID);
  return msg.major_data;
}

uint unreg_int_msg() {
  message msg;
  msg.type = UNREG_INT_MSG;
  msg.receiver = SYSTASK_PID;
  send_msg(&msg);
  recv_msg(&msg, SYSTASK_PID);
  return msg.major_data;
}
