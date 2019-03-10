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
      if (interrupt_map[msg.major_data] != 0)
        panic("Someone has already been assigned to this interrupt!");
      for (irq_num = 0; irq_num < HW_IRQ_COUNT; irq_num++)
        if (interrupt_map[irq_num] == msg.sender)
          panic("This process has already been assigned to an interrupt!");
      /* kprintf("Assigned %d int to %d proc", msg.major_data, msg.sender); */
      interrupt_map[msg.major_data] = msg.sender;
      msg.receiver = msg.sender;
      msg.type = SC_DONE;
      msg.major_data = 0;
      send_msg(&msg);
      break;
    case SC_RELEASE_IRQ_FUNC:
      if (msg.major_data >= HW_IRQ_COUNT)
        panic("No more interrupt!");
      for (irq_num = 0; irq_num < HW_IRQ_COUNT; irq_num++)
        if (interrupt_map[irq_num] == msg.sender)
          break;
      if (interrupt_map[irq_num] != msg.sender)
        panic("No one has been assigned to this interrupt yet!");
      interrupt_map[irq_num] = 0;
      break;

    default:
      panic("Unknown Msg! baka! % >_< %");
      kprintf("!");
    }
  }
}

extern void delay_ms(uint);

void Task_KBD(){
  int i = 0;
  message msg;
  msg.receiver = 1;
  msg.major_data = IRQ_KBD - IRQ0;
  msg.type = SC_SET_IRQ_FUNC;
  send_msg(&msg);
  recv_msg(&msg, 1);

  uint ticks = 0;
  uint old_ticks = 0;

  if(msg.type == SC_DONE)
    kprintf("Got assigned for KBD interrupt!\n");
  while(1){
    recv_msg(&msg, ANY);
    switch(msg.type){
    case INTERRUPT:
      kprintf("0x%02x ", inb(0x60));
    }

    ticks = get_ticks_msg();
    if(old_ticks + 50 < ticks){
      old_ticks = ticks;
      kprintf("<Ticks:%d>", ticks);
    }

  }
}
