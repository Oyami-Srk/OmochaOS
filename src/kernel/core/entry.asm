;; entry for multiboot and our bootloader

;; definations

MB_MAGIC    equ 0x1BADB002
MB_FLAGS    equ 0x0
MB_CHECKSUM equ -(MB_MAGIC + MB_FLAGS)
KERNEL_BASE equ 0x80000000



;; multiboot header

[SECTION .multiboot]
ALIGN 4
    dd MB_MAGIC
    dd MB_FLAGS
    dd MB_CHECKSUM

;; bss section
[SECTION .bss]
ALIGN 16
stack_bottom:
    resb 8192   ; 8 KiB
stack_top:

;; text section
[SECTION .text]
global _start           ; _start for multiboot, require a phy addr
extern core_entry       ; core entry in core.c
extern entry_page_dir   ; entry page dir in core.c

_start equ start - KERNEL_BASE

start:
    mov eax, cr4
    or eax, 0x10    ; extend page size to 4 MByte
    mov cr4, eax

    mov eax, entry_page_dir
    mov cr3, eax

    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    mov esp, stack_top
    
    call core_entry
    hlt
    .end:

