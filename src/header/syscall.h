#ifndef __OMOCHA_OS__SYSCALL_H__
#define __OMOCHA_OS__SYSCALL_H__

#include "type.h"
#include "const.h"

#define SYSCALL_INT 0xE9

extern void* syscall_table[];

#endif
