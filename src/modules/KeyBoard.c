/*
module:
  name: Keyboard
  author: Shiroko
  summary: Module for handling keyboard interrupt
  module_task: Task_KBD
*/
#include "modules/modules.h"

extern int kprintf(const char *fmt, ...);

void Task_KBD(){
  message msg;
  if(register_to(IRQ_KBD - IRQ0) == 0)
    kprintf("Got assigned for KBD interrupt!\n");
  while(1){
    recv_msg(&msg, ANY);
    switch(msg.type){
    case INTERRUPT:
      kprintf("0x%02x,", inb(0x60));
      enable_irq(IRQ_KBD - IRQ0);
    }
  }
}
