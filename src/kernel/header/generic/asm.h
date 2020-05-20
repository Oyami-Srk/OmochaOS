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
    asm volatile("outb %0, %1" ::"a"(data), "d"(port));
}

static inline void outw(unsigned short port, unsigned short data) {
    asm volatile("outw %0, %1" ::"a"(data), "d"(port));
}

static inline void outl(unsigned short port, unsigned int data) {
    asm volatile("outl %0, %1" ::"a"(data), "d"(port));
}

static inline unsigned char inb(unsigned short port) {
    unsigned char r;
    asm volatile("inb %1, %0" : "=a"(r) : "d"(port));
    return r;
}

static inline unsigned short inw(unsigned short port) {
    unsigned short r;
    asm volatile("inw %1, %0" : "=a"(r) : "d"(port));
    return r;
}

static inline unsigned int inl(unsigned short port) {
    unsigned int r;
    asm volatile("inl %1, %0" : "=a"(r) : "d"(port));
    return r;
}

#if USE_JMP_IOWAIT
static inline void io_wait(void) {
    asm volatile("jmp 1f\n\t"
                 "1:jmp 2f\n\t"
                 "2:");
}
#else
static inline void io_wait(void) {
    /* Port 0x80 is used for 'checkpoints' during POST. */
    /* The Linux kernel seems to think it is free for use :-/ */
    asm volatile("outb %%al, $0x80" : : "a"(0));
    /* %%al instead of %0 makes no difference.  TODO: does the register need to
     * be zeroed? */
}
#endif

static inline void mfence() { asm volatile("mfence" ::: "memory"); }
static inline void lfence() { asm volatile("lfence" ::: "memory"); }
static inline void sfence() { asm volatile("sfence" ::: "memory"); }

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

static inline void outsb(int port, const void *addr, int cnt) {
    asm volatile("cld; rep outsb"
                 : "=S"(addr), "=c"(cnt)
                 : "d"(port), "0"(addr), "1"(cnt)
                 : "cc");
}

static inline void outsw(int port, const void *addr, int cnt) {
    asm volatile("cld; rep outsw"
                 : "=S"(addr), "=c"(cnt)
                 : "d"(port), "0"(addr), "1"(cnt)
                 : "cc");
}

#ifdef __DEBUG__
static inline void magic_break(void) { asm volatile("xchgw %bx, %bx"); }
#else
static inline void magic_break(void) { asm volatile("nop"); }
#endif

#endif // __ASM_H__
