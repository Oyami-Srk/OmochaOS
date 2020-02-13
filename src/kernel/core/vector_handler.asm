;; Vector Handler used in IVT

[BITS 32]
[SECTION .text]

extern interrupt_handler

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
    
vector_handler_ret:
    dec dword [interrupt_count]
    cmp dword [interrupt_count], 0
    jne .non_zero
.non_zero:
    pop gs
    pop fs
    pop es
    pop ds
    popad
    add esp, 8 ; trapno, err pushed in IVT
    iretd



[BITS 32]
[SECTION .data]
interrupt_count dd 1
interrupt_stack resb 4096
