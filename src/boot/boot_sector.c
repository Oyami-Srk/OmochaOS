#include "elf.h"
#include "memory_map.h"
/* #include "const.h" */

void boot_main(){
  /*
    MBR直接跳入保护模式，并且暴露_gdt的地址
    约定loader仅完成以下工作：
    * 从硬盘读取内核并加载
    * 读取基本的硬件信息并且写入Infos结构体
    * 调用内核入口点，该入口点不可能返回。
    * Infos结构体的地址由memory map约定
   */
  
}
