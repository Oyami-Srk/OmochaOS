#ifndef __MODULE_MONITOR_H__
#define __MODULE_MONITOR_H__

#include "generic/typedefs.h"

struct console {
    uint origin_offset;
    uint disp_start_offset;
    uint end_offset;
    uint disp_offset;
};

void flush_console(struct console *con);
void switch_console(struct console *con);
void write_console(struct console *con, const char *str);
void clear_console(struct console *con, uint offset, size_t len);
void init_console(struct console *con, int vm_nr);
void init_monitor();

#endif // __MODULE_MONITOR_H__
