#include "core/memory.h"
#include "core/paging.h"
#include "core/process.h"
#include "lib/stdlib.h"
#include "memory.h"
#include "modules/systask.h"
#include "paging.h"

// return child proc's pid
uint fork_proc(struct memory_info *mem, pid_t pid) {
    process *parent_proc = get_proc(pid);
    parent_proc->status |= PROC_STATUS_STOP; // stop to wait to be forked
    process *child_proc = alloc_proc();
    if (child_proc == NULL)
        return 0;
    child_proc->parent_pid = pid;
    memcpy(&child_proc->stack, &parent_proc->stack, sizeof(stack_frame));
    child_proc->pstack                             = parent_proc->pstack;
    child_proc->pstack_size                        = parent_proc->pstack_size;
    child_proc->quene_body                         = NULL;
    child_proc->quene_head_sending_to_this_process = NULL;
    // child_proc pmsg is pa of (va of(par pmsg))
    child_proc->p_msg = parent_proc->p_msg;
    /* child_proc->page_dir = create_page_dir(); */
    child_proc->page_dir = copy_page_dir(mem, parent_proc->page_dir);
    map_pages(mem, child_proc->page_dir, child_proc->pstack,
              /*
                page_alloc(child_proc->pstack_size / PG_SIZE,
                           PAGE_TYPE_INUSE | PAGE_TYPE_USER),
                           */
              vir2phy(parent_proc->page_dir, parent_proc->pstack),
              child_proc->pstack_size, PG_Present | PG_Writeable | PG_User);
    message *msg = parent_proc->p_msg;
    if (msg) { // parpare for child if msg is set
        char *msg_start_pg = (char *)PGROUNDDOWN((uint)msg);
        char *msg_end_pg   = (char *)PGROUNDUP((uint)msg);
    }
    /*
    memcpy(vir2phy(child_proc->page_dir, child_proc->pstack),
           vir2phy(parent_proc->page_dir, parent_proc->pstack),
           child_proc->pstack_size);
           */

    parent_proc->status &= ~PROC_STATUS_STOP;
    child_proc->status = parent_proc->status;

    return child_proc->pid;
}