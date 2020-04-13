#include "core/memory.h"
#include "core/paging.h"
#include "core/process.h"
#include "lib/stdlib.h"
#include "memory.h"
#include "modules/systask.h"
#include "paging.h"

process *set_proc_exit(pid_t pid, uint exit_status) {
    process *proc     = get_proc(pid);
    proc->exit_status = exit_status;
    proc->status      = PROC_STATUS_HANGING | PROC_STATUS_STOP;
    return proc;
}

static void send_exit_to_parent(process *proc) {
    message msg;
    msg.type             = 0x12344321;
    msg.major            = proc->exit_status;
    msg.data.uint_arr.d1 = proc->pid;
    msg.receiver         = proc->parent_pid;
    send_msg(&msg);
    exit_proc(proc->pid);
}

void free_proc(struct memory_info *mem, process *proc) {
    char * pstack  = proc->pstack;
    size_t ps_size = proc->pstack_size;
    if ((uint)pstack >= mem->memory_start && (uint)pstack <= mem->usable_end) {
        unmap_pages(mem, proc->page_dir, pstack, ps_size);
        /* page_free(pstack, ps_size / PG_SIZE); */ // page free if unmap
                                                    // reference is 0
        proc->pstack = NULL;
    }
    // unmap others
    for (uint i = 0; i < 1024; i++) {
        if ((proc->page_dir[i] & PG_Present) &&
            !(proc->page_dir[i] & PG_OS_SYS)) {
            pte_t *ptes = (pte_t *)(proc->page_dir[i] & ~0xFFF);
            for (uint j = 0; j < PTE_PER_PDE; j++) {
                if (ptes[j] & PG_Present)
                    unmap_pages(mem, proc->page_dir, (void *)(ptes[j] & ~0xFFF),
                                PG_SIZE);
            }
        }
    }
    // free others

    if (proc->parent_pid == 0)
        panic(" Init proc cannot exit. ");
    process *parent_proc = get_proc(proc->parent_pid);
    if (parent_proc->status & PROC_STATUS_WATING) {
        parent_proc->status &= ~PROC_STATUS_WATING;
        send_exit_to_parent(proc);
    } else {
        proc->status |= PROC_STATUS_HANGING;
    }

    process *p    = get_proc_list_head();
    process *init = get_proc(1);
    while (p) {
        if (p->parent_pid == proc->pid) {
            p->parent_pid = 1; // assign to init proc
            if ((p->status & PROC_STATUS_HANGING) &&
                (init->status & PROC_STATUS_WATING)) {
                init->status &= ~PROC_STATUS_WATING;
                send_exit_to_parent(p);
            }
        }
        p = p->next;
    }
}

void wait_proc(pid_t parent_pid) {
    process *parent_proc = get_proc(parent_pid);
    process *p           = get_proc_list_head();
    uint     children    = 0;
    while (p) {
        if (p->parent_pid == parent_pid) {
            children++;
            if (p->status & PROC_STATUS_HANGING) {
                send_exit_to_parent(p);
                return;
            }
        }
        p = p->next;
    }

    if (children)
        parent_proc->status |= PROC_STATUS_WATING;
    else {
        message msg;
        msg.type     = 0x43211234;
        msg.major    = 1;
        msg.receiver = parent_pid;
        send_msg(&msg);
    }
}