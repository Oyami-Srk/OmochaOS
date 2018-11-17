#ifndef __OMO_ASM_X86_H__
#define __OMO_ASM_X86_H__
// 内联汇编函数，从xv6抄的

static inline void insl(int port, void *addr, int cnt){
  asm volatile("cld; rep insl"
               :"=D"(addr), "=c"(cnt)
               : "d" (port), "0" (addr), "1" (cnt)
               :"memory","cc");
}

static inline void outb(unsigned short port, unsigned char data){
  asm volatile("out %0,%1"
               :
               :"a"(data),"d"(port));
}

static inline unsigned char inb(unsigned short port){
  unsigned char data;
  asm volatile("in %1, %0":"=a"(data):"d"(port));
  return data;
}

static inline void stosb(void *addr, int data, int cnt){
  asm volatile("cld; rep stosb"
               :"=D" (addr), "=c"(cnt)
               :"0"(addr), "1"(cnt), "a"(data)
               :"memory","cc");
}

#endif // __OMO_ASM_X86_H__
