#ifndef __OMOCHA_OS__CONFIG_H__
#define __OMOCHA_OS__CONFIG_H__

#include "protect.h"

// sync to const.h
static uint KERN_GDT[][3] = {
                           {0, 0, 0}, // Dummy null desc
                           {0, 0xFFFFFFFF, DA_32 | DA_4K | DA_C}, // Code desc
                           {0, 0xFFFFFFFF, DA_32 | DA_4K | DA_DRW}, // Data desc
                           {0xB8000, 0xFFFF, DA_DRW | DA_DPL3}, // Video desc
                           {0x0, 0x0, DA_386TSS}, // Tss desc
                           {0x0, 0x0, DA_LDT} // LDT desc
                           // You cannot define more descriptor below ldt
};

#endif

