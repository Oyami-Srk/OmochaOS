#include "syscall.h"
#include "klib.h"

extern cpu_env cpu;

int __get_ticks(void){
  return cpu.beats;
}

int __test_parm(void *v1, int v2, int v3, int v4){
  return v2+v3+v4;
}

void* syscall_table[] = {
                         __get_ticks,
                         __test_parm
};
