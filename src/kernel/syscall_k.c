#include "kernel/interrupt.h"
#include "kernel/klib.h"
#include "kernel/process.h"
#include "kernel/type.h"
#include "syscall.h"

extern cpu_env cpu;

int __get_ticks(void) { return cpu.beats; }

int __send_msg(process *sender, message *msg) {
  msg->sender = (((uint)sender - (uint)&cpu.processes) / sizeof(process));
  process *receiver = &cpu.processes[msg->receiver];
  assert(
      !((sender->status & PROC_STATUS_SENDING) ||
        (sender->status &
         PROC_STATUS_RECEVING))); // process cannot send or receive msg if it's
                                  // already in sending or receiving status
  assert(msg->receiver != msg->sender);
  if ((receiver->status & PROC_STATUS_RECEVING) &&
          (GET_PROC_STATUS_PID(receiver) == ANY) ||
      (GET_PROC_STATUS_PID(receiver) == sender->pid)) {
    // receiver processes is ready to receive a msg from sender
    memcpy((void *)receiver->p_msg, (void *)msg, sizeof(message));
    receiver->status &= ~PROC_STATUS_RECEVING;
    receiver->status |= PROC_STATUS_NORMAL; // receiver back to normal
    receiver->status |= PROC_STATUS_RUNNING;
    SET_PROC_STATUS_PID(receiver, REFUSE); // refuse any, placeholder
    /* kprintf("[%d sent to %d]", sender->pid, receiver->pid); */
  } else {
    // receiver process is not ready to receive a msg from sender
    // so we just copy msg to send and halt the sender
    sender->p_msg = msg;
    sender->status &= ~PROC_STATUS_NORMAL;
    sender->status |= PROC_STATUS_SENDING;
    SET_PROC_STATUS_PID(sender, msg->receiver);
    process *p = receiver->quene_head_sending_to_this_process;
    if (p) {
      while (p->quene_body)
        p = p->quene_body;
      p->quene_body = sender;
    } else {
      receiver->quene_head_sending_to_this_process = sender;
    }
    sender->quene_body = 0; // remove sending list body

    /* kprintf("[%d sending to %d]", sender->pid, receiver->pid); */

    // reschedule
    kreload_process();
  }
  return 0;
}

int __recv_msg(process *receiver, message *msg, uint recv_from) {
  msg->receiver = (((uint)receiver - (uint)&cpu.processes) / sizeof(process));
  assert(msg->receiver != recv_from);
  uint has_received = 0;
  process *sender = 0;
  if (recv_from == REFUSE)
    panic("Recv with refuse");
  if (receiver->status & PROC_STATUS_GOTINT && (recv_from == INTERRUPT || recv_from == ANY)){
    msg->type = INTERRUPT;
    msg->major_data = INTERRUPT - 1;
    msg->sender = INTERRUPT;
    receiver->status &= ~PROC_STATUS_GOTINT;
    return 0;
  }
  if (recv_from == ANY) { //
    if (receiver->quene_head_sending_to_this_process) {
      sender = receiver->quene_head_sending_to_this_process;
      receiver->quene_head_sending_to_this_process = sender->quene_body;
    }
  } else {
    sender = &cpu.processes[recv_from];
    if ((sender->status & PROC_STATUS_SENDING) &&
        (GET_PROC_STATUS_PID(sender) == receiver->pid)) {
      // remove sender from
      process *p = receiver->quene_head_sending_to_this_process;
      if(p == sender){
        receiver->quene_head_sending_to_this_process = sender->quene_body;
        sender->quene_body = 0;
      }else{
        p = p->quene_body;
        process *prev_p = p;
        assert(p);
        while (p) {
          if (p == sender) {
            prev_p->quene_body = p->quene_body;
            break;
          }
          p = p->quene_body;
          prev_p = prev_p->quene_body;
        }
        if(p != sender)
          panic("Sender not in sending list!");
        prev_p->quene_body = sender->quene_body;
        sender->quene_body = 0;
      }
    } else {
      sender = 0;
    }
  }

  if (sender) {
    assert(msg);
    assert(sender->status & PROC_STATUS_SENDING);
    memcpy(msg, sender->p_msg, sizeof(message));
    sender->p_msg = 0;
    SET_PROC_STATUS_PID(sender, REFUSE);
    sender->status &= ~PROC_STATUS_SENDING;
    sender->status |= PROC_STATUS_NORMAL;
    sender->status |= PROC_STATUS_RUNNING;
    /* kprintf("[%d recved from %d]", receiver->pid, sender->pid); */
    kreload_process();
  } else {
    SET_PROC_STATUS_PID(receiver, recv_from);
    receiver->p_msg = msg;
    receiver->status &= ~PROC_STATUS_RUNNING;
    receiver->status |= PROC_STATUS_RECEVING;
    /* kprintf("[%d recv from %d]", receiver->pid, recv_from); */
    kreload_process();
  }
  return 0;
}

void *syscall_table[] = {__get_ticks, __send_msg, __recv_msg};
