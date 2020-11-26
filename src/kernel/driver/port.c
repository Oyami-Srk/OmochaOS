#include <driver/driver.h>
#include <generic/asm.h>
#include <lib/stdlib.h>

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

int serial_received() { return inb(PORT + 5) & 1; }

char read_serial() {
    while (serial_received() == 0)
        ;

    return inb(PORT);
}

int is_transmit_empty() { return inb(PORT + 5) & 0x20; }

void write_serial(char a) {
    while (is_transmit_empty() == 0)
        ;

    outb(PORT, a);
}

void printf_serial(const char *fmt, ...) {
    int     i;
    char    buf[256];
    va_list arg = (va_list)((char *)(&fmt) + 4);
    i           = vsprintf(buf, fmt, arg);
    buf[i]      = 0;

    char *str = buf;

    while (*str != 0) {
        write_serial(*str);
        str++;
    }
}

/*

static Driver_Declaration driver_PORTCOM1 = {.magic       = DRIVER_DC,
                                             .name        = "COM1",
                                             .major_ver   = 0,
                                             .minor_ver   = 1,
                                             .level       = -1,
                                             .init        = init_port,
                                             .initialized = FALSE};

ADD_DRIVER(driver_PORTCOM1);
*/