#include "kernel/interrupt.h"
#include "kernel/klib.h"
#include "kernel/process.h"
#include "kernel/asm.h"
#include "string.h"
#include "syscall.h"

// systask is the only one (maybe not current yet) module built within kernel
// binary and use share resource with kernel but running on ring-1 tasks running
// on ring-1 user-tasks running on ring-3

extern cpu_env cpu;

void SysIdle() {
  while (1)
    ;
}

void SysTask() {
  message msg;
  msg.receiver = 1;
  uint irq_num = 0;
  while (1) {
    recv_msg(&msg, ANY);

    switch (msg.type) {
    case SC_GET_TICKS:
      msg.major_data = get_ticks();
      msg.type = SC_DONE;
      msg.receiver = msg.sender;
      send_msg(&msg);
      break;
    case SC_SET_IRQ_FUNC:
      if (msg.major_data == 0)
        panic("Cannot assign clock interrupt to tasks");
      if (msg.major_data >= HW_IRQ_COUNT)
        panic("No more interrupt!");
      if (interrupt_methods[msg.major_data].avail != 0)
        panic("Someone has already been assigned to this interrupt!");
      for (irq_num = 0; irq_num < HW_IRQ_COUNT; irq_num++)
        if (interrupt_methods[irq_num].pid == msg.sender)
          panic("This process has already been assigned to an interrupt!");
      /* kprintf("Assigned %d int to %d proc", msg.major_data, msg.sender); */

      // assign
      interrupt_methods[msg.major_data].avail = 1;
      interrupt_methods[msg.major_data].pid = msg.sender;
      interrupt_methods[msg.major_data].data = msg.data.m1.d1;
      interrupt_methods[msg.major_data].method = msg.data.m1.d2;

      msg.receiver = msg.sender;
      msg.type = SC_DONE;
      msg.major_data = 0;

      send_msg(&msg);
      break;
    case SC_RELEASE_IRQ_FUNC:
      if (msg.major_data >= HW_IRQ_COUNT)
        panic("No more interrupt!");
      for (irq_num = 0; irq_num < HW_IRQ_COUNT; irq_num++)
        if (interrupt_methods[irq_num].pid == msg.sender)
          break;
      if (interrupt_methods[irq_num].pid != msg.sender)
        panic("No one has been assigned to this interrupt yet!");
      interrupt_methods[irq_num].avail = 0;
      interrupt_methods[irq_num].pid = 0;
      interrupt_methods[irq_num].data = 0;
      interrupt_methods[irq_num].method = 0;

      break;

    default:
      panic("Unknown Msg! baka! % >_< %");
      kprintf("!");
    }
  }
}

