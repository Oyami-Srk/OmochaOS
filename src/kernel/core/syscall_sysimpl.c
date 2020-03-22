#include "core/process.h"
#include "generic/asm.h"
#include "generic/typedefs.h"
#include "lib/stdlib.h"
#include "lib/string.h"
#include "lib/syscall.h"

extern uint     beats;
extern process *proc_table; // TODO: not hardcode

uint __get_beats(void) { return beats; }

uint __add(void *caller, uint a, uint b, uint c) { return a + b + c; }

uint __send_msg(process *sender, message *msg) {
    msg->sender       = sender->pid;
    process *receiver = &proc_table[msg->receiver];
    // TODO: check deadlock

    assert(msg->receiver != msg->sender);
    if ((receiver->status & PROC_STATUS_RECEVING) &&
        (receiver->p_msg->ready == 1) &&
        ((receiver->p_msg->sender == PROC_ANY) ||
         (receiver->p_msg->sender == sender->pid))) {
        // receiver is ready to receiver a msg from sender
        memcpy(receiver->p_msg, msg, sizeof(message));
        receiver->status &= ~PROC_STATUS_RECEVING;
        receiver->status |= PROC_STATUS_NORMAL;
        receiver->status |= PROC_STATUS_RUNNING; // TODO: seperate RUNNING and
                                                 // READY in scheduler
        receiver->p_msg->ready =
            0; // process itself donot need check this value as if the proc is
               // got its message and running
    } else {
        // receiver is not ready to receive so copy msg to sender and halt the
        // sender, then append to receiver's quene
        sender->p_msg = msg;
        sender->status &= ~PROC_STATUS_NORMAL;
        sender->status |= PROC_STATUS_SENDING;
        process *p = receiver->quene_head_sending_to_this_process;
        if (p) { // head is already set
            while (p->quene_body)
                p = p->quene_body;
            p->quene_body = sender;
        } else
            receiver->quene_head_sending_to_this_process = sender;
        sender->quene_body = NULL; // a proc cannot send msg simultaneously, so
                                   // it's safe to clear the body
        extern void scheduler(void);
        scheduler();
    }
    return 0;
}

uint __recv_msg(process *receiver, message *msg, uint recv_from) {
    msg->receiver = receiver->pid;
    assert(msg->receiver != recv_from);
    uint     has_received = FALSE;
    process *sender       = NULL;

    if (receiver->status & PROC_STATUS_GOTINT &&
        (recv_from == PROC_INTERRUPT || recv_from == PROC_ANY)) {
        msg->type   = MSG_INTERRUPT;
        msg->major  = MSG_INTERRUPT;
        msg->sender = PROC_INTERRUPT;
        receiver->status &= ~PROC_STATUS_GOTINT;
        return 0;
    }

    if (recv_from == PROC_INTERRUPT) {
        msg->ready      = 1;
        msg->sender     = recv_from;
        receiver->p_msg = msg;
        receiver->status &= ~PROC_STATUS_RUNNING;
        receiver->status |= PROC_STATUS_RECEVING;
        extern void scheduler(void);
        scheduler();
        return 0;
    }

    if (recv_from == PROC_ANY) {
        if (receiver->quene_head_sending_to_this_process) {
            sender = receiver->quene_head_sending_to_this_process;
            receiver->quene_head_sending_to_this_process = sender->quene_body;
        }
    } else {
        sender = &proc_table[recv_from];
        if ((sender->status & PROC_STATUS_SENDING) &&
            (sender->p_msg->receiver == receiver->pid)) {
            // remove sender from list
            process *p = receiver->quene_head_sending_to_this_process;
            if (p == sender) {
                receiver->quene_head_sending_to_this_process =
                    sender->quene_body;
                sender->quene_body = 0;
            } else {
                process *prev_p = p;
                p               = p->quene_body;
                assert(p); // p must not be null if we haven't find sender yet
                           // (cause sender is sending so it must in the quene)
                while (p) {
                    if (p == sender) {
                        prev_p->quene_body = p->quene_body;
                        break;
                    }
                    p      = p->quene_body;
                    prev_p = prev_p->quene_body;
                }

                if (p != sender)
                    panic("Sender not in quene, wrong quene.");
                /* prev_p->quene_body = sender->quene_body; */
                sender->quene_body = 0;
            }
        } else {
            sender = NULL;
        }
    }

    if (sender) {
        assert(msg);
        assert(sender->status & PROC_STATUS_SENDING);
        memcpy(msg, sender->p_msg, sizeof(message));
        sender->p_msg = NULL;
        sender->status &= ~PROC_STATUS_SENDING;
        sender->status |= PROC_STATUS_NORMAL;
        sender->status |= PROC_STATUS_RUNNING;
    } else {
        receiver->p_msg         = msg;
        receiver->p_msg->ready  = 1;
        receiver->p_msg->sender = recv_from;
        receiver->status &= ~PROC_STATUS_RUNNING;
        receiver->status |= PROC_STATUS_RECEVING;
        extern void scheduler(void);
        scheduler();
    }

    return 0;
}

void *syscall_table[] = {__get_beats, __send_msg, __recv_msg};
