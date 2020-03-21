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

#include "generic/syscall.h"
#include "modules.h"

#define GDT_SIZE 128

Gate       idt[IVT_COUNT];
Descriptor gdt[GDT_SIZE];
struct tss tss;

uint modules[__MODULES_COUNT__]               = __MODULES_ENTRIES__;
uint modules_preferred_pid[__MODULES_COUNT__] = __MODULES_PREFERRED_PID__;

uint beats = 0;

void SysIdle() {
    // reflect msg to sender
    message msg;
    while (1) {
        recv_msg(&msg, PROC_ANY);
        SEND_BACK(msg);
    }
}

void delay() {
    for (uint i = 0; i < 1000; i++)
        for (uint j = 0; j < 500; j++)
            asm volatile("nop");
}

void TaskTest(void) {
    message msg;
    while (1) {
        msg.receiver = 1;
        msg.major    = get_ticks();
        msg.type     = 2333;
        kprintf("A send\"%d\"to B.", msg.major);
        send_msg(&msg);
        delay();
    }
}

void TaskTestB(void) {
    uint    i = 0;
    message msg;
    while (1) {
        recv_msg(&msg, PROC_ANY);
        kprintf("Recv\"%d\"from%d.", msg.major, msg.sender);
        delay();
    }
}

unsigned int entry_page_dir[PDE_SIZE];

void core_main(multiboot_info_t *multiboot_header, u32 magic) {
    if (magic == MULTIBOOT_BOOTLOADER_MAGIC)
        kprintf("Venlafaxos is booting by multiboot bootloader.\n");
    else
        kprintf("Venlafaxos is booting. magic is 0x%08x\n", magic);
    kprintf("KERN_VEND: 0x%08x\n", KERN_VEND);
    kprintf("MEM_LOWER is 0x%08x\nMEM_UPPER is 0x%08x\n",
            multiboot_header->mem_lower, multiboot_header->mem_upper);
    core_init_memory(KERN_VEND,
                     KP2V(4 * 1024 * 1024)); // to 4MB, the first remap
    core_init_gdt(gdt, GDT_SIZE, &tss);
    kprintf("GDT Initialized.\n");
    init_timer();
    core_init_interrupt(idt, IVT_COUNT);
    kprintf("IDT Initialized.\n");
    core_setup_proc();
    init_proc(0, SysIdle, (u32)entry_page_dir);
    for (uint i = 0; i < __MODULES_COUNT__; i++)
        init_proc(modules_preferred_pid[i], (void *)modules[i],
                  (u32)entry_page_dir);
    /* init_proc(0, (void *)TaskTest, (u32)entry_page_dir); */
    /* init_proc(0, (void *)TaskTestB, (u32)entry_page_dir); */
    move_to_proc(0);
    while (1) {
        ;
    }
}

__attribute__((__aligned__(PG_SIZE))) unsigned int entry_page_dir[PDE_SIZE] = {
    [0]               = (0) | PG_Present | PG_Writeable | PG_PS,
    [KERN_BASE >> 22] = (0) | PG_Present | PG_Writeable | PG_PS};
