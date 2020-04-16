#include "buddy.h"
#include "core/memory.h"
#include "core/paging.h"
#include "core/process.h"
#include "lib/stdlib.h"
#include "memory.h"
#include "modules/systask.h"
#include "paging.h"

#include "driver/graphic.h"

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

process *free_proc(struct memory_info *mem, process *proc) {
    // free stack
    char *stack_va_start = proc->pstack;
    char *stack_va_end   = proc->pstack + proc->pstack_size;
    for (char *va = (char *)PGROUNDDOWN((uint)stack_va_start);
         va < (char *)PGROUNDUP((uint)stack_va_end); va += PG_SIZE) {
        char *pa   = vir2phy(proc->page_dir, va);
        int   refs = decrease_page_ref(mem, pa);
        if (refs == 0) {
            page_free(mem, pa, 1);
        }
    }
    for (uint i = ((uint)stack_va_start >> 22); i < ((uint)stack_va_end >> 22);
         i++) {
        int refs = decrease_page_ref(mem, (void *)(proc->page_dir[i] & ~0xFFF));
        if (refs == 0) {
            page_free(mem, (char *)(proc->page_dir[i] & ~0xFFF), 1);
        }
        proc->page_dir[i] = 0;
    }

    proc->pstack_size = NULL;
    proc->pstack      = NULL;

    // free program
    if (proc->prog_info) {
        char *pg_start = proc->prog_info->image_start;
        char *pg_end   = proc->prog_info->program_break;
        for (char *va = (char *)PGROUNDDOWN((uint)pg_start);
             va < (char *)PGROUNDUP((uint)pg_end); va += PG_SIZE) {
            char *pa   = vir2phy(proc->page_dir, va);
            int   refs = decrease_page_ref(mem, pa);
            if (refs == 0) {
                page_free(mem, pa, 1);
            }
        }
        for (uint i = ((uint)pg_start >> 22); i < ((uint)pg_end >> 22); i++) {
            int refs =
                decrease_page_ref(mem, (void *)(proc->page_dir[i] & ~0xFFF));
            if (refs == 0) {
                page_free(mem, (char *)(proc->page_dir[i] & ~0xFFF), 1);
            }
            proc->page_dir[i] = 0;
        }
    }

    mem_kfree((char *)proc->prog_info);
    proc->prog_info = NULL;
    page_free(mem, (char *)proc->page_dir, 1);
    return proc;
}

void mem_exit_proc(process *proc) {
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