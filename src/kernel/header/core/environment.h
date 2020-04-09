#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

#include "core/interrupt.h"
#include "core/multiboot.h"
#include "core/process.h"
#include "core/protect.h"
#include "generic/typedefs.h"
#include "lib/bitset.h"

#define GDT_SIZE        128
#define IDT_SIZE        256 // must match IVT.asm
#define MEMORY_ZONE_MAX 16

struct core_env_memory_zone {
    uint addr;
    uint length;
    uint type;
#define MEMORY_AVAILABLE        1
#define MEMORY_RESERVED         2
#define MEMORY_ACPI_RECLAIMABLE 3
#define MEMORY_NVS              4
#define MEMORY_BADRAM           5
};

struct core_memory_usage {
    uint core_space_start;
    uint core_space_free_start;
    uint core_space_free_end;
    uint core_space_end;
    uint memory_end;
};

struct core_env {
    uint             beats;
    multiboot_info_t boot_info; // notice: mem_upper and mem_lower is in kb
    uint             core_vend;

    /*      memory usage start     */
    uint core_space_start;
    uint core_space_free_start;
    uint core_space_free_end;
    uint core_space_end;
    /*      memory usage end       */

    uint       memory_end;
    size_t     gdt_size; // = GDT_SIZE
    Descriptor gdt[GDT_SIZE];
    size_t     idt_size; // = IDT_SIZE
    Gate       idt[IDT_SIZE];
    struct tss tss;
    pde_t *    page_dir;
    process *  proc_table;
    process *  proc_list;
    size_t     proc_count;
    size_t     proc_max;
    bitset *   proc_bitmap;
    size_t     proc_bitmap_size; // count of array

    struct interrupt_method interrupt_methods[HW_IRQ_COUNT];
    uint                    interrupt_suscribed[HW_IRQ_COUNT];
    uint                    exception_suscribed[EXCEPTION_COUNT];
    struct interrupt_data   interrupt_suscribed_data[HW_IRQ_COUNT];
    struct interrupt_data   exception_suscribed_data[EXCEPTION_COUNT];

    struct core_env_memory_zone memory_zone[MEMORY_ZONE_MAX];
    uint                        memory_zone_count;
} __attribute__((aligned(4096))); // 4KB aligned

#endif // __ENVIRONMENT_H__
