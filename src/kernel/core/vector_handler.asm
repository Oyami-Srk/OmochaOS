;; Vector Handler used in IVT

%include "core/process.i"

[BITS 32]
[SECTION .text]

extern interrupt_handler    ; defined in interrupt.c
extern proc_running         ; defined in process.c
extern tss                  ; defined in protect.c, is a pointer
; extern core_page_dir        ; defined in core.c
; extern interrupt_stack    ; defined in interrupt.c

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
    push dword ecx ; original stack top
    push dword [interrupt_count]
    inc dword [interrupt_count]
    ; mov ebx, core_page_dir
    ; mov edx, cr3
    ; cmp edx, ebx
    ; je .same_page_dir
; .same_page_dir:
    ; ; mov cr3, ebx ; load global page dir for interrupt
    call interrupt_handler
    add esp, 8
   
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

