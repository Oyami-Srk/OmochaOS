#include "config.h"
#include "paging.h"
#include "memlayout.h"
#include "protect.h"

typedef struct {
  unsigned int BaseL;
  unsigned int BaseH;
  unsigned int LengthL;
  unsigned int LengthH;
  unsigned int Type;
  unsigned int ACPI;
}__attribute__((packed)) SMAP_entry;

void make_desc(unsigned int ID, unsigned int Base, unsigned int Limit, unsigned short Attr){
  GDT_Descriptor *pDesc = KERN_GDT_BASE + ID * sizeof(GDT_Descriptor);
  pDesc->LimitL = Limit & 0xFFFF;
  pDesc->BaseL = Base & 0xFFFF;
  pDesc->BaseM = (Base >> 16) & 0x0FF;
  pDesc->BaseH = (Base >> 24) & 0x0FF;
  pDesc->Attr1 = Attr & 0xFF;
  pDesc->LimitH_Attr2 = ((Limit >> 16) & 0x0F) | ((Attr >> 8) & 0xF0);
}


unsigned char gdt_ptr[6];

void write_string( int colour, const char *string )
{
  volatile char *video = (volatile char*)0xB8000;
  while( *string != 0 )
    {
      *video++ = *string++;
      *video++ = colour;
    }
}

int main(void){
  unsigned int *mem_infos_count = BOOT_LOADER_MEM_INFO_COUNT;
  SMAP_entry *mem_infos = BOOT_LOADER_MEM_INFO;

  make_desc(0, 0, 0, 0); // Dummy Null Desc
  make_desc(1, 0, 0x80F00000, DA_32 | DA_4K | DA_C); // Code Desc
  make_desc(2, 0, 0x80F00000, DA_32 | DA_4K | DA_DRW); // Data Desc
  make_desc(3, 0xB8000, 0xFFFF, DA_DRW | DA_DPL3); // Video Desc

  unsigned short *gdt_limit = (unsigned short *)(&gdt_ptr[0]);
  unsigned int *gdt_base = (unsigned int *)(&gdt_ptr[2]);
  *gdt_limit = 4 * sizeof(GDT_Descriptor) - 1;
  *gdt_base = (unsigned int) KERN_GDT_BASE;

  __asm__ __volatile__("lgdtl (gdt_ptr)");

  *((int*)0xb8000)=0x07690748;

  write_string(0x0F, "Hello world!");

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

