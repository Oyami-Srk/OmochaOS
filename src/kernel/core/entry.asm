;; entry for multiboot bootloader.

;; definations

MB_MAGIC    equ 0x1BADB002
MB_FLAGS    equ 0x0
MB_CHECKSUM equ -(MB_MAGIC + MB_FLAGS)
KERNEL_BASE equ 0x80000000
STACKSIZE   equ 0x2000  ; 8 Kib

;; multiboot header

[section .multiboot]
align 4
    dd MB_MAGIC
    dd MB_FLAGS
    dd MB_CHECKSUM

;; bss section
[section .bss]
align 16
stack:
    resb STACKSIZE
stack_top:

[section .text]
global _start
extern core_main
extern entry_page_dir



_start:
    mov ecx, cr4
    or ecx, 0x10    ; extend page size to 4MByte
    mov cr4, ecx

    mov ecx, entry_page_dir - KERNEL_BASE
    mov cr3, ecx
    
    mov ecx, cr0
    or ecx, 0x80000000
    mov cr0, ecx

    mov esp, stack_top

    push eax    ; multiboot magic number
    push ebx    ; multiboot header

    call core_main
    hlt
    .end:

