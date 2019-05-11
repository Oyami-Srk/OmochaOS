#ifndef __ASM_H__
#define __ASM_H__

static inline void stosb(void *addr, char data, int size) {
  asm volatile("cld; rep stosb"
               : "=D"(addr), "=c"(size)
               : "0"(addr), "1"(size), "a"(data)
               : "memory", "cc");
}

static inline void stosl(void *addr, char data, int size) {
  asm volatile("cld; rep stosl"
               : "=D"(addr), "=c"(size)
               : "0"(addr), "1"(size), "a"(data)
               : "memory", "cc");
}

static inline void outb(unsigned short port, unsigned char data) {
  asm volatile("out %0, %1" ::"a"(data), "d"(port));
}

static inline void outw(unsigned short port, unsigned short data) {
  asm volatile("out %0, %1" ::"a"(data), "d"(port));
}

static inline unsigned char inb(unsigned short port) {
  unsigned char r;
  asm volatile("in %1, %0" : "=a"(r) : "d"(port));
  return r;
}

static inline void sti() { asm volatile("sti"); }

static inline void cli() { asm volatile("cli"); }

static inline void insl(int port, void *addr, int cnt) {
  asm volatile("cld; rep insl"
               : "=D"(addr), "=c"(cnt)
               : "d"(port), "0"(addr), "1"(cnt)
               : "memory", "cc");
}

static inline void outsl(int port, const void *addr, int cnt) {
  asm volatile("cld; rep outsl"
               : "=S"(addr), "=c"(cnt)
               : "d"(port), "0"(addr), "1"(cnt)
               : "cc");
}

#ifndef __NON_DEBUG
static inline void magic_break(void) { asm volatile("xchgw %bx, %bx"); }
#else
static inline void magic_break(void) { asm volatile("nop"); }
#endif

#endif // __ASM_H__
