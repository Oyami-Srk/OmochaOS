#ifndef __OMOCHA_OS__CONST_H__
#define __OMOCHA_OS__CONST_H__

#define GDT_COUNT 128
#define IDT_COUNT 256

#define PROC_COUNT 2

#define GDT_CODE_INDEX 1
#define GDT_DATA_INDEX 2
#define GDT_VIDEO_INDEX 3
#define GDT_TSS_INDEX 4
#define GDT_LDT1_INDEX 5
#define SEL_KCODE (GDT_CODE_INDEX << 3)
#define SEL_KDATA (GDT_DATA_INDEX << 3)
#define SEL_KVIDEO (GDT_VIDEO_INDEX << 3) | SA_RPL3
#define SEL_TSS (GDT_TSS_INDEX << 3)
#define SEL_LDT1 (GDT_LDT1_INDEX << 3)

#define PG_SIZE 0x1000
#define PDE_SIZE 1024

#define LDT_SIZE 2
#define PROCESS_NAME_SIZE 16

#endif
