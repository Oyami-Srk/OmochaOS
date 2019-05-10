/*
module:
  name: TTY
  author: Shiroko
  summary: TTY Process
  module_task: Task_TTY
*/
#include "driver/vga.h"
#include "kbd.h"
#include "lib/stdlib.h"
#include "modules/modules.h"
#include "monitor.h"
#include "syscall/syscall.h"

uint cur_con = 0;
const uint max_con = 4;

// 现在tty先放着，之后tty内也是普通的模块，循环接收消息
// 这些交互另用户进程shell进行，tty就是负责输出输入的

struct console con[4];

void Task_TTY() {
  message msg;
  if (reg_proc("TaskTTY") != 0)
    kprintf("Cannot register as TaskTTY!\n");
  uint key = 0;
  initialize_kbd();
  for (uint i = 0; i < max_con; i++)
    init_console(&con[i], i);
  write_console(&con[0], "Monitor Initialized!\nConsole #1> ");
  write_console(&con[1], "Console #2> ");
  write_console(&con[2], "Console #3> ");
  write_console(&con[3], "Console #4> ");

  while (1) {
    if (!((key = kbd_read()) & 0x0100)) {
      char buf[2];
      sprintf(buf, "%c", key);
      write_console(&con[cur_con], buf);
      flush_console(&con[cur_con]);
    } else {
      int raw = key & MASK_RAW;
      switch (raw) {
      case ENTER:
        write_console(&con[cur_con], "\n");
        break;
      case BACKSPACE:
        write_console(&con[cur_con], "\b");
        break;
      case F1:
        cur_con = 0;
        flush_console(&con[0]);
        break;
      case F2:
        cur_con = 1;
        flush_console(&con[1]);
        break;
      case F3:
        cur_con = 2;
        flush_console(&con[2]);
        break;
      case F4:
        cur_con = 3;
        flush_console(&con[3]);
        break;
      case F12:
        if (con[cur_con].disp_start_offset != con[cur_con].origin_offset)
          con[cur_con].disp_start_offset = con[cur_con].origin_offset;
        else
          con[cur_con].disp_start_offset =
              con[cur_con].origin_offset + SCREEN_SIZE * 2;
        flush_console(&con[cur_con]);
        break;
      }
    }
  }
}
