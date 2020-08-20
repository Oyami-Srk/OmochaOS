#include "monitor.h"
#include <driver/graphic.h>
#include <lib/string.h>

void flush_console(struct console *con) {}

void switch_console(struct console *con) { flush_console(con); }

void write_console(struct console *con, const char *str) {
    while (*str != 0) {
        kputc_color(*str, DEFAULT_FG, DEFAULT_BG);
        str++;
    }
}

void clear_console(struct console *con, uint offset, size_t len) {}

void init_console(struct console *con, int vm_nr) {}

void init_monitor() {}