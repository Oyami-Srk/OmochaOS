#include "syscall.h"
#include "klib.h"
#include "process.h"
#include "interrupt.h"
#include "type.h"

extern cpu_env cpu;

int __get_ticks(void){
  return cpu.beats;
}

int __test_parm(void *v1, int v2, int v3, int v4){
  return v2+v3+v4;
}

int __send_msg(process *sender, message *msg){
  process* receiver = &cpu.processes[msg->receiver - 1];
  assert(!(  (sender->status & PROC_STATUS_SENDING) || (sender->status & PROC_STATUS_RECEVING)
)); // process cannot send or receive msg if it's already in sending or receiving status
  if((receiver->status & PROC_STATUS_RECEVING) &&
     (GET_PROC_STATUS_PID(receiver) == ANY) ||
     (GET_PROC_STATUS_PID(receiver) == sender->pid)){
    // receiver processes is ready to receive a msg from sender
    memcpy((void*)receiver->p_msg, (void*)msg, sizeof(message));
    receiver->status &= ~PROC_STATUS_RECEVING;
    receiver->status |= PROC_STATUS_NORMAL; // receiver back to normal
    receiver->status |= PROC_STATUS_RUNNING;
    SET_PROC_STATUS_PID(receiver, REFUSE); // refuse any, placeholder
  }else{
    // receiver process is not ready to receive a msg from sender
    // so we just copy msg to send and halt the sender
    sender->p_msg = msg;
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
    kreload_process();
  }
  return 0;
}

int __recv_msg(process* receiver, message* msg, uint recv_from){
  uint has_received = 0;
  process* sender = 0;
  if(recv_from == REFUSE)
    panic("Recv with refuse");
  if(recv_from == ANY){
    if(receiver->quene_sending_to_this_process){
      sender = receiver->quene_sending_to_this_process;
      receiver->quene_sending_to_this_process = sender->quene_sending_to_this_process;
    }
  }else{
    sender = &cpu.processes[recv_from - 1];
    if((sender->status & PROC_STATUS_SENDING) &&
       (GET_PROC_STATUS_PID(sender) == receiver->pid)){
      // remove sender from
      process* p = receiver->quene_sending_to_this_process;
      process* prev_p = receiver;
      assert(p);
      while(p){
        if(p == sender){
          prev_p->quene_sending_to_this_process = p->quene_sending_to_this_process;
          break;
        }
        p = p->quene_sending_to_this_process;
        prev_p = prev_p->quene_sending_to_this_process;
      }
    }else{
      sender = 0;
    }
  }

  if(sender){
    assert(msg);
    assert(sender->status & PROC_STATUS_SENDING);
    memcpy(msg, sender->p_msg, sizeof(message));
    sender->p_msg = 0;
    SET_PROC_STATUS_PID(sender, REFUSE);
    sender->status &= ~PROC_STATUS_SENDING;
    sender->status |= PROC_STATUS_NORMAL;
    sender->status |= PROC_STATUS_RUNNING;
    kprintf(" s:0x%04x ", sender->status);
    kreload_process();
  }else{
    SET_PROC_STATUS_PID(receiver, recv_from);
    receiver->p_msg = msg;
    receiver->status &= PROC_STATUS_RECEVING;
    kreload_process();
  }
  return 0;
}

void* syscall_table[] = {
                         __get_ticks,
                         __test_parm,
                         __send_msg,
                         __recv_msg
};
