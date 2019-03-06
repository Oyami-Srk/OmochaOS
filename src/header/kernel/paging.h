/* 分页所需要的常量 */

#ifndef __OMOCHA_OS__PAGING_H__
#define __OMOCHA_OS__PAGING_H__

#include "type.h"

// 分页地址前10bit是页目录地址，后10bit是页表地址，最后12bit是偏移地址

#define PG_P 0x001 // 页存在位
#define PG_W 0x002 // 可写入位
#define PG_U 0x004 // 用户级位
#define PG_PS 0x080 // 页大小位

typedef struct {
  unsigned int BaseL;
  unsigned int BaseH;
  unsigned int LengthL;
  unsigned int LengthH;
  unsigned int Type;
  unsigned int ACPI;
}__attribute__((packed)) SMAP_entry;


void kinit_paging(unsigned int smap_entry_count, SMAP_entry *first, cpu_env *env);
void kprint_meminfo(SMAP_entry *mem_info_start, unsigned int count);

#endif
