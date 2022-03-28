/* This file contains multiboot header and entry point
 * Author: Shiroko
 */

#if 0
#include "./multiboot.h"
#include <sys/memory.h>
#include <sys/paging.h>

// Prepare multiboot header
#define MULTIBOOT_FLAG (MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO)

// clang-format off
static struct multiboot_header __h
    __attribute__((used, aligned(4), section(".multiboot"))) = {
        .magic = MULTIBOOT_HEADER_MAGIC,
        .flags = MULTIBOOT_FLAG, 
        .checksum = -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_FLAG),
};
// clang-format on

// Prepare BSS Section
static unsigned char __bss[0x4000]
    __attribute__((aligned(16), section(".bss")));

// Entry point
extern pde_t core_page_dir[PDE_SIZE];
extern void  core_main(multiboot_info_t *, unsigned int);

void _start(void) {
    // Set Paging control register
    asm volatile("movl %%cr4, %%ecx\n\t"
                 "or   $0x10, %%ecx\n\t" // Enable Extended Page Size
                 "movl %%ecx, %%cr4\n\t"
                 :
                 :
                 : "ecx", "memory");

    while (1)
        ;
    // Load Page Table
    asm volatile("movl %%ecx, %%cr3\n\t"
                 :
                 : "ecx"(((unsigned int)core_page_dir) - KERN_BASE)
                 :);

    // Enable Paging
    asm volatile("movl %%cr0, %%ecx\n\t"
                 "or   $0x80000000, %%ecx\n\t" // Enable Paging
                 "movl %%ecx, %%cr0\n\t"
                 :
                 :
                 : "ecx", "memory");
    // Setup Stack
    asm volatile("movl %%ecx, %%esp\n\t" : : "ecx"(__bss) :);

    // Get multiboot info
    unsigned int mb_info_addr;
    unsigned int mb_magic;
    asm volatile("movl %%eax, %0\n\t"
                 "movl %%ebx, %1\n\t"
                 : "=r"(mb_magic), "=r"(mb_info_addr)
                 :
                 :);

    // Ensure CPUID is usable
    asm volatile("cpuid\n\t" ::: "eax");

    // Enter Main
    core_main((multiboot_info_t *)(mb_info_addr + KERN_BASE), mb_magic);

    // Capture Illegal Return
    asm volatile("cli\n\t"
                 "xchg %bx, %bx");
    for (;;) {
        asm volatile("hlt\n\t");
    }
}

#endif