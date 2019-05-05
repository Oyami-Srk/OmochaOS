#include "kernel/type.h"
#include "lib/string.h"
#include "syscall/syscall.h"

extern uint beats;

uint __get_ticks(void) { return beats; }

void *syscall_table[] = {__get_ticks};