#ifndef __PROCESS_H__
#define __PROCESS_H__

#include "generic/typedefs.h"

#define PROC_STATUS_SUSPEND  0x00
#define PROC_STATUS_RUNNING  0x01
#define PROC_STATUS_READY    0x02
#define PROC_STATUS_GOTINT   0x04
#define PROC_STATUS_RESV     0x08
#define PROC_STATUS_NORMAL   0x00
#define PROC_STATUS_STOP     0x10
#define PROC_STATUS_ERROR    0x20
#define PROC_STATUS_SENDING  0x40
#define PROC_STATUS_RECEVING 0x80

#define PROC_ANY       0xFFFFFFFE
#define PROC_INTERRUPT 0xFFFFFFFF

static inline void move_to_proc() {
    extern process *proc_running;
    extern void     vector_handler_ret(void);
    asm volatile("jmp *%0" ::"r"(vector_handler_ret) : "memory");
}

// if provided pid is usable then use, else find a free pid and return it.
// ret: process' pid
uint init_proc(uint pid, void *entry, u32 page_dir);

#endif // __PROCESS_H__
