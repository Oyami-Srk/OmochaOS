#ifndef __OMOCHA_OS__SYSCALL_H__
#define __OMOCHA_OS__SYSCALL_H__

#include "kernel/type.h"

#define SYSCALL_INT 0xE9

#define SC_GET_TICKS 0x01
#define SC_DONE 0x0FF00FF0

// real syscall
uint get_ticks();
uint send_msg(message *msg);
uint recv_msg(message *msg, uint recv_from);

// fake syscall
uint get_ticks_msg();


#endif
