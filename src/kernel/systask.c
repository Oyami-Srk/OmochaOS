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
    default:
      panic("Wrong Type of SysTaskCall");
    }
  }
}