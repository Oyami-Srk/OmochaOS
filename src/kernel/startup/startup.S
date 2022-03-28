/* This file contain the startup code and multiboot header
 * Author: Shiroko
 */

#include "./multiboot.h"
#include <sys/memory.h>

/* Multiboot Header */
#define MULTIBOOT_FLAGS (MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO)

/* struct multiboot_header_t */
.section .multiboot
.align 4
.long MULTIBOOT_HEADER_MAGIC
.long MULTIBOOT_FLAGS
.long -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_FLAGS)
.long 0
.long 0
.long 0
.long 0
.long 0
.long 0
.long 0
.long 0
.long 0
/* end of struct multiboot_header_t */

/* Kernel stack in BSS */
.section .bss
.align 16
stack_bottom:
.skip 0x4000
stack_top:

/* Startup code */
.section .text
.global _start
.type _start, @function
_start:
    /* Set paging control register */
    movl %cr4, %ecx
    or   $0x10, %ecx /* Enable extended page size */
    movl %ecx, %cr4

    /* Load Page Table */
    movl $core_page_dir - KERN_BASE, %ecx
    movl %ecx, %cr3

    /* Enable Paging */
    movl %cr0, %ecx
    or   $0x80000000, %ecx
    movl %ecx, %cr0

    /* Setup Stack */
    movl $stack_top, %esp

    /* Get multiboot info */
    pushl %eax
    add KERN_BASE, %ebx
    pushl %ebx

    /* Ensure CPUID */
    movl $0x0, %eax
    cpuid

    /* Enter Main */
    call core_main

    /* Capture Illegal Return */
    cli
    xchg %bx, %bx /* Bochs magic breakpoint */

1:  hlt
    jmp 1b /* Dead loop */
