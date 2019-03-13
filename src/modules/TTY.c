/*
module:
  name: TTY
  author: Shiroko
  summary: TTY Process
  module_task: Task_TTY
*/
#include "modules/modules.h"

extern int kprintf(const char *fmt, ...);
extern Circular_BufferB kbd_buffer;
extern void delay_ms(uint);

void Task_TTY() {
  message msg;
  while(1){
    /* kprintf("%x,", read_circular_buffer_b(&kbd_buffer)); */
  }
}

