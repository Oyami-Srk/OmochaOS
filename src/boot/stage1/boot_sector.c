#include "elf.h"
#include "memory_map.h"
#include "asm_x86.h"
/* #include "const.h" */

typedef unsigned char u_byte;
typedef unsigned short u_word;
typedef unsigned int u_dword;
typedef unsigned int uint;


void readsect(u_byte *addr, uint offset){
  while((inb(0x1F7) & 0xC0) != 0x40); // 等待磁盘
  outb(0x1F2, 1); // count
  outb(0x1F3, offset);
  outb(0x1F4, offset>>8);
  outb(0x1F5, offset>>16);
  outb(0x1F6, (offset >> 24) | 0xE0);
  outb(0x1F7, 0x20); // cmd 0x20: 读取扇区

  while((inb(0x1F7) & 0xC0) != 0x40);
  insl(0x1F0, addr, 512 / 4);
}

void readseg(u_byte *addr, uint count, uint offset){
  u_byte *end_addr;
  end_addr = addr+count;
  // 每次读取512字节（一扇区）的内容，真正地址需要向后偏移以使传入的addr指向正确的数据。
  addr -= offset % 512;
  // 转换字节偏移到扇区偏移
  offset = (offset / 512) + 1;
  for(;addr < end_addr; addr += 512, offset++)
    readsect(addr, offset);
}


void boot_main(){
  /*
    MBR直接跳入保护模式，并且暴露_gdt的地址
    约定loader仅完成以下工作：
    * 从硬盘读取内核并加载
    * 读取基本的硬件信息并且写入Infos结构体
    * 调用内核入口点，该入口点不可能返回。
    * Infos结构体的地址由memory map约定
   */
  ELF32_Header *elf;
  ELF32_Prog *ph,*eph;
  void (*entry)(void);
  u_byte *addr;

  elf = (ELF32_Header*)0x10000;

  readseg((u_byte*)elf, 4096, 0); // 0 means 1 sector

  if(elf->magic != ELF_MAGIC)
    return;

  ph = (ELF32_Prog*)((u_byte*)elf + elf->phoff);
  eph = ph + elf->phnum;
  for(;ph<eph;ph++){
    addr = (u_byte*)ph->paddr;
    readseg(addr, ph->filesz, ph->off);
    if(ph->memsz > ph->filesz) // 内存大小大于文件，补0
      stosb(addr + ph->filesz, 0, ph->memsz - ph->filesz);
  }

  entry = (void(*)(void))(elf->entry);
  entry();
}
