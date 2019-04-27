#include "kernel/interrupt.h"
#include "kernel/memory.h"
#include "kernel/paging.h"
#include "kernel/pm.h"
#include "kernel/type.h"
#include "lib/asm.h"

#include "driver/vga.h"

extern uint vector_table[];

Descriptor gdt[128];
Gate idt[256];
struct tss tss;
int main(void) {
  kinit_mem(KERN_END, KP2V(4 * 1024 * 1024));
  kinit_gdt(gdt, &tss, 0);
  kinit_interrupt(idt, sizeof(idt));

  VGA_init();
  VGA_write_color_string_to_vm(0, COLOR(BLACK, WHITE),
                               "Hello world!\nNice to meet you!");
  while (1)
    ;
}

/*
  一开始使用4M的内存页，节约开销和麻烦
 */
__attribute__((__aligned__(PG_SIZE))) unsigned int temp_page_dir[PDE_SIZE] = {
    [0] = (0) | PG_Present | PG_Writeable | PG_PS,
    [KERN_BASE >> 22] = (0) | PG_Present | PG_Writeable | PG_PS};
