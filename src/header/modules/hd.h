#ifndef __MODULES_INC_HD__
#define __MODULES_INC_HD__

#include "kernel/type.h"
#include "syscall/syscall.h"

#define DEV_OPEN 1
#define DEV_CLOSE 2
#define DEV_WRITE 3
#define DEV_READ 4
#define DEV_INFO 5

#define MAKE_DRV(dev, part) (ushort)(((dev) << 8) | ((part)&0xFF))
#define SPLIT_DRV_DEV(drv) (ubyte)(((drv) >> 8) & 0xFF)
#define SPLIT_DEV_PART(drv) (ubyte)((drv)&0xFF)

struct HD_PartInfo {
  uint base;
  uint size;
};

static inline uint HD_dev_open(uint dev) {
  uint task_hd = query_proc("TaskHD");
  message msg;
  msg.type = DEV_OPEN;
  msg.major_data = dev;
  msg.receiver = task_hd;
  send_msg(&msg);
  recv_msg(&msg, task_hd);
  return msg.major_data;
}

static inline uint HD_dev_close(uint dev) {
  uint task_hd = query_proc("TaskHD");
  message msg;
  msg.type = DEV_CLOSE;
  msg.major_data = dev;
  msg.receiver = task_hd;
  send_msg(&msg);
  recv_msg(&msg, task_hd);
  return msg.major_data;
}

// lba in sector, count in bytes
static inline size_t HD_drv_read(ushort drv, uint lba, ubyte *buf,
                                 size_t count) {
  uint task_hd = query_proc("TaskHD");
  message msg;
  msg.type = DEV_READ;
  msg.major_data = drv;
  msg.data.m1.d1 = (uint)buf;
  msg.data.m1.d2 = lba;
  msg.data.m1.d3 = count;

  msg.receiver = task_hd;
  send_msg(&msg);
  recv_msg(&msg, task_hd);
  return msg.major_data;
}

static inline size_t HD_drv_write(ushort drv, uint lba, ubyte *buf,
                                  size_t count) {
  uint task_hd = query_proc("TaskHD");
  message msg;
  msg.type = DEV_WRITE;
  msg.major_data = drv;
  msg.data.m1.d1 = (uint)buf;
  msg.data.m1.d2 = lba;
  msg.data.m1.d3 = count;
  msg.receiver = task_hd;
  send_msg(&msg);
  recv_msg(&msg, task_hd);
  return msg.major_data;
}

static inline int HD_info(ushort drv, struct HD_PartInfo *part_info) {
  uint task_hd = query_proc("TaskHD");
  message msg;
  msg.type = DEV_INFO;
  msg.major_data = drv;
  msg.data.m1.d1 = (uint)part_info;
  msg.receiver = task_hd;
  send_msg(&msg);
  recv_msg(&msg, task_hd);
  return msg.major_data;
}

#endif // __MODULES_INC_HD__