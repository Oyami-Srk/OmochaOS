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
