#ifndef __PROC_H__
#define __PROC_H__

#include "const.h"
#include "kernel/structs.h"
#include "kernel/type.h"

struct __process {
  stack_frame stack;
  u16 selector_ldt;
  Descriptor ldts[LDT_SIZE_PER_PROC];
  uint status;
  uint pid;
  char name[16];
};

typedef struct __process process;
typedef void (*fp_v_v)(void);
typedef void *(*fp_vp_v)(void);

typedef void *(*fp_v_vp)(void *);

typedef void *(*fp_vp_vp)(void *);

extern void vector_handler_ret();

void init_proc(process *proc, uint pid, fp_v_v entry);
static inline void start_proc() {
  asm volatile("jmp %0" ::"r"(vector_handler_ret) : "memory");
}

#endif