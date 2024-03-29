#include <core/interrupt.h>
#include <core/paging.h>
#include <core/process.h>
#include <driver/graphic.h>
#include <generic/asm.h>
#include <generic/typedefs.h>
#include <lib/stdlib.h>
#include <lib/string.h>
#include <lib/syscall.h>

extern uint *   beats;
extern process *proc_table; // TODO: not hardcode

extern uint *                 exception_suscribed;
extern struct interrupt_data *exception_suscribed_data;
extern uint *                 interrupt_suscribed;
extern struct interrupt_data *interrupt_suscribed_data;

uint __get_beats(void) { return *beats; }

uint __add(void *caller, uint a, uint b, uint c) { return a + b + c; }

/*
 *  Notice Process passing to syscall's msg is vaddr
 *  and proc's p_msg is also to be vaddr
 *  Need do vir2phy twice
 */

static BOOL __is_dead_lock(pid_t src, pid_t dest) {
    process *p = &proc_table[dest];
    while (TRUE) {
        if (p->status & PROC_STATUS_SENDING) {
            pid_t to =
                ((message *)(vir2phy(p->page_dir, (void *)p->p_msg)))->receiver;
            if (to == src) {
                // print send chain
                p = &proc_table[dest];
                kprintfc(RED, BLUE, "===DEADLOCK=== %s", p->name);
                do {
                    pid_t to1 =
                        ((message *)(vir2phy(p->page_dir, (void *)p->p_msg)))
                            ->receiver;
                    p = &proc_table[to1];
                    kprintfc(RED, BLUE, "->%s", p->name);
                } while (p->pid != src);
                kprintfc(RED, BLUE, "===DEADLOCK===\n");
                return TRUE;
            }
            p = &proc_table[to];
        } else {
            break;
        }
    }
    return FALSE;
}

uint __send_msg(process *sender, message *msg) {
    message *msg_va = msg;
    msg             = (message *)vir2phy(sender->page_dir, (char *)msg);
    assert(msg != NULL);
    msg->sender       = sender->pid;
    process *receiver = &proc_table[msg->receiver];
    message *recv_msg_pa =
        (message *)vir2phy(receiver->page_dir, (char *)receiver->p_msg);
    // TODO: check deadlock
    if (__is_dead_lock(sender->pid, msg->receiver) == TRUE) {
        panic(DEADLOCK);
    }

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

    if (recv_from == 0) { // 0 is reflect msg
        return 0;
    }

    if (receiver->status & PROC_STATUS_GOTINT &&
        (recv_from == PROC_INTERRUPT || recv_from == PROC_ANY)) {
        pid_t pid        = receiver->pid;
        uint  irq        = 0;
        BOOL  has_second = FALSE;
        for (uint i = 0; i < HW_IRQ_COUNT; i++) {
            if (interrupt_suscribed_data[i].avail == TRUE) {
                if (irq) {
                    has_second = TRUE;
                    break;
                }
                irq = i;
            }
        }
        if (!irq)
            panic("No interrupt with a interrupt status.");
        msg->type   = MSG_INTERRUPT;
        msg->major  = irq;
        msg->sender = PROC_INTERRUPT;
        if (!has_second)
            receiver->status &= ~PROC_STATUS_GOTINT;
        interrupt_suscribed_data[irq].avail = FALSE;
        return 0;
    }

    if (receiver->status & PROC_STATUS_GOTEXC &&
        (recv_from == PROC_INTERRUPT || recv_from == PROC_ANY)) {
        pid_t pid        = receiver->pid;
        uint  exc        = 0;
        uint  data       = 0;
        BOOL  has_second = FALSE;
        for (uint i = 0; i < EXCEPTION_COUNT; i++) {
            if (exception_suscribed_data[i].avail == TRUE) {
                if (exc) {
                    has_second = TRUE;
                    break;
                }
                exc  = i;
                data = exception_suscribed_data[i].data;
            }
        }
        if (!exc)
            panic("No interrupt with a interrupt status.");
        msg->type             = MSG_EXCEPTION;
        msg->major            = exc;
        msg->data.uint_arr.d1 = data;
        msg->sender           = PROC_INTERRUPT;
        if (!has_second)
            receiver->status &= ~PROC_STATUS_GOTEXC;
        exception_suscribed_data[exc].avail = FALSE;
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
