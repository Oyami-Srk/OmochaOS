#ifndef __PROC_H__
#define __PROC_H__

#include "const.h"
#include "kernel/structs.h"
#include "kernel/type.h"

#define PROC_STATUS_SUSPEND 0x00
#define PROC_STATUS_RUNNING 0x01
#define PROC_STATUS_READY 0x02
#define PROC_STATUS_GOTINT 0x04
#define PROC_STATUS_RESV 0x08

#define PROC_STATUS_NORMAL 0x00
#define PROC_STATUS_STOP 0x10
#define PROC_STATUS_ERROR 0x20
#define PROC_STATUS_SENDING 0x40
#define PROC_STATUS_RECEVING 0x80

#define GET_PROC_STATUS_PID(p)                                                 \
  ((((((process *)((p)))->status) >> 8) & 0x00FFFF))
#define SET_PROC_STATUS_PID(p, x)                                              \
  ((((process *)((p)))->status) |= (((x)&0xFFFF) << 8))
#define CLR_PROC_STATUS_PID(p) (((process *)((p)))->status &= 0xFF0000FF)
#define ANY 0
#define REFUSE 0xFFFF
#define INTERRUPT 0xFFFE
struct __process {
  stack_frame stack;
  u16 selector_ldt;
  Descriptor ldts[LDT_SIZE_PER_PROC];
  uint status;
  uint pid;
  char name[16];

  message *p_msg;
  struct __process *quene_head_sending_to_this_process;
  struct __process *quene_body;
} __attribute__((packed));

typedef struct __process process;
typedef void (*fp_v_v)(void);
typedef void *(*fp_vp_v)(void);

typedef void *(*fp_v_vp)(void *);

typedef void *(*fp_vp_vp)(void *);

extern process *current_running_proc; // proc.c
extern process *proc_table;           // proc.c

extern void vector_handler_ret();
void init_proc(process *proc, uint pid, fp_v_v entry);

void init_proctable(process *proc_table, size_t max_proc);
void scheduler(uint); // be seperated later
#define SCHEDULER_MAX_RETRY 10
static inline void start_proc() {
  current_running_proc = proc_table;
  asm volatile("jmp %0" ::"r"(vector_handler_ret) : "memory");
}

#endif