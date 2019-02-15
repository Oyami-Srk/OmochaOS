/* 内存分布常量 */

#ifndef __OMOCHA_OS__MEMLAYOUT_H__
#define __OMOCHA_OS__MEMLAYOUT_H__

#include "const.h"
#include "protect.h"
#include "interrupt.h"

// 启动区块常量
#ifndef __EXT_BOOT_LOADER__

#define BOOT_LOADER_MEM_INFO 0x8000
#define BOOT_LOADER_MEM_INFO_COUNT (0x8000 - 4)

#endif

// 内核基址
#define KERN_BASE 0x80000000

#define P2V(a) ((void *)(((char *) (a)) + KERN_BASE))
#define V2P(a) ((void *)(((char *) (a)) - KERN_BASE))

// sizeof descriptor is 8byte 32bit

#define KERN_GDT_BASE 0x80200000
#define KERN_IDT_BASE (KERN_GDT_BASE + (GDT_COUNT * sizeof(GDT_Descriptor)))
// 内核页目录表和页表基地址
#define KERN_PAGE_DIR_BASE (KERN_IDT_BASE + (IDT_COUNT * sizeof(Gate_Descriptor)))
#define KERN_PAGE_TBL_BASE (KERN_PAGE_DIR_BASE + 0x1000)

extern char KERN_END[]; // kernel end addr, provided by kernel.ld

#endif
