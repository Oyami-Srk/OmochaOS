#include "config.h"
#include "paging.h"
#include "memlayout.h"
#include "protect.h"
#include "klib.h"
#include "interrupt.h"


void make_desc(unsigned int ID, unsigned int Base, unsigned int Limit, unsigned short Attr){
  MAKE_DESC((GDT_Descriptor*)KERN_GDT_BASE + ID * sizeof(GDT_Descriptor),
            Base, Limit, Attr);
}

unsigned char gdt_ptr[6];
unsigned char idt_ptr[6];
extern uint vector_table[];

int main(void){
  unsigned int *mem_infos_count = (unsigned int*)BOOT_LOADER_MEM_INFO_COUNT;
  SMAP_entry *mem_infos = (SMAP_entry*)BOOT_LOADER_MEM_INFO;

  clr_scr();

  /* kinit_paging(*mem_infos_count, mem_infos); */

  make_desc(0, 0, 0, 0); // Dummy Null Desc
  make_desc(1, 0, KERN_END, DA_32 | DA_4K | DA_C); // Code Desc
  make_desc(2, 0, KERN_END, DA_32 | DA_4K | DA_DRW); // Data Desc
  make_desc(3, 0xB8000, 0xFFFF, DA_DRW | DA_DPL3); // Video Desc

  unsigned short *gdt_limit = (unsigned short *)(&gdt_ptr[0]);
  unsigned int *gdt_base = (unsigned int *)(&gdt_ptr[2]);
  *gdt_limit = 4 * sizeof(GDT_Descriptor) - 1;
  *gdt_base = (unsigned int) KERN_GDT_BASE;

  __asm__ __volatile__("lgdtl (gdt_ptr)");

  Gate_Descriptor *idt = (Gate_Descriptor*)KERN_IDT_BASE;
  char b[32];

  for(int i = 0; i < IDT_COUNT; i++){
    MAKE_GATE(&idt[i], 1 << 3, vector_table[i], 0,GATE_INT32);
  }

  unsigned short *idt_limit = (unsigned short*)(&idt_ptr[0]);
  unsigned int *idt_base = (unsigned int*)(&idt_ptr[2]);
  *idt_limit = IDT_COUNT * sizeof(Gate_Descriptor) - 1;
  *idt_base = (unsigned int) KERN_IDT_BASE;

  __asm__ __volatile__("lidt (idt_ptr)");

  init_8259A();

  write_string(0x0F, "Hello world!\n");
  write_string(0x0F, "We got memory info like: \n");
  char buf[16];

  SMAP_entry *tmp = mem_infos;
  for(;mem_infos < tmp + (*mem_infos_count);){
    itoa(mem_infos->BaseH << 16 | mem_infos->BaseL, buf, 16);
    write_string(0x0F, "Base addr: ");
    write_string(0x0F, buf);
    write_string(0x0F, "  Length: ");
    itoa(mem_infos->LengthH << 16 | mem_infos->LengthL , buf, 16);
    write_string(0x0F, buf);
    write_string(0x0F, "  Type: ");
    switch(mem_infos->Type){
    case 1:
      write_string(0x0F, "USABLE\n");
      break;
    case 2:
      write_string(0x0F, "RESERVED\n");
      break;
    case 3:
      write_string(0x0F, "ACPI RECLAM\n");
      break;
    case 4:
      write_string(0x0F, "ACPI NVS\n");
      break;
    case 5:
      write_string(0x0F, "Bad\n");
      break;
    defult:
      write_string(0x0F, "Unrecognized\n");
      break;
    }
    mem_infos++;
  }


  __asm__ __volatile__ ("int $0x10");

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

