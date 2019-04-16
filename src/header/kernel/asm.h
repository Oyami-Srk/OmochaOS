#ifndef __OMOCHA_OS__ASM_H__
#define __OMOCHA_OS__ASM_H__

#define PUBLIC

PUBLIC static inline void stosb(void *addr, char data, int size) {
  __asm__ __volatile__("cld; rep stosb"
                       :"=D"(addr), "=c"(size)
                       : "0"(addr), "1"(size), "a" (data)
                       :"memory", "cc");
}

PUBLIC static inline void stosl(void *addr, char data, int size) {
  __asm__ __volatile__("cld; rep stosl"
                       :"=D"(addr), "=c"(size)
                       : "0"(addr), "1"(size), "a" (data)
                       :"memory", "cc");
}

PUBLIC static inline void outb(unsigned short port, unsigned char data) {
  __asm__ __volatile__("out %0, %1" :: "a"(data), "d"(port));
}

PUBLIC static inline void outw(unsigned short port, unsigned short data) {
  __asm__ __volatile__("out %0, %1" :: "a"(data), "d"(port));
}

PUBLIC static inline unsigned char inb(unsigned short port) {
  unsigned char r;
  __asm__ __volatile__("in %1, %0": "=a"(r) : "d"(port));
  return r;
}

PUBLIC static inline void sti() {
  __asm__ __volatile__("sti");
}

PUBLIC static inline void cli() {
  __asm__ __volatile__("cli");
}

PUBLIC static inline void insl(int port, void *addr, int cnt){
  __asm__ __volatile__("cld; rep insl":
                       "=D"(addr), "=c"(cnt):
                       "d"(port), "0"(addr), "1"(cnt):
                       "memory", "cc");
}

#endif
