;; Vector Handler used in IVT

%include "core/process.i"

[BITS 32]
[SECTION .text]

extern interrupt_handler
extern proc_running
extern tss
extern interrupt_stack

interrupt_count dd 1

global vector_handler
vector_handler:
    pushad
    push ds
    push es
    push fs
    push gs

    mov esi, edx
    mov ax, 2 << 3 ;; data selector, hard code is bad
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ecx, esp
    cmp dword [interrupt_count], 0
    jne .non_zero
    mov esp, interrupt_stack + 4096
.non_zero:
    inc dword [interrupt_count]
    push dword ecx ; original stack top
    call interrupt_handler
    add esp, 4
   
global vector_handler_ret
vector_handler_ret:
    dec dword [interrupt_count]
    cmp dword [interrupt_count], 0
    jne .non_zero
    mov ecx, [proc_running]
    mov esp, ecx    ; switch to proc stack frame
    ; handle syscall here
.non_syscall:
    mov ebx, [ecx + process.page_dir]
    mov edx, cr3
    cmp edx, ebx
    je .non_zero
    mov cr3, ebx
    mov esp, ecx
    add ecx, process.page_dir
    mov dword [tss + __tss.esp0], ecx
.non_zero:
    pop gs
    pop fs
    pop es
    pop ds
    popad
    add esp, 8 ; trapno, err pushed in IVT
    iretd

