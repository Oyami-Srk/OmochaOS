#include "core/process.h"
#include "generic/asm.h"
#include "generic/syscall.h"
#include "generic/typedefs.h"

extern uint beats;

uint __get_beats(void) { return beats; }

void *syscall_table[] = {__get_beats};
