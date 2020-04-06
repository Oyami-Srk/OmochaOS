#include "core/interrupt.h"
#include "core/environment.h"
#include "core/memory.h"
#include "core/paging.h"
#include "driver/graphic.h"
#include "generic/asm.h"
#include "lib/stdlib.h"
#include "lib/string.h"
#include "lib/syscall.h"

/* void *interrupt_stack; */

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

extern uint              vector_table[];
extern void *            syscall_table[];
struct interrupt_method *interrupt_methods;
uint *                   interrupt_suscribed;
uint *                   beats;
extern pde_t             core_page_dir[PDE_SIZE];

static inline void EOI_M(void) { outb(IO_PIC_M, 0x20); }
static inline void EOI_S(void) { outb(IO_PIC_S, 0x20); }

void send_interrupt_msg(uint irq, uint pid) {
    extern size_t proc_count;
    assert(pid < proc_count);
    extern process *proc_table;
    process *       p   = &proc_table[pid];
    message *       msg = (message *)vir2phy(p->page_dir, (char *)p->p_msg);
    if (p->status & PROC_STATUS_RECEVING) {
        if (msg->sender == PROC_ANY || msg->sender == PROC_INTERRUPT) {
            msg->sender = PROC_INTERRUPT;
            msg->type   = MSG_INTERRUPT;
            msg->major  = irq;
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

void core_init_interrupt(struct core_env *env) {
    for (uint i = 0; i < env->idt_size; i++) {
        if (i == SYSCALL_INT)
            make_gate(&env->idt[i], 1 << 3, (uint)vector_table[i], DPL3,
                      GATE_INT32);
        else
            make_gate(&env->idt[i], 1 << 3, (uint)vector_table[i], DPL0,
                      GATE_INT32);
    }
    uchar idt_ptr[6];
    u16 * idt_limit = (u16 *)(&idt_ptr[0]);
    u32 * idt_base  = (u32 *)(&idt_ptr[2]);
    *idt_limit      = env->idt_size * sizeof(Gate) - 1;
    *idt_base       = (u32)(env->idt);
    asm volatile("lidt (%0)" ::"r"(idt_ptr));
    init_8259A();
    /* interrupt_stack = kalloc(0); // notice here allocate */
    /* memset(interrupt_suscribed, 0, sizeof(interrupt_suscribed)); */
    /* memset(interrupt_methods, 0, */
    /* sizeof(struct interrupt_method) * HW_IRQ_COUNT); */
    interrupt_suscribed = env->interrupt_suscribed;
    interrupt_methods   = env->interrupt_methods;
    beats               = &env->beats; // TODO: not use pointer anyway
    asm("sti");
}

extern void     scheduler();
extern process *proc_running;

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
                exception_message[intf->trap_no], proc_running->pid);
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
        (*beats)++;
        scheduler();
        EOI_M();
        break;
    case SYSCALL_INT: {
        pde_t *original_pg_dir = NULL;
        BOOL   cr3changed      = FALSE;
        asm volatile("movl %%cr3, %0" : "=r"(original_pg_dir));
        if (original_pg_dir != KV2P(core_page_dir)) {
            asm volatile("movl %0, %%cr3\n\t" ::"r"(KV2P(core_page_dir)));
            cr3changed = TRUE;
        }
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
        if (cr3changed)
            asm volatile("movl %0, %%cr3" ::"r"(original_pg_dir));
        break;
    }
    default:
        break;
    }
}
