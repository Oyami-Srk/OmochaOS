#ifndef __MODULES_H__
#define __MODULES_H__

#include "kernel/type.h"

// code below is auto-generated by python script
// {Insert Below}

// FS Process
extern void Task_FS();
// TTY Process
extern void Task_TTY();
// HD Process
extern void Task_HD();
#define __MODULES_COUNT__ 3
#define __MODULES_DEFINES__ {(uint)Task_FS,(uint)Task_TTY,(uint)Task_HD}

// {Insert Above}
// code above is auto-generated by python script

#endif // __MODULES_H__