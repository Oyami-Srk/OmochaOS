/*
module:
  name: SysTask
  author: Shiroko
  summary: System Task's process
  entry: SysTask
  preferred_pid: 1
*/

#include "generic/systask.h"
#include "core/process.h"
#include "driver/graphic.h"
#include "generic/syscall.h"
#include "lib/stdlib.h"
#include "lib/string.h"

void SysTask() {
    message msg;
    msg.receiver = 0;
    // send msg to proc 0 (System idle) to get back
    // to check our pid is 1
    send_msg(&msg);
    recv_msg(&msg, 0);
    if (msg.receiver != SYSTASK_PID)
        panic("Systask pid is not 1");
    while (1) {
        recv_msg(&msg, PROC_ANY);
        switch (msg.type) {
        default:
            break;
        }
    }
}
