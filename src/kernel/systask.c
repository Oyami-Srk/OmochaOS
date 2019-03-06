#include "syscall.h"
#include "string.h"
#include "kernel/klib.h"
#include "kernel/process.h"

void SysIdle() {
  while(1);
}

void SysTask() {
  message msg;
  msg.receiver = 1;
  while(1) {
    kprintf(".");
    recv_msg(&msg, ANY);

    switch(msg.type){
    case SC_GET_TICKS:
      msg.major_data = get_ticks();
      msg.type = SC_DONE;
      kprintf("[%d]", msg.sender);
      send_msg(&msg);
      break;
    default:
      /* panic("Unknown Msg! baka! % >_< %"); */
      kprintf("!");
    }
  }
}
