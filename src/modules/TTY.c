/*
module:
  name: TTY
  author: Shiroko
  summary: TTY Process
  module_task: Task_TTY
*/
#include "modules/modules.h"

extern int kprintf(const char *fmt, ...);

void Task_TTY() {
  message msg;
  while(1){
    msg.receiver = 4;
    msg.major_data = 0;
    msg.type = 1;
    kprintf(".");
    send_msg(&msg);
    recv_msg(&msg, 4);
    if(msg.major_data & 0x0100){
      kprintf("[%c]", msg.major_data);
    }
  }
}

