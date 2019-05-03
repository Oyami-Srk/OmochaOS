#include "kernel/proc.h"
#include "kernel/memory.h"
#include "kernel/pm.h"

void init_proc(process *proc, uint pid, fp_v_v entry) {
  make_descriptor(&proc->ldts[0], 0x0, 0xFFFFFFFF, DA_32 | DA_4K | DA_C);
  make_descriptor(&proc->ldts[1], 0x0, 0xFFFFFFFF, DA_32 | DA_4K | DA_DRW);
  proc->ldts[0].attr1 = DA_C | (1 << 5); // dpl 1
  proc->ldts[1].attr1 = DA_DRW | (1 << 5);
  proc->stack.cs = (0 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | SA_RPL1;
  proc->stack.ds = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | SA_RPL1;
  proc->stack.es = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | SA_RPL1;
  proc->stack.fs = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | SA_RPL1;
  proc->stack.ss = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | SA_RPL1;
  proc->stack.gs = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | SA_RPL1;

  proc->stack.eip = (uint)entry;
  proc->stack.esp = (uint)kalloc() + 4096;
  proc->stack.eflags = 0x1202;

  proc->status = 0;
  proc->pid = pid;
}
