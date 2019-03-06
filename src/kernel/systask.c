#include "syscall.h"
#include "string.h"
#include "kernel/klib.h"
#include "kernel/process.h"

void SysTask() {
  message msg;
  msg.receiver = 1;
  while(1) {
    recv_msg(&msg, ANY);

    switch(msg.type){
    case SC_GET_TICKS:
      msg.major_data = get_ticks();
      msg.type = SC_DONE;
      send_msg(&msg);
      break;
    default:
      /* panic("Unknown Msg! baka! % >_< %"); */
      ;
    }
  }
}
