  ; Kernel entry point

  MAGIC_NUMBER equ 0x1BADB002
  FLAGS equ 0
  CHECKSUM equ -(MAGIC_NUMBER - FLAGS)
  ; kernel_base是虚拟地址，实际上执行代码的加载地址是 kernel.ld里定义的
  ; .text: AT(0x100000) 上，但是链接地址是0x80100000
  ; 所以为了让_start正确指向没经过分页的物理地址，就用ld认为的
  ; start地址（虚拟0x80100000）减去基底0x80000000.
  KERNEL_BASE equ 0x80000000

[SECTION .multboot]
ALIGN 4
  dd MAGIC_NUMBER
  dd FLAGS
  dd CHECKSUM

[SECTION .bss]
ALIGN 16
stack_bottom:
  resb 8096                     ; 8Kb
stack_top:

[SECTION .text]
global _start                   ; load to 0x80000000 + RA
extern main                     ; link to kernel.c
extern temp_page_dir

_start equ start - KERNEL_BASE

global start
start:
  ; jmp $                         ; debug 目的
  mov eax, cr4
  or eax, 0x10                  ; extend page size to 4 megabyte
  mov cr4, eax

  mov eax, temp_page_dir - KERNEL_BASE ; temp_page_dir in kernel.c
  mov cr3, eax

  mov eax, cr0
  or eax, 0x80000000            ; enable paging
  mov cr0, eax

  mov esp, stack_top

  mov eax, main
  jmp eax                       ; jump to kernel.c -> main

  P_STACKBASE	equ	0
  GSREG		equ	P_STACKBASE
  FSREG		equ	GSREG		+ 4
  ESREG		equ	FSREG		+ 4
  DSREG		equ	ESREG		+ 4
  EDIREG		equ	DSREG		+ 4
  ESIREG		equ	EDIREG		+ 4
  EBPREG		equ	ESIREG		+ 4
  KERNELESPREG	equ	EBPREG		+ 4
  EBXREG		equ	KERNELESPREG	+ 4
  EDXREG		equ	EBXREG		+ 4
  ECXREG		equ	EDXREG		+ 4
  EAXREG		equ	ECXREG		+ 4
  TRAPNO		equ	EAXREG		+ 4
  ERRNO     equ TRAPNO    + 4
  EIPREG		equ	ERRNO		+ 4
  CSREG		equ	EIPREG		+ 4
  EFLAGSREG	equ	CSREG		+ 4
  ESPREG		equ	EFLAGSREG	+ 4
  SSREG		equ	ESPREG		+ 4
  P_STACKTOP	equ	SSREG		+ 4
  P_LDT_SEL	equ	P_STACKTOP
  P_LDT		equ	P_LDT_SEL	+ 4

global start_process
extern p_proc_ready
extern cpu
start_process:
  mov esp, [p_proc_ready]
  lldt [esp + P_LDT_SEL]

  pop gs
  pop fs
  pop es
  pop ds
  popad
  add esp, 8

  iretd
