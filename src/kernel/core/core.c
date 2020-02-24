#include "core/interrupt.h"
#include "core/memory.h"
#include "core/multiboot.h"
#include "core/paging.h"
#include "core/process.h"
#include "core/protect.h"
#include "driver/graphic.h"
#include "generic/typedefs.h"

#define GDT_SIZE 128

Gate       idt[IVT_COUNT];
Descriptor gdt[GDT_SIZE];
struct tss tss;

uint beats = 0;

void core_main(multiboot_info_t *multiboot_header, u32 magic) {
    if (magic == MULTIBOOT_HEADER_MAGIC)
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
    core_init_interrupt(idt, IVT_COUNT);
    kprintf("IDT Initialized.\n");
    kprintf("gdt is located in 0x%08x\n", gdt);
    while (1) {
        kprintf("%d.", beats);
        for (uint i = 0; i < 100000000; i++)
            ;
    }
}

__attribute__((__aligned__(PG_SIZE))) unsigned int entry_page_dir[PDE_SIZE] = {
    [0]               = (0) | PG_Present | PG_Writeable | PG_PS,
    [KERN_BASE >> 22] = (0) | PG_Present | PG_Writeable | PG_PS};
