#include "core/interrupt.h"
#include "core/memory.h"
#include "driver/graphic.h"
#include "generic/asm.h"
#include "lib/stdlib.h"
#include "lib/string.h"
#include "lib/syscall.h"

void *interrupt_stack;

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

extern uint             vector_table[];
extern void *           syscall_table[];
struct interrupt_method interrupt_methods[HW_IRQ_COUNT];
uint                    interrupt_suscribed[HW_IRQ_COUNT];

static inline void EOI_M(void) { outb(IO_PIC_M, 0x20); }
static inline void EOI_S(void) { outb(IO_PIC_S, 0x20); }

void send_interrupt_msg(uint irq, uint pid) {
    extern size_t proc_count;
    assert(pid < proc_count);
    extern process *proc_table;
    process *       p = &proc_table[pid];
    if (p->status & PROC_STATUS_RECEVING) {
        if (p->p_msg->sender == PROC_ANY ||
            p->p_msg->sender == PROC_INTERRUPT) {
            p->p_msg->sender = PROC_INTERRUPT;
            p->p_msg->type   = MSG_INTERRUPT;
            p->p_msg->major  = irq;
            p->status &= ~PROC_STATUS_RECEVING;
            return;
        }
    }
    p->status |= PROC_STATUS_GOTINT;
}

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
        if (i == SYSCALL_INT)
            make_gate(&idt[i], 1 << 3, (uint)vector_table[i], DPL3, GATE_INT32);
        else
            make_gate(&idt[i], 1 << 3, (uint)vector_table[i], DPL0, GATE_INT32);
    }
    uchar idt_ptr[6];
    u16 * idt_limit = (u16 *)(&idt_ptr[0]);
    u32 * idt_base  = (u32 *)(&idt_ptr[2]);
    *idt_limit      = count * sizeof(Gate) - 1;
    *idt_base       = (u32)KV2P(idt);
    asm volatile("lidt (%0)" ::"r"(idt_ptr));
    init_8259A();
    interrupt_stack = kalloc();
    memset(interrupt_suscribed, 0, sizeof(interrupt_suscribed));
    memset(interrupt_methods, 0,
           sizeof(struct interrupt_method) * HW_IRQ_COUNT);
    asm("sti");
}

extern uint beats;
extern void scheduler();

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
        // handler interrupt and enable irq
        if (interrupt_methods[intf->trap_no - IRQ0].avail == TRUE) {
            disable_irq(intf->trap_no - IRQ0);
            void *       func = interrupt_methods[intf->trap_no - IRQ0].func;
            typedef void fp_v_v(void);
            ((fp_v_v *)func)();
        }
        if (interrupt_suscribed[intf->trap_no - IRQ0])
            send_interrupt_msg(intf->trap_no - IRQ0,
                               interrupt_suscribed[intf->trap_no - IRQ0]);
        if (intf->trap_no - IRQ0 < 8)
            EOI_M();
        else {
            EOI_M();
            EOI_S();
        }
    }
    switch (intf->trap_no) {
    case IRQ_TIMER:
        beats++;
        scheduler();
        EOI_M();
        break;
    case SYSCALL_INT: {
        volatile int retval = 0;
        asm volatile("push %%edx\n\t"
                     "push %%ebx\n\t"
                     "push %%ecx\n\t"
                     "push %%eax\n\t"
                     "call *%1\n\t"
                     "movl %%eax, %0\n\t"
                     "add $16, %%esp\n\t"
                     : "=r"(retval)
                     : "r"(syscall_table[intf->eax]), "c"(intf->ecx),
                       "b"(intf->ebx), "d"(intf->edx), "a"(intf)
                     : "memory");
        break;
    }
    default:
        break;
    }
}
