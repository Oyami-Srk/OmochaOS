#include "core/interrupt.h"
#include "core/memory.h"
#include "core/multiboot.h"
#include "core/paging.h"
#include "core/process.h"
#include "core/protect.h"
#include "driver/graphic.h"
#include "driver/misc.h"
#include "generic/typedefs.h"
#include "lib/stdlib.h"

#include "lib/syscall.h"
#include "modules/modules.h"

#include "core/environment.h"
#include "core/init.h"

uint modules[__MODULES_COUNT__]               = __MODULES_ENTRIES__;
uint modules_preferred_pid[__MODULES_COUNT__] = __MODULES_PREFERRED_PID__;

void SysIdle() {
    // reflect msg to sender
    message msg;
    while (1) {
        recv_msg(&msg, PROC_ANY);
        SEND_BACK(msg);
    }
}

unsigned int    entry_page_dir[PDE_SIZE];
struct core_env core_env;

void core_main(multiboot_info_t *multiboot_header, u32 magic) {
    memset(&core_env, 0, sizeof(struct core_env));
    init_timer();

    core_init_memory(KERN_VEND,
                     KP2V(4 * 1024 * 1024)); // to 4MB, the first remap

    memcpy(&core_env.boot_info, multiboot_header, sizeof(multiboot_info_t));

    core_env.beats     = 0;
    core_env.core_vend = (uint)KERN_VEND;
    core_env.gdt_size  = GDT_SIZE;
    core_env.idt_size  = IDT_SIZE;

    core_env.proc_table       = (process *)kalloc();
    core_env.proc_max         = (PG_SIZE / sizeof(process));
    core_env.proc_count       = 0;
    core_env.proc_bitmap      = (bitset *)kalloc();
    core_env.proc_bitmap_size = (PG_SIZE / sizeof(bitset));
    memset(core_env.proc_table, 0, PG_SIZE);
    memset(core_env.proc_bitmap, 0, PG_SIZE);

    core_init_gdt(&core_env);
    core_init_interrupt(&core_env);
    core_init_proc(&core_env);

    init_proc(0, SysIdle, (u32)entry_page_dir);
    for (uint i = 0; i < __MODULES_COUNT__; i++)
        init_proc(modules_preferred_pid[i], (void *)modules[i],
                  (u32)entry_page_dir);
    move_to_proc();
    while (1) {
        ;
    }
}

__attribute__((__aligned__(PG_SIZE))) unsigned int entry_page_dir[PDE_SIZE] = {
    [0]               = (0) | PG_Present | PG_Writeable | PG_PS,
    [KERN_BASE >> 22] = (0) | PG_Present | PG_Writeable | PG_PS};
