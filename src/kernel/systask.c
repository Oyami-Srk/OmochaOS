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
extern uint proc_count;
extern void puts_monitor(void*,char*);
extern void* get_con(uint nr_con);
extern void flush_scr(void*);

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
      msg.receiver = msg.sender;
      msg.type = SC_DONE;
      msg.major_data = 0;
      send_msg(&msg);
      break;
    case SC_REGISTER_PROC:{
      char *name = msg.data.m3;
      for(uint i = 0; i < proc_count; i++)
        if(strcmp(name, cpu.processes[i].name) == 0 &&
           i != msg.sender){
          msg.receiver = msg.sender;
          msg.major_data = -1;
          msg.type = SC_FAILED;
          send_msg(&msg);

          goto final;
        }
      uint len = strlen(name);
      strcpy(cpu.processes[msg.sender].name, name);
      msg.receiver = msg.sender;
      msg.major_data = 0;
      msg.type = SC_DONE;
      send_msg(&msg);
      final:
      break;
    }
    case SC_UNREGISTER_PROC:
      memset(cpu.processes[msg.sender].name, 0, PROCESS_NAME_SIZE);
      msg.receiver = msg.sender;
      msg.major_data = 0;
      msg.type = SC_DONE;
      send_msg(&msg);
      break;
    case SC_QUERY_PROC:{
      uint pid = 0;
      char *name = msg.data.m3;
      for(uint i = 0; i < proc_count; i++)
        if(strcmp(name, cpu.processes[i].name) == 0){
          pid = i;
          break;
        }
      if(pid != 0){
        msg.receiver = msg.sender;
        msg.major_data = pid;
        msg.type = SC_DONE;
      }else{
        msg.receiver = msg.sender;
        msg.major_data = 0;
        msg.type = SC_FAILED;
      }
      send_msg(&msg);
      break;
    }
    case SC_WRITE:
      switch(msg.major_data){
      case 0:{ // tty
        extern uint cur_con;
        puts_monitor(get_con(cpu.processes[msg.sender].nr_tty), (char*)msg.data.m1.d1);
        if(cpu.processes[msg.sender].nr_tty == cur_con)
          flush_scr(get_con(cpu.processes[msg.sender].nr_tty));
        break;
      }
      }
      break;
    default:
      panic("Unknown Msg! baka! % >_< %");
      kprintf("!");
    }
  }
}

