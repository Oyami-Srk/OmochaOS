#ifndef __OMOCHA_OS__TYPE_H__
#define __OMOCHA_OS__TYPE_H__

#include "const.h"

typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;

typedef unsigned int pte; // for hint
typedef unsigned int pde;

typedef struct {
  unsigned short LimitL;
  unsigned short BaseL;
  unsigned char BaseM;
  unsigned char Attr1;
  unsigned char LimitH_Attr2;
  unsigned char BaseH;
}__attribute__((packed)) Descriptor;

typedef struct {
  uint offset_low : 16;
  uint sel : 16;
  // attr
  uint some_thing_should_be_zero : 8;
  uint type : 4;
  uint s: 1; // system , be zero
  uint dpl: 2; // 描述符特权级
  uint p: 1; // present
  // attr end
  uint offset_high: 16;
}__attribute__((packed)) Gate;

typedef struct {
  ushort gs;
  ushort padding1;
  ushort fs;
  ushort padding2;
  ushort es;
  ushort padding3;
  ushort ds;
  ushort padding4;

  uint edi;
  uint esi;
  uint ebp;
  uint oesp;
  uint ebx;
  uint edx;
  uint ecx;
  uint eax;

  uint trapno; // push by vector_table
  uint err;

  uint eip;
  ushort cs;
  ushort padding5;
  uint eflags;
  uint esp;
  ushort ss;
  ushort padding6;

} stack_frame;

struct tss {
	uint	backlink;
	uint	esp0;		/* stack pointer to use during interrupt */
	uint	ss0;		/*   "   segment  "  "    "        "     */
	uint	esp1;
	uint	ss1;
	uint	esp2;
	uint	ss2;
	uint	cr3;
	uint	eip;
	uint	flags;
	uint	eax;
	uint	ecx;
	uint	edx;
	uint	ebx;
	uint	esp;
	uint	ebp;
	uint	esi;
	uint	edi;
	uint	es;
	uint	cs;
	uint	ss;
	uint	ds;
	uint	fs;
	uint	gs;
	uint	ldt;
	ushort	trap;
	ushort	iobase;	/* I/O位图基址大于或等于TSS段界限，就表示没有I/O许可位图 */
	/*char	iomap[2];*/
};

typedef struct {
  stack_frame stack;
  unsigned short selector_ldt;
  Descriptor ldts[LDT_SIZE];
  uint pid;
  char name[PROCESS_NAME_SIZE];

  char *kernel_stack;
} process;


typedef struct {
  uint cpu_id;
  uint interrupt_count;
  uint current_running_proc;

  Gate idt[IDT_COUNT];
  Descriptor gdt[GDT_COUNT];
  pde *pgdir;
  process processes[PROC_COUNT];
  struct tss tss;

  uint beats;
} cpu_env;


typedef char* va_list;

#endif
