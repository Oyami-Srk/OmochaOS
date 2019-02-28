#include "syscall.h"
#include "klib.h"
#include "process.h"

extern cpu_env cpu;

int __get_ticks(void){
  return cpu.beats;
}

int __test_parm(void *v1, int v2, int v3, int v4){
  return v2+v3+v4;
}

int __send_msg(process *sender, message *msg){
  process* receiver = &cpu.processes[msg->receiver];
  assert(!(sender->status & (PROC_STATUS_RECEVING | PROC_STATUS_SENDING))); // process cannot send or receive msg if it's already in sending or receiving status
  if((receiver->status & PROC_STATUS_RECEVING) &&
     (GET_PROC_STATUS_PID(receiver) == ANY) ||
     (GET_PROC_STATUS_PID(receiver) == sender->pid)){
    // receiver processes is ready to receive a msg from sender
    memcpy((void*)&receiver->p_msg, (void*)msg, sizeof(message));
    receiver->status &= ~PROC_STATUS_RECEVING;
    receiver->status |= PROC_STATUS_NORMAL; // receiver back to normal
    SET_PROC_STATUS_PID(receiver, REFUSE); // refuse any, placeholder
  }else{
    // receiver process is not ready to receive a msg from sender
    // so we just copy msg to send and halt the sender
    sender->status &= ~PROC_STATUS_NORMAL;
    sender->status |= PROC_STATUS_SENDING;
    SET_PROC_STATUS_PID(sender, msg->receiver);
    process *p = receiver->quene_sending_to_this_process;
    if(p){
      while(p->quene_sending_to_this_process)
        p = p->quene_sending_to_this_process;
      p->quene_sending_to_this_process = sender;
    }
    else
      receiver->quene_sending_to_this_process = sender;

    //reschedule
  }
  return 0;
}

void* syscall_table[] = {
                         __get_ticks,
                         __test_parm,
                         __send_msg
};
