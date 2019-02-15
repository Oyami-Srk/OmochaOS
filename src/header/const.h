#ifndef __OMOCHA_OS__CONST_H__
#define __OMOCHA_OS__CONST_H__

#define GDT_COUNT 128
#define IDT_COUNT 256

#define GDT_CODE_INDEX 1
#define SEL_CODE (GDT_CODE_INDEX << 3)

#define PG_SIZE 0x1000
#define PDE_SIZE 1024



#endif
