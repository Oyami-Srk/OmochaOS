#include "core/interrupt.h"
#include "core/memory.h"
#include "driver/graphic.h"
#include "generic/asm.h"
#include "lib/stdlib.h"
#include "lib/string.h"

const char *exception_message[] = {"#DE: Divide-by-zero Error",
                                   "#DB: Debug",
                                   "#--: Non-maskable Interrupt",
                                   "#BP: Break Point",
                                   "#OF: Overflow",
                                   "#BR: Bound Range Exceeded",
                                   "#UD: Invalid Opcode",
                                   "#NM: Device Not Avaliable",
                                   "#DF: Double Fault",
                                   "#RESV: Reserved",
                                   "#TS: Invalid TSS",
                                   "#NP: Segment Not Present",
                                   "#SS: Stack-Segment Fault",
                                   "#GP: General Protection Fault",
                                   "#PF: Page Fault",
                                   "#RESV: Reserved2",
                                   "#MF: x87 Floating-Point Exception",
                                   "#AC: Alignment Check",
                                   "#MC: Machine Check",
                                   "#XF: SIMD Floating-Point Exception"

};

extern uint vector_table[];

static inline void EOI_M(void) { outb(IO_PIC_M, 0x20); }
static inline void EOI_S(void) { outb(IO_PIC_S, 0x20); }

void init_8259A() {
    outb(IO_PIC_M, 0x11);     // ICW 1
    outb(IO_PIC_S, 0x11);     // ICW 1
    outb(IO_PIC_M + 1, 0x20); // 0x20 -> Master first
    outb(IO_PIC_S + 1, 0x28); // 0x28 -> Salve first

    outb(IO_PIC_M + 1, 0x4); // ICW 3
    outb(IO_PIC_S + 1, 0x2); // ICW 3
    outb(IO_PIC_M + 1, 0x1);
    outb(IO_PIC_S + 1, 0x1);

    outb(IO_PIC_M + 1, 0xFE);
    outb(IO_PIC_S + 1, 0xFF);
}

void enable_irq(uint irq) {
    if (irq < 8)
        outb(IO_PIC_M + 1, inb(IO_PIC_M) & ~(1 << irq));
    else
        outb(IO_PIC_S + 1, inb(IO_PIC_S) & ~(1 << irq));
}

void disable_irq(uint irq) {
    if (irq < 8)
        outb(IO_PIC_M + 1, inb(IO_PIC_M) | (1 << irq));
    else
        outb(IO_PIC_S + 1, inb(IO_PIC_S) | (1 << irq));
}

void core_init_interrupt(Gate *idt, size_t count) {
    if (count < IVT_COUNT) {
        magic_break();
        while (1)
            ; // panic
    }
    for (uint i = 0; i < IVT_COUNT; i++) {
        make_gate(&idt[i], 1 << 3, (uint)vector_table[i], DPL0, GATE_INT32);
    }
    uchar idt_ptr[6];
    u16 * idt_limit = (u16 *)(&idt_ptr[0]);
    u32 * idt_base  = (u32 *)(&idt_ptr[2]);
    *idt_limit      = count * sizeof(Gate) - 1;
    *idt_base       = (u32)KV2P(idt);
    asm volatile("lidt (%0)" ::"r"(idt_ptr));
    init_8259A();
    asm("sti");
}

extern uint beats;

void interrupt_handler(stack_frame *intf) {
    if (intf->trap_no <= 19) {
        cli();
        GRAPHIC_write_color_string_to_vm(0, COLOR(BLUE, BLACK),
                                         "                    "
                                         "                    "
                                         "                    "
                                         "                   ");
        char buf[128];
        sprintf(buf, "Exception %s in proc %d",
                exception_message[intf->trap_no], 0);
        GRAPHIC_write_color_string_to_vm(0, COLOR(BLUE, RED), buf);
        magic_break();
        while (1)
            ;
    }
    if (intf->trap_no <= IRQ0 + HW_IRQ_COUNT && intf->trap_no > IRQ_TIMER) {
    }
    switch (intf->trap_no) {
    case IRQ_TIMER:
        beats++;
        EOI_M();
        break;
    default:
        break;
    }
}
