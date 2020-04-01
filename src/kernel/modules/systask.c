/*
module:
  name: SysTask
  author: Shiroko
  summary: System Task's process
  entry: SysTask
  preferred_pid: 1
*/

#include "modules/systask.h"
#include "core/environment.h"
#include "core/interrupt.h"
#include "core/process.h"
#include "driver/graphic.h"
#include "driver/misc.h"
#include "lib/stdlib.h"
#include "lib/string.h"
#include "lib/syscall.h"

// this task will be at the same addr sapce as the kernel does
// like a macrokernel, just because I am a good vegetable

extern struct core_env core_env;

static int __reg_proc(uint pid, char *name) {
    for (uint i = 0; i < core_env.proc_count; i++)
        if (strcmp(name, core_env.proc_table[i].name) == 0 && i != pid)
            return -1;
    size_t len = strlen(name);
    strcpy(core_env.proc_table[pid].name, name);
    return 0;
}

static void __unreg_proc(uint pid) {
    memset(core_env.proc_table[pid].name, 0,
           sizeof(core_env.proc_table[pid].name));
}

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
            msg.major = core_env.beats;
            SEND_BACK(msg);
            break;
        }
        case REG_PROC: {
            msg.major = __reg_proc(msg.sender, (char *)msg.data.b16);
            SEND_BACK(msg);
            break;
        }
        case UNREG_PROC:
            __unreg_proc(msg.sender);
            msg.major = 0;
            SEND_BACK(msg);
        case QUERY_PROC: {
            uint  pid  = 0;
            char *name = msg.data.b16;
            for (process *p = core_env.proc_list; p != NULL; p = p->next) {
                if (strcmp(name, p->name) == 0) {
                    pid = p->pid;
                    break;
                }
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
            if (core_env.interrupt_methods[msg.major].avail == TRUE)
                panic("Someone has been assigned to this interrupt!");
            core_env.interrupt_methods[msg.major].avail = TRUE;
            core_env.interrupt_methods[msg.major].pid   = msg.sender;
            core_env.interrupt_methods[msg.major].func =
                (void *)msg.data.uint_arr.d1;
            msg.major = 0;
            SEND_BACK(msg);
            break;
        case UNREG_INT_FUNC:
            if (msg.major == 0)
                panic("Cannot unassign clock interrupt!");
            if (msg.major >= HW_IRQ_COUNT)
                panic("Cannot unassign an interrupt bigger than 16!");
            if (core_env.interrupt_methods[msg.major].avail == FALSE)
                panic("No one has been assigned to this interrupt!");
            if (core_env.interrupt_methods[msg.major].pid != msg.sender)
                panic("Cannot unassign an interrupt not assigned to you!");
            core_env.interrupt_methods[msg.major].avail = FALSE;
            core_env.interrupt_methods[msg.major].pid   = 0;
            core_env.interrupt_methods[msg.major].func  = NULL;
            msg.major                                   = 0;
            SEND_BACK(msg);
            break;
        case REG_INT_MSG:
            if (msg.major == 0)
                panic("Cannot suscribe clock interrupt!");
            if (msg.major >= HW_IRQ_COUNT)
                panic("Cannot suscribe an interrupt bigger than 16");
            if (core_env.interrupt_suscribed[msg.major] != 0)
                panic("Someone has suscribed this irq");
            core_env.interrupt_suscribed[msg.major] = msg.sender;
            msg.major                               = 0;
            SEND_BACK(msg);
            break;
        case UNREG_INT_MSG:
            if (msg.major == 0)
                panic("Cannot unsuscribe clock interrupt!");
            if (msg.major >= HW_IRQ_COUNT)
                panic("Cannot unsuscribe an interrupt bigger than 16");
            if (core_env.interrupt_suscribed[msg.major] == 0)
                panic("Cannot unsuscribe an unsuscribed interrupt");
            core_env.interrupt_suscribed[msg.major] = 0;
            msg.major                               = 0;
            SEND_BACK(msg);
            break;
        case PEEK_MSG:
            if (core_env.proc_table[msg.sender]
                    .quene_head_sending_to_this_process != NULL)
                msg.major = TRUE;
            else
                msg.major = FALSE;
            SEND_BACK(msg);
            break;
        case QUERY_ENV:
            switch (msg.major) {
            case ENV_KEY_MEMORY_LOWER:
                msg.major = core_env.boot_info.mem_lower;
                SEND_BACK(msg);
                break;
            case ENV_KEY_MEMORY_UPPER:
                msg.major = core_env.boot_info.mem_upper;
                SEND_BACK(msg);
                break;
            case ENV_KEY_MMAP: {
                void * buf      = (void *)msg.data.uint_arr.d1;
                size_t buf_size = msg.data.uint_arr.d2;
                if (buf_size < sizeof(struct core_env_memory_zone) *
                                   core_env.memory_zone_count)
                    msg.major = 0xFFFFFFFF;
                else {
                    memcpy(buf, core_env.memory_zone,
                           sizeof(struct core_env_memory_zone) *
                               core_env.memory_zone_count);
                    msg.major = core_env.memory_zone_count;
                }
                SEND_BACK(msg);
                break;
            }
            case ENV_KEY_BOOT_INFO: {
                void * buf      = (void *)msg.data.uint_arr.d1;
                size_t buf_size = msg.data.uint_arr.d2;
                if (buf_size < sizeof(core_env.boot_info))
                    msg.major = 0xFFFFFFFF;
                else {
                    memcpy(buf, &core_env.boot_info,
                           sizeof(core_env.boot_info));
                    msg.major = 0;
                }
                SEND_BACK(msg);
                break;
            }
            case ENV_KEY_MEMORY_USAGE: {
                size_t buf_size = msg.data.uint_arr.d2;
                if (buf_size < sizeof(struct core_memory_usage))
                    msg.major = 0xFFFFFFFF;
                else {
                    struct core_memory_usage *buf =
                        (void *)msg.data.uint_arr.d1;
                    buf->core_space_start      = core_env.core_space_start;
                    buf->core_space_end        = core_env.core_space_end;
                    buf->core_space_free_start = core_env.core_space_free_start;
                    buf->core_space_free_end   = core_env.core_space_free_end;
                    buf->memory_end            = core_env.memory_end;
                    msg.major                  = 0;
                }
                SEND_BACK(msg);
                break;
            }
            }
            break;
        case EXIT_PROC: {
            __unreg_proc(msg.sender);
            clear_bit(core_env.proc_bitmap, msg.sender);
            uint proc_stack =
                PGROUNDDOWN(core_env.proc_table[msg.sender].stack.esp);
            if (proc_stack <
                core_env.core_space_free_end) // core free stack is managed by
                                              // memory.c
                kfree((char *)proc_stack);
            else {
                ; // free stack by memory modules
            }
            // free page dir
            uint proc_pd = core_env.proc_table[msg.sender].page_dir;
            // if proc_pd's reference is 1 free it
            // call memory module to free page dir
            core_env.proc_count--;
            process *target = &core_env.proc_table[msg.sender];
            process *p      = core_env.proc_list;

            if (target == p) {
                core_env.proc_list = target;
            } else {
                process *f  = p;
                process *ff = NULL;
                while (p) {
                    f = p;
                    p = p->next;
                    if (p == target) {
                        ff = f;
                        break;
                    }
                }
                ff->next = target->next;
            }

            memset(&core_env.proc_table[msg.sender], 0, sizeof(process));
            break;
        }
        case MODIFY_PROC: {
            uint   KEY      = msg.major;
            uint   major    = msg.data.uint_arr.d1;
            ubyte *buf      = (ubyte *)msg.data.uint_arr.d2;
            size_t buf_size = msg.data.uint_arr.d3;
            uint   pid      = msg.data.uint_arr.d4;
            // check sender's privilige
            if ((core_env.proc_table[msg.sender].stack.cs & 0x3) != 1)
                panic("NO Permission to modify proc.");
            switch (KEY) {
            case MOD_PROC_CR3: {
                core_env.proc_table[pid].page_dir = major;
                msg.major                         = 0;
                break;
            }
            default:
                msg.major = 0xFFFFFFFF;
                break;
            }
            break;
            SEND_BACK(msg);
        }
        default:
            break;
        }
    }
}
