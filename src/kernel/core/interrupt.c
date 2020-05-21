#include "core/interrupt.h"
#include "core/config.h"
#include "core/cpuid.h"
#include "core/environment.h"
#include "core/memory.h"
#include "core/paging.h"
#include "driver/graphic.h"
#include "generic/asm.h"
#include "lib/stdlib.h"
#include "lib/string.h"
#include "lib/syscall.h"

#ifdef USE_APIC
#include "core/apic.h"
#include "driver/hpet.h"
#elif USE_8259A
#include "core/8259A.h"
#endif

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
                                   "#XF: SIMD Floating-Point Exception"};

extern uint              vector_table[];
extern void *            syscall_table[];
struct interrupt_method *interrupt_methods;
uint *                   exception_suscribed;
struct interrupt_data *  exception_suscribed_data;
uint *                   interrupt_suscribed;
struct interrupt_data *  interrupt_suscribed_data;
uint *                   beats;
extern pde_t             core_page_dir[PDE_SIZE];

void send_interrupt_msg(uint irq, pid_t pid) {
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
    interrupt_suscribed_data[irq].avail = TRUE;
    interrupt_suscribed_data[irq].data  = irq;
}

void send_exception_msg(uint exception, uint data, pid_t pid) {
    extern size_t proc_count;
    assert(pid < proc_count);
    extern process *proc_table;
    process *       p   = &proc_table[pid];
    message *       msg = (message *)vir2phy(p->page_dir, (char *)p->p_msg);
    if (p->status & PROC_STATUS_RECEVING) {
        if (msg->sender == PROC_ANY || msg->sender == PROC_INTERRUPT) {
            msg->sender           = PROC_INTERRUPT;
            msg->type             = MSG_EXCEPTION;
            msg->major            = exception;
            msg->data.uint_arr.d1 = data;
            p->status &= ~PROC_STATUS_RECEVING;
            return;
        }
    }
    p->status |= PROC_STATUS_GOTEXC;
    exception_suscribed_data[exception].avail = TRUE;
    exception_suscribed_data[exception].data  = data;
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
    asm volatile("lidt (%0)\n\t" ::"r"(idt_ptr));
    init_inthw(env);
    /* interrupt_stack = kalloc(0); // notice here allocate */
    /* memset(interrupt_suscribed, 0, sizeof(interrupt_suscribed)); */
    /* memset(interrupt_methods, 0, */
    /* sizeof(struct interrupt_method) * HW_IRQ_COUNT); */
    interrupt_suscribed      = env->interrupt_suscribed;
    interrupt_methods        = env->interrupt_methods;
    exception_suscribed      = env->exception_suscribed;
    exception_suscribed_data = env->exception_suscribed_data;
    interrupt_suscribed_data = env->interrupt_suscribed_data;
    beats                    = &env->beats; // TODO: not use pointer anyway
    sti();
}

extern void     scheduler();
extern process *proc_running;

static char timer_str[] = {'/', '|', '\\', '-'};
int         timer_str_n = 0;

void interrupt_handler(int interrupt_count, stack_frame *intf) {
    if (intf->trap_no == IRQ_TIMER) {
        (*beats)++;
#if PRINT_CLOCK
        kputc_color_xy(0, 0, timer_str[timer_str_n++], RED, BLACK);
        if (timer_str_n >= sizeof(timer_str))
            timer_str_n = 0;
#endif
        end_interrupt(IRQ_TIMER - IRQ0);
        if (interrupt_count == 0)
            scheduler();
        return;
    }

    asm volatile("movl %%cr3, %%ebx\n\t"
                 "cmp %%ebx, %0\n\t"
                 "je .done\n\t"
                 "movl %0, %%cr3\n\t"
                 ".done:\n\t" ::"r"(KV2P(core_page_dir))
                 : "memory", "%ebx");
    if (intf->trap_no < EXCEPTION_COUNT) {
        if (exception_suscribed[intf->trap_no]) {
            ((process *)intf)->status &= PROC_STATUS_ERROR;
            uint trap_no = intf->trap_no;
            switch (trap_no) {
            case EXCEPTION_PF:
                asm volatile("movl %%cr2, %0\n\t" : "=r"(intf->trap_no));
                break;
            }
            send_exception_msg(trap_no, (uint)intf,
                               exception_suscribed[trap_no]);
            scheduler();
            /*
            kprintf("Exception %s with err_code %d in proc %d\n"
                    "Proc esp: 0x%x, eip: 0x%x (va)\n"
                    "Proc status: 0x%x  cr2: %x\n",
                    exception_message[trap_no], intf->err_code,
                    proc_running->pid, proc_running->stack.esp,
                    proc_running->stack.eip, proc_running->status,
                    intf->trap_no);
            magic_break();
            */
            return;
        }
        cli();
        kprintfc(RED, BLUE,
                 "Exception %s with err_code %d in proc %d\n"
                 "Proc esp: 0x%x, eip: 0x%x (va)\n"
                 "Proc status: 0x%x\n",
                 exception_message[intf->trap_no], intf->err_code,
                 proc_running->pid, proc_running->stack.esp,
                 proc_running->stack.eip, proc_running->status);
        magic_break();
        while (1)
            ;
    }
    if (intf->trap_no <=
        IRQ0 + HW_IRQ_COUNT /*&& intf->trap_no != IRQ_TIMER*/) {
        kprintf("[INT %d]", intf->trap_no);
        // test hpet
        u32 counter_hi, counter_lo;
        get_hpet_reg(0xFED00000, 0xF0, &counter_lo, &counter_hi);
        kprintf("HPET Counter test: 0x%x 0x%x\n", counter_hi, counter_lo);
        // handler interrupt and enable irq
        if (interrupt_methods[intf->trap_no - IRQ0].avail ==
            TRUE) { // better not to use this way
            disable_interrupt(intf->trap_no - IRQ0);
            void *       func = interrupt_methods[intf->trap_no - IRQ0].func;
            typedef void fp_v_v(void *);
            ((fp_v_v *)func)(interrupt_methods[intf->trap_no - IRQ0].data);
            enable_interrupt(intf->trap_no - IRQ0);
        }
        if (interrupt_suscribed[intf->trap_no - IRQ0]) {
            send_interrupt_msg(intf->trap_no - IRQ0,
                               interrupt_suscribed[intf->trap_no - IRQ0]);
        }
        end_interrupt(intf->trap_no - IRQ0); // maybe it could move to the top?
        return;
    }
    switch (intf->trap_no) {
    case SYSCALL_INT: {
        /*
        asm volatile("movl %%cr3, %%ebx\n\t"
                     "cmp %%ebx, %0\n\t"
                     "je .done\n\t"
                     "movl %0, %%cr3\n\t"
                     ".done:\n\t" ::"r"(KV2P(core_page_dir))
                     : "memory", "%ebx");
                     */
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
