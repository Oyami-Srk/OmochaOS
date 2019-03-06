  ; Vector handler used in vector_table

  ; struct used

%define proc_count 3

struc stack_frame
.gs resd 1
.fs resd 1
.es resd 1
.ds resd 1
.edi resd 1
.esi resd 1
.ebp resd 1
.oesp  resd 1
.ebx resd 1
.edx resd 1
.ecx resd 1
.eax resd 1
.trapno resd 1
.err resd 1
.eip resd 1
.cs resd 1
.eflags resd 1
.esp resd 1
.ss resd 1
endstruc

struc tss
.backlink resd 1
.esp0 resd 1
.ss0 resd 1
.esp1 resd 1
.ss1 resd 1
.esp2 resd 1
.ss2 resd 1
.cr3 resd 1
.eip resd 1
.flags resd 1
.eax resd 1
.ecx resd 1
.edx resd 1
.ebx resd 1
.esp resd 1
.ebp resd 1
.esi resd 1
.edi resd 1
.es resd 1
.cs resd 1
.ss resd 1
.ds resd 1
.fs resd 1
.gs resd 1
.ldt resd 1
.trap resw 1
.iobase resw 1
endstruc

struc message
.sender resd 1
.receiver resd 1
.type resd 1
.major_data resd 1
.data resb 16
endstruc

struc process
.stack resb stack_frame_size
.selector_ldt resw 1
.status resd 1
.ldts resb 2 * 8
.pid resd 1
.name resb 16
; .message resb message_size
.message resd 1
.quene_sending_to_this_process resd 1
endstruc

struc cpu_env
.cpu_id resd 1
.interrupt_count resd 1
.current_running_proc resd 1

.idt resb 256 * 8
.gdt resb 128 * 8
.pgdir resd 1
.process resb process_size * proc_count
.tss resb tss_size

.beats resd 1
endstruc

[SECTION .text]
[BITS 32]
extern interrupt_handler
extern kernel_stack             ; 内核栈
extern cpu
global vector_handler
global vector_handler_ret
vector_handler:
  jmp .1
  dd cpu_env_size
  dd process_size
  dd stack_frame_size
.1:
  pushad
  push ds
  push es
  push fs
  push gs

  mov ax, 2 << 3                ; 2 means Data desc
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov ecx, esp
  cmp dword [cpu + cpu_env.interrupt_count], 0
  jne .non_zero
  mov esp, kernel_stack + 4096  ; page size
.non_zero:
  inc dword [cpu + cpu_env.interrupt_count]
  push dword ecx

  call interrupt_handler
  add esp, 4                    ; C调用规约

vector_handler_ret:
  dec dword [cpu + cpu_env.interrupt_count]
  cmp dword [cpu + cpu_env.interrupt_count], 0
  jne .non_zero
  ; check if syscall and move retval
  mov ecx, [cpu + cpu_env.current_running_proc]

  cmp dword [ecx + process.stack + stack_frame.trapno], 0xE9 ; syscall const
  jne .non_syscall
  mov dword [ecx + process.stack + stack_frame.eax], eax
.non_syscall:
  mov ebx, [ecx + process.selector_ldt]
  lldt bx
  mov esp, ecx
  add ecx, process.selector_ldt
  mov dword [cpu + cpu_env.tss + tss.esp0], ecx ; GCC自动对齐了结构体的内存

.non_zero:
  pop gs
  pop fs
  pop es
  pop ds
  popad
  add esp, 8                    ; tapno and err
  iretd
