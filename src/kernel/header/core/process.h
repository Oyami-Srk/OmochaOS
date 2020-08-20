#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <generic/typedefs.h>

#define PROC_STATUS_SUSPEND  0x000
#define PROC_STATUS_RUNNING  0x001
#define PROC_STATUS_READY    0x002
#define PROC_STATUS_GOTINT   0x004
#define PROC_STATUS_GOTEXC   0x008
#define PROC_STATUS_NORMAL   0x000
#define PROC_STATUS_STOP     0x010
#define PROC_STATUS_ERROR    0x020
#define PROC_STATUS_SENDING  0x040
#define PROC_STATUS_RECEVING 0x080
#define PROC_STATUS_WATING   0x100
#define PROC_STATUS_HANGING  0x200

#define PROC_ANY       0xFFFFFFFE
#define PROC_INTERRUPT 0xFFFFFFFF

#define PROC_STACK_SIZE (8192) // 8KB

static inline void move_to_proc() {
    extern process *proc_running;
    extern void     vector_handler_ret(void);
    asm volatile("jmp *%0" ::"r"(vector_handler_ret) : "memory");
}

// if provided pid is usable then use, else find a free pid and return it.
// ret: process' pid
uint init_proc(uint pid, void *entry, pde_t *page_dir);

#endif // __PROCESS_H__
