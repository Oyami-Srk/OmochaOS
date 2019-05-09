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

process processes[2];

Descriptor gdt[128];
Gate idt[256];
struct tss tss;
uint interrupt_count = 1;
void *kernel_stack;
uint beats;

uint disp_pos = 0;
#define kwrite_str(str)                                                        \
  disp_pos = VGA_write_color_string_to_vm(disp_pos, COLOR(BLACK, WHITE), str)

void TestA(void) {
  kwrite_str("Process A has started! ");
  while (1)
    ;
}

void TestB(void) {
  kwrite_str("Process B has started! ");
  while (1) {
    char buf[32];
    sprintf(buf, "<%d>", get_ticks());
    kwrite_str(buf);
    delay_ms(200);
  }
}

int main(void) {
  beats = 0;
  kernel_stack = kalloc();
  kinit_mem(KERN_END, KP2V(4 * 1024 * 1024));
  kinit_gdt(gdt, 128, &tss, processes, 2);
  kinit_interrupt(idt, sizeof(idt));

  init_proc(&processes[0], 1, TestA);
  init_proc(&processes[1], 2, TestB);
  init_proctable(processes, 2);

  VGA_init();

  kwrite_str("Hello world!\nNice to meet you!\n");
  char buf[128];
  sprintf(buf, "Sizeof process is %d\n", sizeof(process));
  kwrite_str(buf);

  start_proc();
  magic_break();
}

/*
  一开始使用4M的内存页，节约开销和麻烦
 */
__attribute__((__aligned__(PG_SIZE))) unsigned int temp_page_dir[PDE_SIZE] = {
    [0] = (0) | PG_Present | PG_Writeable | PG_PS,
    [KERN_BASE >> 22] = (0) | PG_Present | PG_Writeable | PG_PS};
