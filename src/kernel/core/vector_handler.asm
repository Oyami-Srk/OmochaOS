;; Vector Handler used in IVT

%include "core/process.i"

[BITS 32]
[SECTION .text]

extern interrupt_handler    ; define in interrupt.c
extern proc_running         ; define in process.c
extern tss                  ; define in protect.c, is a pointer
; extern interrupt_stack      ; define in interrupt.c

interrupt_count dd 1
interrupt_stack resb 8196 ; 8KB Interrupt stack defined in .text
interrupt_stack_bottom:


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
    mov esp, interrupt_stack_bottom
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
    ; handle syscall here
    cmp dword [ecx + process.stack + stack_frame.trap_no], 0xE9 ; syscall int
    jne .non_syscall
    mov dword [ecx + process.stack + stack_frame.eax], eax
.non_syscall:
    mov ebx, [ecx + process.page_dir]
    mov edx, cr3
    cmp edx, ebx
    je .set_esp0
    mov cr3, ebx
.set_esp0:
    mov esp, ecx
    add ecx, process.page_dir
    mov eax, [tss]
    mov dword [eax + __tss.esp0], ecx
.non_zero:
    pop gs
    pop fs
    pop es
    pop ds
    popad
    add esp, 8 ; trapno, err pushed in IVT
    iretd

