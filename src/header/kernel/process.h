#ifndef __OMOCHA_OS__PROCESS_H__
#define __OMOCHA_OS__PROCESS_H__

#include "type.h"
#include "config.h"

// 32bit uint status:
// |--8bits for OwnerID (Not supported yet)--|
// |--16bits for pid which msg sending(receiving) through--|
// |--8bit for status--|
#define PROC_STATUS_SUSPEND 0x00
#define PROC_STATUS_RUNNING 0x01
#define PROC_STATUS_READY   0x02
#define PROC_STATUS_GOTINT  0x04

#define PROC_STATUS_NORMAL   0x00 //only normal can be scheled
#define PROC_STATUS_STOP     0x10
#define PROC_STATUS_ERROR    0x20
#define PROC_STATUS_SENDING  0x40
#define PROC_STATUS_RECEVING 0x80

#define GET_PROC_STATUS_PID(p) ((((((process*)((p)))->status) >> 8) & 0x00FF))
#define SET_PROC_STATUS_PID(p,x) (((((process*)((p)))->status) |= ((((x)) << 8) & 0xFF)))

#define ANY 0
#define REFUSE 0xFFFF
#define INTERRUPT 0xFFFE

#endif
