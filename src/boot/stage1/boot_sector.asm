  ; Coding for fun, not for salary.
  ; Coding for will, till the top.
  ; 码乐隔币，码德至璋

%include "protect.asm"
%define entry_memory_addr 0x7c00 ; maybe be put to const.inc later


  ;; ELF模式下不可以用org来制定加载位置
  ;; 使用link来使.text段在0x7c00处加载
; org entry_memory_addr
  stack_base equ entry_memory_addr ; 栈顶

  ;; 现在我有两个选择，一个是加载一个FAT分区然后读取loader
  ;; 或者是从硬盘的第一个sector中读取loader
  ;; 我选择先从sector中读取loader，然后日后如果设计出一个
  ;; 可以用很少的代码就定位loader的文件系统的话，可能会切换到
  ;; 那种方式。虽然debug可能困难些，不过应该无所谓。
  ;; 以下代码学习xv6，直接跳入保护模式并且使用C语言读取第一个扇区

  jmp start                    ; 跳入16位实模式并且立即进入保护模式
  nop                           ; 但是不进入长模式，长模式由内核处理。

_dwnMCR equ 0x8000 - 4
_MINFO equ 0x8000

_gdt:                           ; 全局描述符表
null_desc: Descriptor 0, 0, 0
code_desc: Descriptor 0, 0xFFFFF, DA_32 | DA_4K | DA_C_E ; Code - Execute
data_desc: Descriptor 0, 0xFFFFF, DA_32 | DA_4K | DA_D_RW ; Data - ReadWrite
  ;; End of GDT
  GDT_size equ $ - _gdt
  pGDT dw GDT_size -1           ; limit = sizeof gdt
  dd _gdt   ; base addr

  ;; Selectors
  S_Code equ code_desc - _gdt
  S_Data equ data_desc - _gdt

  ;; 只有一个.text段，用于链接到执行地址
[SECTION .text]
[BITS 16]
global start
start:
  ; mov ax, cs
  xor ax, ax
  mov ds, ax
  ; mov es, ax
  mov ss, ax
  mov sp, stack_base
  xor ebx, ebx
  mov di, _MINFO
.memory_loop:
  mov eax, 0xE820
  mov ecx, 24
  mov edx, 0x534D4150         ; 'SMAP'
  int 15h
  jc .memory_fail
  add di, 24
  inc dword [_dwnMCR]
  cmp ebx, 0
  jne .memory_loop
  jmp .done

.memory_fail:
  ; mov dword [_dwnMCR], 0
  jmp $
.done:
  ; 内存检测完成
  ; 直接跳入保护模式，然后加载C代码
  ; 长模式交由内核处理，Loader负责处理硬件信息并
  ; 写入Infos结构体，Infos结构体仅能被内核访问
  ; 并且要求内核访问完毕后在结构体地址建立可以被驱动
  ; 访问的Infos，以此类推。
  cli                           ; 关中断以跳入保护模式
  lgdt [pGDT]
  ; cli

  in al, 92h
  or al, 0x02
  out 92h, al

  mov eax, cr0
  or eax, 1                     ; 置第一位即保护模式位为1
  mov cr0, eax

  jmp dword S_Code:_start32 ; 刷新流水线，正式进入保护模式
  hlt

;ALIGN 32
[BITS 32]
extern boot_main
_start32:
  mov ax, S_Data                ; 初始化各种寄存器
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov ss, ax
  mov esp, stack_base

  call boot_main                ; 跳入C语言继续处理

  ; boot_main并不返回，但是出错时除外。
  ; 剩下的代码接受错误并旋锁

  ; mov ax, 0x8A00                ; 触发Bochs的断点
  ; mov dx, ax
  ; out dx, ax
  ; mov ax, 0x8AE0
  ; out dx, ax

  jmp $

