#include "kernel/interrupt.h"
#include "kernel/memory.h"
#include "kernel/paging.h"
#include "kernel/pm.h"
#include "kernel/proc.h"
#include "kernel/type.h"
#include "lib/asm.h"
#include "lib/stdlib.h"

#include "driver/vga.h"

#include "syscall/syscall.h"

extern uint vector_table[];

process processes[3];

Descriptor gdt[128];
Gate idt[256];
struct tss tss;
uint interrupt_count = 1;
void *kernel_stack;
uint beats;

void TestA(void) {
  kprintf("Process A has started! ");
  message msg;
  msg.receiver = 2;
  msg.major_data = 2333;
  while (1) {
    send_msg(&msg);
    kprintf("<msg sent>");
  }
}

void TestB(void) {
  kprintf("Process B has started! ");
  message msg;
  while (1) {
    char buf[32];
    sprintf(buf, "<%d>", get_ticks());
    kprintf(buf);
    delay_ms(200);
    recv_msg(&msg, 1);
    sprintf(buf, "msg is %d ", msg.major_data);
    kprintf(buf);
  }
}

void SysIdle() {
  while (1)
    ;
}

int main(void) {
  beats = 0;
  kernel_stack = kalloc();
  kinit_mem(KERN_END, KP2V(4 * 1024 * 1024));
  kinit_gdt(gdt, 128, &tss, processes, 3);
  kinit_interrupt(idt, sizeof(idt));

  init_proc(&processes[0], 0, SysIdle);
  init_proc(&processes[1], 1, TestA);
  init_proc(&processes[2], 2, TestB);
  init_proctable(processes, 3);

  VGA_init();

  kprintf("Hello world!\nNice to meet you!\n");
  char buf[128];

  start_proc();
  magic_break();
}

/*
  一开始使用4M的内存页，节约开销和麻烦
 */
__attribute__((__aligned__(PG_SIZE))) unsigned int temp_page_dir[PDE_SIZE] = {
    [0] = (0) | PG_Present | PG_Writeable | PG_PS,
    [KERN_BASE >> 22] = (0) | PG_Present | PG_Writeable | PG_PS};
