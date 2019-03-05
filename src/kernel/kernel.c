#include "config.h"
#include "kmem.h"
#include "paging.h"
#include "memlayout.h"
#include "protect.h"
#include "klib.h"
#include "interrupt.h"
#include "kmem.h"

#include "syscall.h"
#include "process.h"

cpu_env cpu;
char *task_stack[PROC_COUNT];

void TestA();
void TestB();

uint tasks[PROC_COUNT];
char *kernel_stack;

void load_process_context(){
  kprintf("Initialize proc table");
  for(int i = 0; i < PROC_COUNT; i++){
    process *p = &cpu.processes[i];
    p->selector_ldt = SEL_LDT1 + (i << 3);
    MAKE_DESC(&p->ldts[0], 0x0, 0xFFFFFFFF, DA_32 | DA_4K | DA_C);
    p->ldts[0].Attr1 = DA_C | (1 << 5);
    MAKE_DESC(&p->ldts[1], 0x0, 0xFFFFFFFF, DA_32 | DA_4K | DA_DRW);
    p->ldts[1].Attr1 = DA_DRW | (1 << 5);

    p->stack.cs = (0 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | SA_RPL1;
    p->stack.ds = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | SA_RPL1;
    p->stack.es = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | SA_RPL1;
    p->stack.fs = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | SA_RPL1;
    p->stack.ss = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | SA_RPL1;
    p->stack.gs = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | SA_RPL1;
    p->stack.eip = tasks[i];
    p->stack.esp = (uint)kalloc() + 4096;
    p->stack.eflags = 0x1202;

    p->status = 0;
  }
}

void delay_ms(uint ms){
  uint k = get_ticks();
  uint t = ms / 10;
  while((k + t) > get_ticks());
}

void TestA(){
  int i = 0;
  message msg;
  msg.receiver = 2;
  msg.major_data = 2333;
  volatile int r = 10010;
  while(1){
    kprintf("A ");
    kprintf(":%x:", &cpu.processes[0]);
    send_msg(&msg);
    delay_ms(500);
    i++;
  }
}

void TestB(){
  int i = 0;
  message msg;
  while(1){
    kprintf("B ");
    recv_msg(&msg, ANY);
    kprintf("msg from %d, data is %d\n", msg.sender, msg.major_data);
    delay_ms(500);
    i++;
  }
}

void TestC(){
  while(1){
    kprintf("C ");
    delay_ms(500);
  }
}


extern void vector_handler_ret();

int main(void){
  cpu.cpu_id = 0;
  cpu.interrupt_count = (uint)1;
  cpu.beats = 0;


  unsigned int *mem_infos_count = (unsigned int*)BOOT_LOADER_MEM_INFO_COUNT;
  SMAP_entry *mem_infos = (SMAP_entry*)BOOT_LOADER_MEM_INFO;
  clr_scr();
  kprintf(" <- bumping char means clock^-^ Hello world!\n");

  kinit_gdt(&cpu);
  kinit_interrupt(&cpu);
  kinit_mem(KERN_END, P2V(4 * 1024 * 1024));
  /* kinit_paging(*mem_infos_count, mem_infos, &cpu); */

  kernel_stack = kalloc();
  kprintf("Kern stack addr is 0x%08x\n",kernel_stack);

  kprintf("Kern end addr is 0x%8x\n", KERN_END);
  kprintf("Addr of esp0 is %x\n", &cpu.tss.esp0);

  tasks[0] = (uint)TestA;
  tasks[1] = (uint)TestB;
  tasks[2] = (uint)TestC;
  load_process_context();
  cpu.processes[0].status = PROC_STATUS_NORMAL | PROC_STATUS_RUNNING;
  cpu.processes[1].status = PROC_STATUS_NORMAL | PROC_STATUS_RUNNING;
  cpu.processes[2].status = PROC_STATUS_NORMAL | PROC_STATUS_RUNNING;
  kprintf("\nReady to jump ring 3...\n");

  message msg;

  cpu.current_running_proc = (volatile uint)&cpu.processes[0];
  __asm__ __volatile__("jmp %0"
                       :
                       :"r"(vector_handler_ret));


  while(1);
}

/*
  一开始使用4M的内存页，节约开销和麻烦
 */
__attribute__((__aligned__(PG_SIZE)))
unsigned int temp_page_dir[PDE_SIZE] = {
                           [0] = (0) | PG_P | PG_W | PG_PS,
                           [KERN_BASE >> 22] = (0) | PG_P | PG_W | PG_PS
};

