#include "syscall.h"
#include "string.h"
#include "kernel/klib.h"
#include "kernel/process.h"
#include "kernel/interrupt.h"

// systask is the only one (maybe not current yet) module built within kernel binary
// and use share resource with kernel
// but running on ring-1
// tasks running on ring-1
// user-tasks running on ring-3

extern cpu_env cpu;

void SysIdle() {
  while(1);
}

void SysTask() {
  message msg;
  msg.receiver = 1;
  while(1) {
    recv_msg(&msg, ANY);

    switch(msg.type){
    case SC_GET_TICKS:
      msg.major_data = get_ticks();
      msg.type = SC_DONE;
      msg.receiver = msg.sender;
      send_msg(&msg);
      break;
    case SC_SET_IRQ_FUNC:

    default:
      panic("Unknown Msg! baka! % >_< %");
      kprintf("!");
    }
  }
}
