;; Interrupt Vector Table
[BITS 32]
[SECTION .text]
extern vector_handler

%assign i 0
%rep 256
global vector%+i
vector%+i:
%if (i < 10 || i > 14) && (i != 8 && i != 17)
    push dword 0x0
%endif
    push dword i
    jmp vector_handler
%assign i i+1
%endrep

[BITS 32]
[SECTION .data]
global vector_table
vector_table:
%assign i 0
%rep 256
    dd vector%+i
%assign i i+1
%endrep
