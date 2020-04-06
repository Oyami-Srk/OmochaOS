#include "core/paging.h"
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

/*
 *  Notice Process passing to syscall's msg is vaddr
 *  and proc's p_msg is also to be vaddr
 *  Need do vir2phy twice
 */

uint __send_msg(process *sender, message *msg) {
    message *msg_va = msg;
    msg             = (message *)vir2phy(sender->page_dir, (char *)msg);
    assert(msg != NULL);
    msg->sender       = sender->pid;
    process *receiver = &proc_table[msg->receiver];
    message *recv_msg_pa =
        (message *)vir2phy(receiver->page_dir, (char *)receiver->p_msg);
    // TODO: check deadlock

    assert(msg->receiver != msg->sender);
    if ((receiver->status & PROC_STATUS_RECEVING) &&
        (recv_msg_pa->ready == 1) &&
        ((recv_msg_pa->sender == PROC_ANY) ||
         (recv_msg_pa->sender == sender->pid))) {
        // receiver is ready to receiver a msg from sender
        assert(recv_msg_pa != NULL);
        memcpy(recv_msg_pa, msg, sizeof(message));
        receiver->status &= ~PROC_STATUS_RECEVING;
        receiver->status |= PROC_STATUS_NORMAL;
        receiver->status |= PROC_STATUS_RUNNING; // TODO: seperate RUNNING and
                                                 // READY in scheduler
        recv_msg_pa->ready = 0; // process itself donot need check this value as
                                // if the proc is got its message and running
    } else {
        // receiver is not ready to receive so copy msg to sender and halt the
        // sender, then append to receiver's quene
        sender->p_msg = msg_va;
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
    message *msg_va = msg;
    msg             = (message *)vir2phy(receiver->page_dir, (char *)msg);
    assert(msg != NULL);
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
        receiver->p_msg = msg_va;
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
        message *send_msg_pa =
            (message *)vir2phy(sender->page_dir, (char *)sender->p_msg);
        /* message *send_msg_pa = sender->p_msg; */
        if (send_msg_pa && (sender->status & PROC_STATUS_SENDING) &&
            (send_msg_pa->receiver == receiver->pid)) {
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
        message *send_msg_pa =
            (message *)vir2phy(sender->page_dir, (char *)sender->p_msg);
        /* message *send_msg_pa = sender->p_msg; */
        assert(msg);
        assert(sender->status & PROC_STATUS_SENDING);
        memcpy(msg, send_msg_pa, sizeof(message));
        sender->p_msg = NULL;
        sender->status &= ~PROC_STATUS_SENDING;
        sender->status |= PROC_STATUS_NORMAL;
        sender->status |= PROC_STATUS_RUNNING;
    } else {
        msg->ready      = 1;
        msg->sender     = recv_from;
        receiver->p_msg = msg_va;
        receiver->status &= ~PROC_STATUS_RUNNING;
        receiver->status |= PROC_STATUS_RECEVING;
        extern void scheduler(void);
        scheduler();
    }

    return 0;
}

void *syscall_table[] = {__get_beats, __send_msg, __recv_msg};
