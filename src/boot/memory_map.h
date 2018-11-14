/*
  内存映射定义
  用于Loader和Boot_sector
  不用于Kernel，Kernel可能会重新定义一些内存地址
 */

// BIOS 内存布局
// 中断向量表，1024字节
#define INT_VECTOR_S 0x0000
#define INT_VECTOR_E 0x03FF
// BIOS数据区
#define BIOS_DATA_S 0x0400
#define BIOS_DATA_E 0x04FF
// 启动扇区加载地址
#define BOOT_SECT_S 0x7C00
#define BOOT_SECT_E 0x7DFF
// 拓展BIOS数据区
#define BIOS_EDATA_S 0x9FC00
#define BIOS_EDATA_E 0x9FFFF
// EGA/VGA/XGA/XVGA 图像Buffer
#define GRAPH_BUFFER_S 0xA0000
#define GRAPH_BUFFER_E 0xAFFFF
// 单色字符Buffer
#define MONO_TEXT_S 0xB0000
#define MONO_TEXT_E 0xB7FFF
// 多彩字符Buffer
#define CHROMA_TEXT_S 0xB8000
#define CHROMA_TEXT_E 0xBFFFF
// BIOS运行时
#define BIOS_RT_S 0xC0000
#define BIOS_RT_E 0xFDFFF
// BIOS 启动区域
#define BIOS_BOOT_S 0xFE000
#define BIOS_BOOT_E 0xFFFFF
#define BIOS_BOOT_ENTRY 0xFFFF0

// 我们的区域

// Loader从硬盘读取加载的内存地址，64k
#define OMO_LOADER_LOAD_POINT 0x80000
#define OMO_LOADER_LOAD_SIZE 0x10000
