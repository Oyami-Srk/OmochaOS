#include "monitor.h"
#include "driver/graphic.h"
#include "lib/string.h"

void flush_console(struct console *con) {
    GRAPHIC_set_vm_display_addr(con->disp_start_offset);
    GRAPHIC_set_cursor_addr(con->disp_offset);
}

void switch_console(struct console *con) { flush_console(con); }

void write_console(struct console *con, const char *str) {
    con->disp_offset = GRAPHIC_write_color_string_to_vm(
        con->disp_offset, COLOR(BLACK, WHITE | BRIGHT), str);
}

void clear_console(struct console *con, uint offset, size_t len) {
    memset((void *)(0xB8000 + con->origin_offset + offset), 0, len);
}

void init_console(struct console *con, int vm_nr) {
    con->origin_offset     = (SCREEN_SIZE * 2 * 2) * vm_nr;
    con->end_offset        = con->origin_offset + SCREEN_SIZE * 2 * 2;
    con->disp_start_offset = con->disp_offset = con->origin_offset;
    clear_console(con, 0, SCREEN_SIZE * 2 * 2);
}
