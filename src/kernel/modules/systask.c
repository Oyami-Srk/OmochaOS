/*
module:
  name: SysTask
  author: Shiroko
  summary: System Task's process
  entry: SysTask
  preferred_pid: 1
*/

#include "generic/systask.h"
#include "core/interrupt.h"
#include "core/process.h"
#include "driver/graphic.h"
#include "generic/syscall.h"
#include "lib/stdlib.h"
#include "lib/string.h"

// this task will be at the same addr sapce as the kernel does
// like a macrokernel, just because I am a good vegetable

extern size_t                  proc_count;
extern process *               proc_table;
extern uint                    beats;
extern struct interrupt_method interrupt_methods[HW_IRQ_COUNT];
extern uint                    interrupt_suscribed[HW_IRQ_COUNT];

void SysTask() {
    message msg;
    msg.receiver = 0;
    // send msg to proc 0 (System reflect idle)
    // to check our pid is 1
    send_msg(&msg);
    recv_msg(&msg, 0);
    if (msg.receiver != SYSTASK_PID)
        panic("Systask pid is not 1");
    while (1) {
        recv_msg(&msg, PROC_ANY);
        switch (msg.type) {
        case GET_TICKS: {
            msg.major = beats;
            SEND_BACK(msg);
            break;
        }
        case REG_PROC: {
            char *name = msg.data.b16;
            for (uint i = 0; i < proc_count; i++)
                if (strcmp(name, proc_table[i].name) == 0 && i != msg.sender) {
                    msg.major = -1;
                    SEND_BACK(msg);
                }
            size_t len = strlen(name);
            strcpy(proc_table[msg.sender].name, name);
            msg.major = 0;
            SEND_BACK(msg);
            break;
        }
        case UNREG_PROC:
            memset(proc_table[msg.sender].name, 0,
                   sizeof(proc_table[msg.sender].name));
            msg.major = 0;
            SEND_BACK(msg);
        case QUERY_PROC: {
            uint  pid  = 0;
            char *name = msg.data.b16;
            for (uint i = 0; i < proc_count; i++)
                if (strcmp(name, proc_table[i].name) == 0) {
                    pid = i;
                    break;
                }
            msg.major = pid;
            SEND_BACK(msg);
            break;
        }
        case REG_INT_FUNC:
            if (msg.major == 0)
                panic("Cannot assign to clock interrupt!");
            if (msg.major >= HW_IRQ_COUNT)
                panic("Cannot assign to interrupt bigger than 16!");
            if (interrupt_methods[msg.major].avail == TRUE)
                panic("Someone has been assigned to this interrupt!");
            interrupt_methods[msg.major].avail = TRUE;
            interrupt_methods[msg.major].pid   = msg.sender;
            interrupt_methods[msg.major].func  = (void *)msg.data.uint_arr.d1;
            msg.major                          = 0;
            SEND_BACK(msg);
            break;
        case UNREG_INT_FUNC:
            if (msg.major == 0)
                panic("Cannot unassign clock interrupt!");
            if (msg.major >= HW_IRQ_COUNT)
                panic("Cannot unassign an interrupt bigger than 16!");
            if (interrupt_methods[msg.major].avail == FALSE)
                panic("No one has been assigned to this interrupt!");
            if (interrupt_methods[msg.major].pid != msg.sender)
                panic("Cannot unassign an interrupt not assigned to you!");
            interrupt_methods[msg.major].avail = FALSE;
            interrupt_methods[msg.major].pid   = 0;
            interrupt_methods[msg.major].func  = NULL;
            msg.major                          = 0;
            SEND_BACK(msg);
            break;
        case REG_INT_MSG:
            if (msg.major == 0)
                panic("Cannot suscribe clock interrupt!");
            if (msg.major >= HW_IRQ_COUNT)
                panic("Cannot suscribe an interrupt bigger than 16");
            if (interrupt_suscribed[msg.major] != 0)
                panic("Someone has suscribed this irq");
            interrupt_suscribed[msg.major] = msg.sender;
            msg.major                      = 0;
            SEND_BACK(msg);
            break;
        case UNREG_INT_MSG:
            if (msg.major == 0)
                panic("Cannot unsuscribe clock interrupt!");
            if (msg.major >= HW_IRQ_COUNT)
                panic("Cannot unsuscribe an interrupt bigger than 16");
            if (interrupt_suscribed[msg.major] == 0)
                panic("Cannot unsuscribe an unsuscribed interrupt");
            interrupt_suscribed[msg.major] = 0;
            msg.major                      = 0;
            SEND_BACK(msg);
            break;
        case PEEK_MSG:
            if (proc_table[msg.sender].quene_head_sending_to_this_process !=
                NULL)
                msg.major = TRUE;
            else
                msg.major = FALSE;
            SEND_BACK(msg);
            break;
        default:
            break;
        }
    }
}
