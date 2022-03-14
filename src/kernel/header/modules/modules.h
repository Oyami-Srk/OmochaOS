#ifndef __MODULES_H__
#define __MODULES_H__

// code below is auto-generated by python script
// {Insert Below}

// System Task's process
extern void SysTask(void);
// Device Process
extern void Task_DEV(void);
// FS Process
extern void Task_FS(void);
// HD Process
extern void Task_HD(void);
// Memory Process
extern void Task_Memory(void);
// TTY process
extern void Task_TTY(void);
// VFS Process
extern void Task_VFS(void);

#define __MODULES_COUNT__ 7
#define __MODULES_ENTRIES__ {(unsigned int)SysTask,(unsigned int)Task_DEV,(unsigned int)Task_FS,(unsigned int)Task_HD,(unsigned int)Task_Memory,(unsigned int)Task_TTY,(unsigned int)Task_VFS}
#define __MODULES_PREFERRED_PID__ {(unsigned int)2,(unsigned int)0xFFFFFFFE,(unsigned int)0xFFFFFFFE,(unsigned int)0xFFFFFFFE,(unsigned int)0xFFFFFFFE,(unsigned int)0xFFFFFFFE,(unsigned int)0xFFFFFFFE}

// {Insert Above}
// code above is auto-generated by python script

#endif // __MODULES_H__
