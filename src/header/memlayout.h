/* 内存分布常量 */

// 启动区块常量
#ifndef __EXT_BOOT_LOADER__

#define BOOT_LOADER_MEM_INFO 0x8000
#define BOOT_LOADER_MEM_INFO_COUNT (0x8000 - 4)


#endif

// 内核基址
#define KERN_BASE 0x80000000

// 内核页目录表和页表基地址
#define KERN_PAGE_DIR_BASE 0x200000
#define KERN_PAGE_TBL_BASE 0x201000
