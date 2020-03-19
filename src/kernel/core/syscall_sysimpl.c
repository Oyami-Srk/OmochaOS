#include "core/process.h"
#include "generic/asm.h"
#include "generic/syscall.h"
#include "generic/typedefs.h"

extern uint     beats;
extern process *proc_table;

uint __get_beats(void) { return beats; }

uint __send_msg(process *sender, message *msg) {
    msg->sender = sender->pid;

    return 0;
}

void *syscall_table[] = {__get_beats};
