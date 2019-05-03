#include "kernel/interrupt.h"
#include "kernel/memory.h"
#include "kernel/paging.h"
#include "kernel/pm.h"
#include "kernel/proc.h"
#include "kernel/type.h"
#include "lib/asm.h"
#include "lib/stdlib.h"

#include "driver/vga.h"

extern uint vector_table[];

process processes[1];

Descriptor gdt[128];
Gate idt[256];
struct tss tss;
uint interrupt_count = 1;
void *kernel_stack;

uint disp_pos = 0;
#define kwrite_str(str)                                                        \
  disp_pos = VGA_write_color_string_to_vm(disp_pos, COLOR(BLACK, WHITE), str)

void TestA(void) {
  kwrite_str("Process A has started! ");
  while (1)
    ;
}

int main(void) {
  kernel_stack = kalloc();
  kinit_mem(KERN_END, KP2V(4 * 1024 * 1024));
  kinit_gdt(gdt, 128, &tss, processes, 1);
  kinit_interrupt(idt, sizeof(idt));

  init_proc(processes, 1, TestA);

  VGA_init();
  kwrite_str("Hello world!\nNice to meet you!\n");

  start_proc();
  magic_break();
  while (1)
    ; // never reach here
}

/*
  一开始使用4M的内存页，节约开销和麻烦
 */
__attribute__((__aligned__(PG_SIZE))) unsigned int temp_page_dir[PDE_SIZE] = {
    [0] = (0) | PG_Present | PG_Writeable | PG_PS,
    [KERN_BASE >> 22] = (0) | PG_Present | PG_Writeable | PG_PS};
