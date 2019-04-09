/*
module:
  name: TTY
  author: Shiroko
  summary: TTY Process
  module_task: Task_TTY
*/
#include "KeyBoard.h"
#include "Monitor.h"
#include "modules/modules.h"

uint cur_con = 0;
uint max_con = 3;

extern int kprintf(const char *fmt, ...);
extern Circular_BufferB kbd_buffer;
extern void delay_ms(uint);


// 现在tty先放着，之后tty内也是普通的模块，循环接收消息
// 这些交互另用户进程shell进行，tty就是负责输出输入的

void Task_TTY() {
  message msg;
  if (register_proc("TaskTTY") != 0)
    kprintf("Cannot register as TaskTTY!\n");
  uint key = 0;
  initialize_kbd();
  initialize_monitor();
  monitor_printf(cur_con, "Monitor Initialized!\nConsole #1> ");
  monitor_printf(1, "Console #2> ");
  monitor_printf(2, "Console #3> ");
  monitor_printf(3, "Console #4> ");

  while (1) {
    if (!((key = kbd_read()) & 0x0100)) {
      putc_monitor(get_con(cur_con), key);
      flush_scr(get_con(cur_con));
    } else {
      int raw = key & MASK_RAW;
      switch (raw) {
      case ENTER:
        putc_monitor(get_con(cur_con), '\n');
        break;
      case BACKSPACE:
        putc_monitor(get_con(cur_con), '\b');
        break;
      case F1:
        cur_con = 0;
        flush_scr(get_con(cur_con));
        break;
      case F2:
        cur_con = 1;
        flush_scr(get_con(cur_con));
        break;
      case F3:
        cur_con = 2;
        flush_scr(get_con(cur_con));
        break;
      case F4:
        cur_con = 3;
        flush_scr(get_con(cur_con));
        break;
      case F12:
        switch_monitor(get_con(cur_con));
        flush_scr(get_con(cur_con));
        break;
      }
    }
  }
}
