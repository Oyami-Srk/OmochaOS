#ifndef __PROCESS_H__
#define __PROCESS_H__

#include "generic/typedefs.h"

#define LDT_SIZE 2

typedef struct {
    u16  gs, pd1, fs, pd2, es, pd3, ds, pd4;
    u32  edi, esi, ebp, oesp, ebx, edx, ecx, eax;
    uint trap_no;
    uint err_code;
    u32  eip;
    u16  cs, pd5;
    u32  eflags, esp;
    u16  ss, pd6;
} __attribute__((packed)) stack_frame;

struct tss {
    u32 backlink;
    u32 esp0, ss0, esp1, ss1, esp2, ss2, cr3, eip, flags, eax, ecx, edx, ebx,
        esp, ebp, esi, edi, es, cs, ss, ds, fs, gs, ldt;
    u16 trap;
    u16 iobase; /* I/O位图基址大于或等于TSS段界限，就表示没有I/O许可位图 */
                // char iomap[2];
} __attribute__((packed));

struct __process {
    stack_frame stack;
    u32         page_dir;
    uint        status;
    uint        pid;
    char        name[16];
} __attribute__((packed));

typedef struct __process process;

static inline void move_to_proc(uint pid) {
    extern process *proc_running;
    extern void     vector_handler_ret(void);
    extern process *proc_table;
    proc_running = &proc_table[pid];
    asm volatile("jmp *%0" ::"r"(vector_handler_ret) : "memory");
}

void core_setup_proc();
// if provided pid is usable then use, else find a free pid and return it.
// ret: process' pid
uint init_proc(uint pid, void *entry, u32 page_dir);

#endif // __PROCESS_H__
