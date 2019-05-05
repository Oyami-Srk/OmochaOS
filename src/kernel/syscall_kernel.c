#include "kernel/type.h"
#include "lib/string.h"
#include "syscall/syscall.h"

extern uint beats;

uint __get_ticks(void) { return beats; }

uint __add(void *caller, uint a, uint b, uint c) { return a + b + c; }

void *syscall_table[] = {__get_ticks, __add};