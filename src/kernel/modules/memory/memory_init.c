#include "buddy.h"
#include "core/memory.h"
#include "core/paging.h"
#include "core/process.h"
#include "core/protect.h"
#include "lib/stdlib.h"
#include "memory.h"
#include "paging.h"

#include "modules/tty.h"

extern void init(void); // init.c

void start_up_init(struct memory_info *mem) {
    char *   pstack_top    = (char *)KERN_BASE;
    char *   pstack_bottom = (char *)(KERN_BASE - PROC_STACK_SIZE);
    process *init_proc     = get_proc(1);
    init_proc->status      = PROC_STATUS_STOP;

    init_proc->stack.cs = SEL_CODE_DPL1;
    init_proc->stack.ds = SEL_DATA_DPL1;
    init_proc->stack.es = SEL_DATA_DPL1;
    init_proc->stack.fs = SEL_DATA_DPL1;
    init_proc->stack.gs = SEL_DATA_DPL1;
    init_proc->stack.ss = SEL_DATA_DPL1;

    init_proc->stack.eip    = (u32)init;
    init_proc->pstack       = pstack_bottom;
    char *stack             = page_alloc(mem, PROC_STACK_SIZE / PG_SIZE,
                             PAGE_TYPE_INUSE | PAGE_TYPE_USER); // 8KB
    init_proc->pstack_size  = PROC_STACK_SIZE;
    init_proc->stack.esp    = (uint)pstack_top;
    init_proc->stack.eflags = 0x1202;
    init_proc->page_dir     = create_page_dir(mem);
    map_pages(mem, (pde_t *)init_proc->page_dir, init_proc->pstack, stack,
              init_proc->pstack_size, PG_Present | PG_Writable | PG_User);

    init_proc->pid        = 1;
    init_proc->parent_pid = 0;

    extern process **proc_list;  // process.c
    extern size_t *  proc_count; // process.c

    process *p = *proc_list;
    process *f = p;
    while (p) {
        f = p;
        p = p->next;
    }
    init_proc->next = NULL;
    f->next         = init_proc;
    (*proc_count)++;
    init_proc->status = PROC_STATUS_RUNNING | PROC_STATUS_NORMAL;
}

// page_id = (page_paddr - pstart) / PG_SIZE
// pg_count = (end-start) / PG_SIZE
// buddy bits = (pg_count / (2^order)) / 2 = page_count >> (order+1)
// total buddy bits = ((2^MAX_ORDER)-1)*pg_count/(2^MAX_ORDER)

void init_memory(struct memory_info *mem) {
    printf("[MEM] Init memory 0x%x - 0x%x\n", mem->memory_start,
           mem->memory_end);
    size_t pg_count     = (mem->memory_end - mem->memory_start) / PG_SIZE;
    size_t pg_info_size = pg_count * sizeof(struct page);
    mem->page_count     = pg_count;
    uint buddy_total_bits =
        ((1 << (MAX_ORDER)) - 1) * pg_count / (1 << MAX_ORDER);
    size_t buddy_bytes    = buddy_total_bits / 8;
    uint   max_block_size = (1 << (MAX_ORDER - 1)) * PG_SIZE;
    mem->usable_end       = ROUNDDOWN_WITH(
        max_block_size, (mem->memory_end - buddy_bytes - pg_info_size));
    mem->pages_info    = (struct page *)(mem->usable_end);
    mem->buddy_map[0]  = (bitset *)(mem->memory_end - buddy_bytes);
    mem->free_list[0]  = (block_list *)NULL;
    mem->free_count[0] = 0;
    printf("[MEM] Init buddy map on 0x%x, size %d bytes.\n", mem->buddy_map,
           buddy_bytes);
    printf("[MEM] Init pages info on %x, size %d bytes.\n", mem->pages_info,
           pg_info_size);
    printf("[MEM] Total occupied mem size: %d bytes(%d megabytes).\n",
           mem->memory_end - mem->usable_end,
           (mem->memory_end - mem->usable_end) / 1024 / 1024);
    memset((void *)mem->usable_end, 0, mem->memory_end - mem->usable_end);
    for (uint i = 0; i < MAX_ORDER - 1; i++) {
        mem->buddy_map[i + 1] =
            (bitset *)(((uint)mem->buddy_map[i]) + (pg_count >> (i + 1)) / 8);
        mem->free_list[i + 1]  = (block_list *)NULL;
        mem->free_count[i + 1] = 0;
    }
    void *pg = NULL;
    for (pg = (void *)mem->memory_start; pg < (void *)mem->usable_end;
         pg += max_block_size) {
        // free max block first
        block_list *current = (block_list *)pg;
        // attach_to_free_list(mem, current, MAX_ORDER - 1);

        current->next                 = mem->free_list[MAX_ORDER - 1];
        current->next->prev           = current;
        current->prev                 = NULL;
        mem->free_list[MAX_ORDER - 1] = current;

        mem->free_count[MAX_ORDER - 1]++;
    }
    printf("[MEM] pages info size: %d KB\n",
           pg_count * sizeof(struct page) / 1024);
    for (uint i = 0; i < pg_count; i++) {
        if (GET_PAGE_BY_ID(mem, i) < mem->usable_end)
            mem->pages_info[i].type = PAGE_TYPE_FREE | PAGE_TYPE_USABLE;
        else
            mem->pages_info[i].type = PAGE_TYPE_INUSE | PAGE_TYPE_SYSTEM;
    }
    printf("[MEM] Initialized. Total %d pages. 4M Block Count: %d.\n", pg_count,
           mem->free_count[MAX_ORDER - 1]);
    return;
}