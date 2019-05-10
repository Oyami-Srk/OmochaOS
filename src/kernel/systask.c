#include "kernel/interrupt.h"
#include "kernel/proc.h"
#include "syscall/syscall.h"

#include "lib/stdlib.h"
#include "lib/string.h"

void SysIdle() {
  while (1)
    ;
}

#define SEND_BACK(msg)                                                         \
  ((msg).receiver = (msg).sender);                                             \
  send_msg(&(msg))

void SysTask() {
  message msg;
  msg.receiver = 1;
  while (1) {
    recv_msg(&msg, ANY);
    switch (msg.type) {
    case GET_TICKS:
      msg.major_data = get_ticks();
      SEND_BACK(msg);
      break;
    case GET_PID:
      msg.major_data = proc_table[msg.sender].pid;
      SEND_BACK(msg);
      break;
    case REG_PROC: {
      char *name = msg.data.m3;
      for (int i = 0; i < proc_table_size; i++)
        if (strcmp(name, proc_table[i].name) == 0 && i != msg.sender) {
          msg.major_data = -1;
          SEND_BACK(msg);
        }
      size_t len = strlen(name);
      strcpy(proc_table[msg.sender].name, name);
      msg.major_data = 0;
      SEND_BACK(msg);
      break;
    }
    case UNREG_PROC:
      memset(proc_table[msg.sender].name, 0, 16); // hardcode todo: change
      msg.major_data = 0;
      SEND_BACK(msg);
      break;
    case QUERY_PROC: {
      uint pid = 0;
      char *name = msg.data.m3;
      for (uint i = 0; i < proc_table_size; i++)
        if (strcmp(name, proc_table[i].name) == 0) {
          pid = i;
          break;
        }
      msg.major_data = pid;
      SEND_BACK(msg);
      break;
    }
    case REG_INT_FUNC:
      if (msg.major_data == 0)
        panic("Cannot assign to clock interrupt!");
      if (msg.major_data >= HW_IRQ_COUNT)
        panic("Cannot assign to interrupt bigger than 16!");
      if (interrupt_methods[msg.major_data].avail == TRUE)
        panic("Someone has been assigned to this interrupt!");
      interrupt_methods[msg.major_data].avail = TRUE;
      interrupt_methods[msg.major_data].pid = msg.sender;
      interrupt_methods[msg.major_data].func = (fp_v_v)msg.data.m1.d1;
      msg.major_data = 0;
      SEND_BACK(msg);
      break;
    case UNREG_INT_FUNC:
      if (msg.major_data == 0)
        panic("Cannot unassign clock interrupt!");
      if (msg.major_data >= HW_IRQ_COUNT)
        panic("Cannot unassign an interrupt bigger than 16!");
      if (interrupt_methods[msg.major_data].avail == FALSE)
        panic("No one has been assigned to this interrupt!");
      if (interrupt_methods[msg.major_data].pid != msg.sender)
        panic("Cannot unassign an interrupt not assigned to you!");
      interrupt_methods[msg.major_data].avail = FALSE;
      interrupt_methods[msg.major_data].pid = 0;
      interrupt_methods[msg.major_data].func = NULL;
      msg.major_data = 0;
      SEND_BACK(msg);
      break;
    default:
      panic("Wrong Type of SysTaskCall");
    }
  }
}