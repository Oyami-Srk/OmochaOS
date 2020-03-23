#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

#include "core/interrupt.h"
#include "core/multiboot.h"
#include "core/process.h"
#include "core/protect.h"
#include "generic/typedefs.h"
#include "lib/bitset.h"

#define GDT_SIZE 128
#define IDT_SIZE 256 // must match IVT.asm

struct core_env {
    uint             beats;
    multiboot_info_t boot_info;
    uint             core_vend;
    size_t           gdt_size; // = GDT_SIZE
    Descriptor       gdt[GDT_SIZE];
    size_t           idt_size; // = IDT_SIZE
    Gate             idt[IDT_SIZE];
    struct tss       tss;
    process *        proc_table;
    size_t           proc_count;
    size_t           proc_max;
    bitset *         proc_bitmap;
    size_t           proc_bitmap_size;

    struct interrupt_method interrupt_methods[HW_IRQ_COUNT];
    uint                    interrupt_suscribed[HW_IRQ_COUNT];
};

#endif // __ENVIRONMENT_H__
