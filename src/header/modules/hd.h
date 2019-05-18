#ifndef __MODULES_INC_HD__
#define __MODULES_INC_HD__

#include "kernel/type.h"
#include "syscall/syscall.h"

#define DEV_OPEN 1
#define DEV_CLOSE 2
#define DEV_WRITE 3
#define DEV_READ 4

static inline uint HD_dev_open(uint drv) {
  uint task_hd = query_proc("TaskHD");
  message msg;
  msg.type = DEV_OPEN;
  msg.major_data = drv;
  msg.receiver = task_hd;
  send_msg(&msg);
  recv_msg(&msg, task_hd);
  return msg.major_data;
}

static inline uint HD_dev_close(uint drv) {
  uint task_hd = query_proc("TaskHD");
  message msg;
  msg.type = DEV_CLOSE;
  msg.major_data = drv;
  msg.receiver = task_hd;
  send_msg(&msg);
  recv_msg(&msg, task_hd);
  return msg.major_data;
}

static inline size_t HD_dev_read(uint drv, uint addr, ubyte *buf,
                                 size_t count) {
  uint task_hd = query_proc("TaskHD");
  message msg;
  msg.type = DEV_READ;
  msg.major_data = drv;
  msg.data.m1.d1 = (uint)buf;
  msg.data.m1.d2 = addr;
  msg.data.m1.d3 = count;

  msg.receiver = task_hd;
  send_msg(&msg);
  recv_msg(&msg, task_hd);
  return msg.major_data;
}

static inline size_t HD_dev_write(uint drv, uint addr, ubyte *buf,
                                  size_t count) {
  uint task_hd = query_proc("TaskHD");
  message msg;
  msg.type = DEV_WRITE;
  msg.major_data = drv;
  msg.data.m1.d1 = (uint)buf;
  msg.data.m1.d2 = addr;
  msg.data.m1.d3 = count;
  msg.receiver = task_hd;
  send_msg(&msg);
  recv_msg(&msg, task_hd);
  return msg.major_data;
}

#endif // __MODULES_INC_HD__