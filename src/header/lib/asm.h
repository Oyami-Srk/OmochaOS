#ifndef __ASM_H__
#define __ASM_H__

#define PUBLIC

PUBLIC static inline void stosb(void *addr, char data, int size) {
  asm volatile("cld; rep stosb"
               : "=D"(addr), "=c"(size)
               : "0"(addr), "1"(size), "a"(data)
               : "memory", "cc");
}

PUBLIC static inline void stosl(void *addr, char data, int size) {
  asm volatile("cld; rep stosl"
               : "=D"(addr), "=c"(size)
               : "0"(addr), "1"(size), "a"(data)
               : "memory", "cc");
}

PUBLIC static inline void outb(unsigned short port, unsigned char data) {
  asm volatile("out %0, %1" ::"a"(data), "d"(port));
}

PUBLIC static inline void outw(unsigned short port, unsigned short data) {
  asm volatile("out %0, %1" ::"a"(data), "d"(port));
}

PUBLIC static inline unsigned char inb(unsigned short port) {
  unsigned char r;
  asm volatile("in %1, %0" : "=a"(r) : "d"(port));
  return r;
}

PUBLIC static inline void sti() { asm volatile("sti"); }

PUBLIC static inline void cli() { asm volatile("cli"); }

PUBLIC static inline void insl(int port, void *addr, int cnt) {
  asm volatile("cld; rep insl"
               : "=D"(addr), "=c"(cnt)
               : "d"(port), "0"(addr), "1"(cnt)
               : "memory", "cc");
}

#endif // __ASM_H__
