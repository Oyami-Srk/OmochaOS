#include <core/environment.h>
#include <core/interrupt.h>
#include <core/memory.h>
#include <core/paging.h>
#include <core/process.h>
#include <driver/graphic.h>
#include <driver/misc.h>
#include <lib/stdlib.h>
#include <lib/string.h>
#include <lib/syscall.h>
#include <modules/systask.h>

// this task will be at the same addr sapce as the kernel does
// like a macrokernel, just because I am a good vegetable

extern struct core_env core_env;

static int __reg_proc(pid_t pid, char *name) {
    for (uint i = 0; i < core_env.proc_count; i++)
        if (strcmp(name, core_env.proc_table[i].name) == 0 && i != pid)
            return -1;
    size_t len = strlen(name);
    strcpy(core_env.proc_table[pid].name, name);
    return 0;
}

static void __unreg_proc(pid_t pid) {
    memset(core_env.proc_table[pid].name, 0,
           sizeof(core_env.proc_table[pid].name));
}

static uint __query_proc(const char *name) {
    pid_t pid = 0;
    for (process *p = core_env.proc_list; p != NULL; p = p->next) {
        if (strcmp(name, p->name) == 0) {
            pid = p->pid;
            break;
        }
    }
    return pid;
}

void SysTask() {
    message msg;
    msg.receiver = 0;
    // send msg to proc 0 (System reflect idle)
    // to check our pid is 1
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
            msg.major = __reg_proc(
                msg.sender,
                (char *)vir2phy(core_env.proc_table[msg.sender].page_dir,
                                (char *)msg.data.b16));
            SEND_BACK(msg);
            break;
        }
        case UNREG_PROC:
            __unreg_proc(msg.sender);
            msg.major = 0;
            SEND_BACK(msg);
        case QUERY_PROC: {
            msg.major = __query_proc(
                (char *)vir2phy(core_env.proc_table[msg.sender].page_dir,
                                (char *)msg.data.b16));
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
                (void *)vir2phy(core_env.proc_table[msg.sender].page_dir,
                                (char *)msg.data.uint_arr.d1);
            core_env.interrupt_methods[msg.major].data =
                (void *)vir2phy(core_env.proc_table[msg.sender].page_dir,
                                (char *)msg.data.uint_arr.d2);
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
            core_env.interrupt_methods[msg.major].data  = NULL;
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
            if (core_env.interrupt_suscribed[msg.major] != msg.sender)
                panic("Cannot unsuscribe interrupt not suscribe by you");
            core_env.interrupt_suscribed[msg.major] = 0;
            msg.major                               = 0;
            SEND_BACK(msg);
            break;
        case REG_EXC_MSG:
            if (msg.major >= EXCEPTION_COUNT)
                panic("Cannot suscribe an exception bigger than 32");
            if (core_env.exception_suscribed[msg.major] != 0)
                panic("Someone has suscribed this exc");
            core_env.exception_suscribed[msg.major] = msg.sender;
            msg.major                               = 0;
            SEND_BACK(msg);
            break;
        case UNREG_EXC_MSG:
            if (msg.major >= EXCEPTION_COUNT)
                panic("Cannot unsuscribe an exception bigger than 32");
            if (core_env.exception_suscribed[msg.major] == 0)
                panic("Cannot unsuscribe an unsuscribed exception");
            if (core_env.exception_suscribed[msg.major] != msg.sender)
                panic("Cannot unsuscribe exception not suscribe by you");
            core_env.exception_suscribed[msg.major] = 0;
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
                void *buf =
                    (void *)vir2phy(core_env.proc_table[msg.sender].page_dir,
                                    (char *)msg.data.uint_arr.d1);
                if ((uint)buf >= KERN_BASE && buf <= KERN_VEND)
                    panic("Override system memory");
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
                void *buf =
                    (void *)vir2phy(core_env.proc_table[msg.sender].page_dir,
                                    (char *)msg.data.uint_arr.d1);
                if ((uint)buf >= KERN_BASE && buf <= KERN_VEND)
                    panic("Override system memory");
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
                    struct core_memory_usage *buf = (void *)vir2phy(
                        core_env.proc_table[msg.sender].page_dir,
                        (char *)msg.data.uint_arr.d1);
                    if ((uint)buf >= KERN_BASE && (void *)buf <= KERN_VEND)
                        panic("Override system memory");
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
            pid_t pid = msg.major;
            // actually proc should exit after unreg everything itself
            // just a insurance
            __unreg_proc(pid);
            for (uint i = 0; i < HW_IRQ_COUNT; i++) {
                // unreg and unsus int
                if (core_env.interrupt_suscribed[i] == pid)
                    core_env.interrupt_suscribed[i] = 0;
                if (core_env.interrupt_methods[i].pid == pid) {
                    core_env.interrupt_methods[i].pid   = 0;
                    core_env.interrupt_methods[i].avail = FALSE;
                    core_env.interrupt_methods[i].func  = NULL;
                }
            }
            clear_bit(core_env.proc_bitmap, pid);
            uint proc_stack = (uint)core_env.proc_table[pid].pstack;
            if (proc_stack != NULL &&
                proc_stack <
                    core_env.core_space_free_end) // core free stack is managed
                                                  // by memory.c
                kfree_pages((char *)proc_stack);

            core_env.proc_count--;
            process *target = &core_env.proc_table[pid];
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

            memset(&core_env.proc_table[pid], 0, sizeof(process));
            break;
        }
        case ALLOC_PROC: {
            // check sender's privilige
            if ((core_env.proc_table[msg.sender].stack.cs & 0x3) != 1)
                panic("NO Permission to alloc proc.");
            pid_t pid = find_first_unset_bit(core_env.proc_bitmap,
                                             core_env.proc_bitmap_size);
            if (pid == 0xFFFFFFFF) {
                msg.major = NULL;
                SEND_BACK(msg);
                break;
            }
            set_bit(core_env.proc_bitmap, pid);
            process *proc = &core_env.proc_table[pid];
            memset(proc, 0, sizeof(process)); // clean proc
            proc->status = PROC_STATUS_STOP;
            if (core_env.proc_list) {
                process *p = core_env.proc_list;
                process *f = p;
                while (p) {
                    f = p;
                    p = p->next;
                }
                f->next = proc;
            } else
                core_env.proc_list = proc;
            proc->next         = NULL;
            proc->parent_pid   = 1;
            proc->pid          = pid;
            proc->stack.eflags = 0x1202; // IOPL=1
            core_env.proc_count++;
            msg.major = (uint)proc;
            SEND_BACK(msg);
            break;
        }
        case GET_PROC: {
            // check sender's privilige
            if ((core_env.proc_table[msg.sender].stack.cs & 0x3) != 1)
                panic("NO Permission to get proc.");
            msg.major = (uint)&core_env.proc_table[msg.major];
            SEND_BACK(msg);
            break;
        }
        case PROC_VIR2PHY: {
            char *paddr = vir2phy(core_env.proc_table[msg.major].page_dir,
                                  (char *)msg.data.uint_arr.d1);
            msg.major   = (uint)paddr;
            SEND_BACK(msg);
            break;
        }
        case GET_PROC_LIST_HEAD: {
            // check sender's privilige
            if ((core_env.proc_table[msg.sender].stack.cs & 0x3) != 1)
                panic("NO Permission to get proc list head.");
            msg.major = (uint)&core_env.proc_list[0];
            SEND_BACK(msg);
            break;
        }
        case SWITCHPRIVRING3: {
            // check sender's privilige
            if ((core_env.proc_table[msg.sender].stack.cs & 0x3) != 1)
                panic("Not in ring1");
            core_env.proc_table[msg.sender].stack.cs = SEL_CODE_DPL3;
            core_env.proc_table[msg.sender].stack.es = SEL_DATA_DPL3;
            core_env.proc_table[msg.sender].stack.fs = SEL_DATA_DPL3;
            core_env.proc_table[msg.sender].stack.ds = SEL_DATA_DPL3;
            core_env.proc_table[msg.sender].stack.ss = SEL_DATA_DPL3;
            core_env.proc_table[msg.sender].stack.gs = SEL_DATA_DPL3;
            SEND_BACK(msg);
            break;
        }
        case SWITCHPRIVRING1: {
            // check process is in whitelist
            // check privilige
            if ((core_env.proc_table[msg.sender].stack.cs & 0x3) != 3)
                panic("Not in ring3");
            core_env.proc_table[msg.sender].stack.cs = SEL_CODE_DPL1;
            core_env.proc_table[msg.sender].stack.es = SEL_DATA_DPL1;
            core_env.proc_table[msg.sender].stack.fs = SEL_DATA_DPL1;
            core_env.proc_table[msg.sender].stack.ds = SEL_DATA_DPL1;
            core_env.proc_table[msg.sender].stack.ss = SEL_DATA_DPL1;
            core_env.proc_table[msg.sender].stack.gs = SEL_DATA_DPL1;
            SEND_BACK(msg);
            break;
        }
        default:
            break;
        }
    }
}
