;; entry for multiboot bootloader.

%include "multiboot.i"

;; definations
MB_MAGIC    equ 0x1BADB002
MB_FLAGS    equ MULTIBOOT_FLAG
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
extern core_page_dir



_start:
    mov ecx, cr4
    or ecx, 0x10    ; extend page size to 4MByte
    mov cr4, ecx

    mov ecx, core_page_dir - KERNEL_BASE
    mov cr3, ecx
    
    mov ecx, cr0
    or ecx, 0x80000000
    mov cr0, ecx

    mov ecx, .1
    jmp ecx ; directly jump to higher addr
.1:
    ; mov dword [core_page_dir], 0   ; clear the 0-4M's map
    ; mov ecx, cr3
    ; mov cr3, ecx ; reload

    mov esp, stack_top

    push eax    ; multiboot magic number
    add ebx, KERNEL_BASE ; ebx is stored by phy address
    push ebx    ; multiboot header

    call core_main
    cli
    xchg bx,bx
.end:
    hlt
    jmp .end

