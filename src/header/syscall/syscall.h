#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#include "kernel/structs.h"
#include "kernel/type.h"

uint get_ticks();
uint send_msg(message *msg);
uint recv_msg(message *msg, uint recv_from);

#endif // __SYSCALL_H__