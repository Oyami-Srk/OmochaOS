#include "core/process.h"
#include "core/memory.h"
#include "core/paging.h"
#include "core/protect.h"
#include "generic/asm.h"
#include "generic/typedefs.h"
#include "lib/stdlib.h"
#include "lib/string.h"

process *proc_running;
process *proc_table;
size_t   proc_table_size;
uint     proc_count;

// proc initialized here is running under ring1
void core_init_proc(process *proc, uint pid, void *entry, u32 page_dir) {
    proc->stack.cs = SEL_CODE_DPL1;
    proc->stack.ds = SEL_DATA_DPL1;
    proc->stack.gs = SEL_DATA_DPL1;
    proc->stack.es = SEL_DATA_DPL1;
    proc->stack.fs = SEL_DATA_DPL1;
    proc->stack.ss = SEL_DATA_DPL1;
    /* proc->stack.cs = 0x08; */
    /* proc->stack.ds = 0x10; */
    /* proc->stack.gs = 0x10; */
    /* proc->stack.es = 0x10; */
    /* proc->stack.fs = 0x10; */
    /* proc->stack.ss = 0x10; */

    proc->stack.eip    = (u32)(entry);
    proc->stack.esp    = (uint)kalloc() + 4096; // task stack
    proc->stack.eflags = 0x1202;
    proc->page_dir     = (u32)KV2P(page_dir);

    proc->status = 0;
    proc->pid    = pid;
}
