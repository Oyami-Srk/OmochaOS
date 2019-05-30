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

#include "modules/hd.h"
#include "modules/modules.h"

extern uint vector_table[];

process processes[4 + __MODULES_COUNT__];
uint modules[__MODULES_COUNT__] = __MODULES_DEFINES__;

Descriptor gdt[128];
Gate idt[256];
struct tss tss;
uint interrupt_count = 1;
void *kernel_stack;
uint beats;

void TestA(void) {
  kprintf("Process A has started! ");
  message msg;
  msg.receiver = 3;
  msg.major_data = 2333;
  while (1) {
    ;
  }
}

void TestB(void) {
  kprintf("Process B has started! ");
  message msg;
  delay_ms(200);
  while (1) {
    ;
  }
}
extern void SysIdle();
extern void SysTask();
int main(void) {
  beats = 0;
  kernel_stack = kalloc();
  kinit_mem(KERN_END, KP2V(4 * 1024 * 1024));
  kinit_gdt(gdt, 128, &tss, processes,
            sizeof(processes) / sizeof(processes[0]));
  kinit_interrupt(idt, sizeof(idt));

  init_proc(&processes[0], 0, SysIdle);
  init_proc(&processes[1], 1, SysTask);
  init_proc(&processes[2], 2, TestA);
  init_proc(&processes[3], 3, TestB);
  for (uint i = 0; i < __MODULES_COUNT__; i++)
    init_proc(&processes[4 + i], 4 + i, (fp_v_v)modules[i]);
  init_proctable(processes, sizeof(processes) / sizeof(processes[0]));

  VGA_init();

  kprintf("^-^\nLoad complete! Totally got %d processes.\n",
          sizeof(processes) / sizeof(processes[0]));
  kprintf("Hello world!\nNice to meet you!\n");

  start_proc();
  magic_break();
}

/*
  一开始使用4M的内存页，节约开销和麻烦
 */
__attribute__((__aligned__(PG_SIZE))) unsigned int temp_page_dir[PDE_SIZE] = {
    [0] = (0) | PG_Present | PG_Writeable | PG_PS,
    [KERN_BASE >> 22] = (0) | PG_Present | PG_Writeable | PG_PS};
