#include "syscall.h"
#include "klib.h"

extern cpu_env cpu;

int get_ticks(void){
  return cpu.beats;
}

void* syscall_table[] = {
                         get_ticks
};
