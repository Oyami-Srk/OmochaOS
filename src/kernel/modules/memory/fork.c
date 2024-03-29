#include "buddy.h"
#include "memory.h"
#include "paging.h"
#include <core/memory.h>
#include <core/paging.h>
#include <core/process.h>
#include <generic/asm.h>
#include <lib/stdlib.h>
#include <modules/systask.h>

static pde_t *copy_parent(struct memory_info *mem, process *parent) {
    pde_t *pg_dir = create_page_dir(mem, PG_Writable);

    // copy stack
    char *stack_va_start = parent->pstack;
    char *stack_va_end =
        parent->pstack + parent->pstack_size; // end is KERN_BASE, donot map
    for (uint i = ((uint)stack_va_start >> 22); i < ((uint)stack_va_end >> 22);
         i++) {
        // parent->page_dir[i] &= ~PG_Writable; // only pte needs set ro
        pg_dir[i] = parent->page_dir[i];
        increase_page_ref(
            mem,
            (void *)(pg_dir[i] & ~0xFFF)); // increase page table's reference
    }
    for (char *va = (char *)PGROUNDDOWN((uint)stack_va_start);
         va < (char *)PGROUNDUP((uint)stack_va_end); va += PG_SIZE) {
        char *pa = vir2phy(pg_dir, va);
        *get_pte(pg_dir, va) &= ~PG_Writable; // clear every page's write bit
        increase_page_ref(mem, pa);
        // kprintf("pa(%x): %d\n", pa, get_page_reference(mem, pa));
    }
    // copy program
    if (parent->prog_info) {
        char *pg_start = parent->prog_info->image_start;
        char *pg_end   = parent->prog_info->program_break;
        for (uint i = ((uint)pg_start >> 22); i <= ((uint)pg_end >> 22); i++) {
            pg_dir[i] = parent->page_dir[i];
            increase_page_ref(mem, (void *)(pg_dir[i] & ~0xFFF));
        }
        for (char *va = (char *)PGROUNDDOWN((uint)pg_start);
             va < (char *)PGROUNDUP((uint)pg_end); va += PG_SIZE) {
            char *pa = vir2phy(pg_dir, va);
            *get_pte(pg_dir, va) &= ~PG_Writable;
            increase_page_ref(mem, pa);
        }
    }

    // set msg's page wrtable
    char *msg_va = (char *)parent->p_msg;
    if (!msg_va)
        return pg_dir;
    size_t msg_pgs = 1;
    if ((0xFFF - ((uint)msg_va & 0xFFF)) < sizeof(message))
        msg_pgs = 2;
    msg_va = (char *)PGROUNDDOWN((uint)msg_va);
    char *msg_va_end =
        (char *)PGROUNDDOWN((uint)msg_va + msg_pgs * PG_SIZE - 1);
    for (; msg_va <= msg_va_end; msg_va += PG_SIZE) {
        char *old_pa = vir2phy(parent->page_dir, msg_va);
        char *new_pa = page_alloc(mem, 1, PAGE_TYPE_INUSE | PAGE_TYPE_USER);
        memcpy(new_pa, old_pa, PG_SIZE);
        // kprintf("old pa(%x): %d\n", old_pa, get_page_reference(mem, old_pa));
        decrease_page_ref(mem, old_pa);

        pde_t *pde        = &pg_dir[(uint)msg_va >> 22];
        pte_t *old_pg_tab = (pte_t *)(*pde & ~0xFFF);
        decrease_page_ref(mem, old_pg_tab);

        pte_t *pg_tab =
            (pte_t *)page_alloc(mem, 1, PAGE_TYPE_INUSE | PAGE_TYPE_PGTBL);
        memcpy(pg_tab, old_pg_tab, PG_SIZE);
        *pde = (uint)pg_tab | PG_Present | PG_User | PG_Writable;

        *get_pte(pg_dir, msg_va) =
            (uint)new_pa | PG_Present | PG_User | PG_Writable;
    }

    return pg_dir;
}

// return child proc's pid
uint fork_proc(struct memory_info *mem, pid_t pid, process **par,
               process **chi) {
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
    // child_proc->page_dir = copy_page_dir(mem, parent_proc->page_dir);
    child_proc->page_dir = copy_parent(mem, parent_proc);
    child_proc->prog_info =
        (struct prog_info *)mem_kmalloc(sizeof(struct prog_info));
    memcpy(child_proc->prog_info, parent_proc->prog_info,
           sizeof(struct prog_info));

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

    // parent_proc->status &= ~PROC_STATUS_STOP;
    child_proc->status = parent_proc->status;

    *par = parent_proc;
    *chi = child_proc;

    return child_proc->pid;
}