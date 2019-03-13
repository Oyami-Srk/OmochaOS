/*
module:
  name: Monitor
  author: Shiroko
  summary: Module for manipulate monitor to display something
  module_task: Task_Monitor
*/

#include "modules/modules.h"

extern void delay_ms(uint);

void Task_Monitor(){
  while(1){
    kprintf(".");
    delay_ms(500);
  }
}
