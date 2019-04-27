; vector handler used in vector_table
[SECTION .text]
[BITS 32]
extern interrupt_handler ; interrupt.c

global vector_handler
vector_handler:
  pushad
  push ds
  push es
  push fs
  push gs

  mov esi, edx
  mov ax, 2 << 3                ; 2 means Data desc
  mov ds, ax
  mov es, ax
  mov edx, esi

  push esp
  call interrupt_handler
  add esp, 4                    ; C调用规约
  ; jmp .vector_handler_ret

.vector_handler_ret:
  pop gs
  pop fs
  pop es
  pop ds
  popad
  add esp, 8                    ; tapno and err
  iretd
