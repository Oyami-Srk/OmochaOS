#include "buddy.h"
#include "memory.h"
#include "paging.h"
#include <core/memory.h>
#include <core/paging.h>

void *mem_sbrk(struct memory_info *mem, process *proc, int increse) {
    if (!proc->prog_info) {
        return (void *)-1;
    }
    void *current_brk = proc->prog_info->program_break;
    void *new_brk     = current_brk + increse;
    if (PGROUNDDOWN((uint)new_brk) != PGROUNDDOWN((uint)current_brk)) {
        if (PGROUNDDOWN((uint)new_brk) < PGROUNDDOWN((uint)current_brk)) {
            // free page
            unmap_pages(mem, proc->page_dir, current_brk,
                        1); // free current_brk's page
        } else {
            // alloc page
            void *pa = page_alloc(mem, 1, PAGE_TYPE_USER | PAGE_TYPE_INUSE);
            map_pages(mem, proc->page_dir, (void *)PGROUNDDOWN((uint)new_brk),
                      pa, PG_SIZE, PG_Writable | PG_Present | PG_User);
        }
    }
    if (increse) {
        proc->prog_info->program_break = new_brk;
        proc->prog_info->program_size += increse;
    }
    return current_brk;
}