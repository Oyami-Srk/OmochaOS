#include "multiboot.h"

#include <arch/x86_asm.h>
#include <sys/memory.h>
#include <sys/paging.h>

pde_t core_page_dir[PDE_SIZE];

#define PORT 0x3f8 /* COM1 */

int init_port(void) {

    outb(PORT + 1, 0x00); // Disable all interrupts
    outb(PORT + 3, 0x80); // Enable DLAB (set baud rate divisor)
    outb(PORT + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
    outb(PORT + 1, 0x00); //                  (hi byte)
    outb(PORT + 3, 0x03); // 8 bits, no parity, one stop bit
    outb(PORT + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
    outb(PORT + 4, 0x0B); // IRQs enabled, RTS/DSR set

    return 0;
}
int  is_transmit_empty() { return inb(PORT + 5) & 0x20; }
void write_serial(char a) {
    while (is_transmit_empty() == 0)
        ;

    outb(PORT, a);
}

_Noreturn void core_main(multiboot_info_t *multiboot_header,
                         unsigned int      magic) {
    write_serial('H');
    write_serial('e');
    write_serial('l');
    write_serial('l');
    write_serial('o');
    write_serial(' ');
    write_serial('W');
    write_serial('a');
    while (1)
        ;
}

// PDE enable large Page Size - 4MB
// clang-format off
__attribute__((__aligned__(PG_SIZE))) pde_t core_page_dir[PDE_SIZE] = {
    [0]               = (0) | PG_Present | PG_Writable | PG_PS,
    [1]               = (0x00400000) | PG_Present | PG_Writable | PG_PS,
    [2]               = (0x00800000) | PG_Present | PG_Writable | PG_PS | PG_PCD,
    [3]               = (0x00C00000) | PG_Present | PG_Writable | PG_PS,

    [KERN_BASE >> 22] = (0) | PG_Present | PG_Writable | PG_PS | PG_User,
    [(KERN_BASE >> 22)+1] = (0x00400000) | PG_Present | PG_Writable | PG_PS,
    [(KERN_BASE >> 22)+2] = (0x00800000) | PG_Present | PG_Writable | PG_PS | PG_PCD,
    [(KERN_BASE >> 22)+3] = (0x00C00000) | PG_Present | PG_Writable | PG_PS,
};
// clang-format on
