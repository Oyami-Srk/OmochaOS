#include "core/config.h"

#if USE_8259A
#include "core/8259A.h"
#include "driver/misc.h"
#include "generic/asm.h"

void init_8259A(struct core_env *env) {
    outb(IO_PIC_M, 0x11); // ICW 1
    io_wait();
    outb(IO_PIC_S, 0x11); // ICW 1
    io_wait();
    outb(IO_PIC_M + 1, 0x20); // 0x20 -> Master first
    io_wait();
    outb(IO_PIC_S + 1, 0x28); // 0x28 -> Salve first
    io_wait();

    outb(IO_PIC_M + 1, 0x4); // ICW 3
    io_wait();
    outb(IO_PIC_S + 1, 0x2); // ICW 3
    io_wait();
    outb(IO_PIC_M + 1, 0x1);
    io_wait();
    outb(IO_PIC_S + 1, 0x1);
    io_wait();

    outb(IO_PIC_M + 1, 0x00);
    outb(IO_PIC_S + 1, 0x00);

    init_8253();
}

void enable_interrupt(uint irq) {
    if (irq < 8)
        outb(IO_PIC_M + 1, inb(IO_PIC_M) & ~(1 << irq));
    else
        outb(IO_PIC_S + 1, inb(IO_PIC_S) & ~(1 << irq));
}

void disable_interrupt(uint irq) {
    if (irq < 8)
        outb(IO_PIC_M + 1, inb(IO_PIC_M) | (1 << irq));
    else
        outb(IO_PIC_S + 1, inb(IO_PIC_S) | (1 << irq));
}

static inline void EOI_M(void) { outb(IO_PIC_M, 0x20); }
static inline void EOI_S(void) { outb(IO_PIC_S, 0x20); }
void               end_interrupt(uint i) {
    if (i < 8)
        EOI_M();
    else {
        EOI_M();
        EOI_S();
    }
}

#endif