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

static inline uint get_key_from(uint proc) {
  message msg;
  msg.type = MSG_KBD_READ;
  msg.receiver = proc;
  send_msg(&msg);
  recv_msg(&msg, proc);
  return msg.major_data;
}

static inline void putc_to(uint proc, char c) {
  message msg;
  msg.type = MSG_MONITOR_PUTC;
  msg.major_data = cur_con;
  msg.receiver = proc;
  msg.data.m3[0] = c;
  send_msg(&msg);
}

static inline void puts_to(uint proc, char *s) {
  message msg;
  msg.type = MSG_MONITOR_PUTS;
  msg.major_data = cur_con;
  msg.receiver = proc;
  msg.data.m1.d1 = (uint)s;
  send_msg(&msg);
}

static inline void flush_monitor(uint proc) {
  message msg;
  msg.type = MSG_MONITOR_FLUSH;
  msg.major_data = cur_con;
  msg.receiver = proc;
  send_msg(&msg);
}

static inline int tty_printf(uint proc, uint con, char *fmt, ...) {
  int i;
  char buf[256];
  va_list arg = (va_list)((char *)(&fmt) + 4);
  i = vsprintf(buf, fmt, arg);
  message msg;
  msg.type = MSG_MONITOR_PUTS;
  msg.major_data = con;
  msg.receiver = proc;
  msg.data.m1.d1 = (uint)buf;
  send_msg(&msg);
  msg.type = MSG_MONITOR_FLUSH;
  msg.major_data = con;
  msg.receiver = proc;
  send_msg(&msg);
  return i;
}

void Task_TTY() {
  message msg;
  if (register_proc("TaskTTY") != 0)
    kprintf("Cannot register as TaskTTY!\n");
  uint proc_monitor = find_proc("TaskMonitor");
  uint proc_kbd = find_proc("TaskKBD");
  while ((proc_monitor == 0) || (proc_kbd == 0)) {
    if (!proc_monitor)
      proc_monitor = find_proc("TaskMonitor");
    if (!proc_kbd)
      proc_kbd = find_proc("TaskKBD");
    delay_ms(100);
  }
  uint key = 0;
  msg.receiver = proc_monitor;
  msg.type = MSG_MONITOR_INIT;
  send_msg(&msg);
  tty_printf(proc_monitor, 0, "Console Initialized!\nConsole #1\n>");
  tty_printf(proc_monitor, 1, "Console #2\n>");
  tty_printf(proc_monitor, 2, "Console #3\n>");
  tty_printf(proc_monitor, 3, "Console #4\n>");
  while (1) {
    if (!((key = get_key_from(proc_kbd)) & 0x0100)) {
      putc_to(proc_monitor, key);
      flush_monitor(proc_monitor);
    } else {
      int raw = key & MASK_RAW;
      switch (raw) {
      case ENTER:
        putc_to(proc_monitor, '\n');
        break;
      case BACKSPACE:
        putc_to(proc_monitor, '\b');
        break;
      case F1:
        cur_con = 0;
        flush_monitor(proc_monitor);
        break;
      case F2:
        cur_con = 1;
        flush_monitor(proc_monitor);
        break;
      case F3:
        cur_con = 2;
        flush_monitor(proc_monitor);
        break;
      case F4:
        cur_con = 3;
        flush_monitor(proc_monitor);
        break;
      case F12:
        msg.type = MSG_MONITOR_SWITCH;
        msg.major_data = cur_con;
        msg.receiver = proc_monitor;
        send_msg(&msg);
      }
    }
  }
}
