; vector handler used in vector_table
[SECTION .text]
[BITS 32]
extern interrupt_handler ; interrupt.c
extern interrupt_count   ; kernel.c
extern kernel_stack      ; kernel.c
extern tss               ; kernel.c
extern current_running_proc  ; proc.c

%include "kernel/pm.inc"

global vector_handler
global vector_handler_ret

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
  mov fs, ax
  mov ecx, esp
  cmp dword [interrupt_count], 0
  jne .non_zero
  mov esp, kernel_stack + 4096  ; page size
.non_zero:
  inc dword [interrupt_count]
  push dword ecx
  call interrupt_handler
  add esp, 4                    ; C调用规约
  ; jmp .vector_handler_ret

vector_handler_ret:
  dec dword [interrupt_count]
  cmp dword [interrupt_count], 0
  jne .non_zero
  ; check wheather syscall and pass retval
  mov ecx, [current_running_proc]

  cmp dword [ecx + __process.stack + stack_frame.trap_no], 0xE9 ; syscall int
  jne .non_syscall
  mov dword [ecx + __process.stack + stack_frame.eax], eax
.non_syscall:
  mov ebx, [ecx + __process.selector_ldt]
  lldt bx
  mov esp, ecx
  add ecx, __process.selector_ldt
  mov dword [tss + __tss.esp0], ecx
  
.non_zero:
  pop gs
  pop fs
  pop es
  pop ds
  popad
  add esp, 8                    ; tapno and err
  iretd
