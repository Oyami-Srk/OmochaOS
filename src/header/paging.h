/* 分页所需要的常量 */

// 分页地址前10bit是页目录地址，后10bit是页表地址，最后12bit是偏移地址

#define PG_SIZE 0x1000
#define PDE_SIZE 1024

#define PG_P 0x001 // 页存在位
#define PG_W 0x002 // 可写入位
#define PG_U 0x004 // 用户级位
#define PG_PS 0x080 // 页大小位
