#include "core/process.h"
#include "driver/graphic.h"
#include "kbd.h"
#include "lib/stdlib.h"
#include "lib/string.h"
#include "lib/syscall.h"
#include "modules/memory.h"
#include "modules/systask.h"
#include "monitor.h"

uint       cur_con = 0;
const uint max_con = 4;

struct console con[4];

void Task_TTY() {
    message msg;
    if (reg_proc("TaskTTY") != 0)
        kprintf("Cannot register as TaskTTY!\n");
    uint key = 0;
    init_kbd();
    init_monitor();
    for (uint i = 0; i < max_con; i++)
        init_console(&con[i], i);
    write_console(&con[0], "Monitor Initialized!\nConsole #1> ");
    write_console(&con[1], "Console #2> ");
    write_console(&con[2], "Console #3> ");
    write_console(&con[3], "Console #4> ");

    while (1) {
        if (peek_msg()) {
            recv_msg(&msg, PROC_ANY);
            const char *buf =
                (const char *)proc_vir2phy(msg.sender, (char *)msg.major);
            write_console(&con[cur_con], buf);
            SEND_BACK(msg);
        }
        if (!((key = kbd_read()) & 0x0100)) {
            char buf[2];
            sprintf(buf, "%c", key);
            write_console(&con[cur_con], buf);
            flush_console(&con[cur_con]);
        } else {
            int raw = key & MASK_RAW;
            switch (raw) {
            case ENTER:
                write_console(&con[cur_con], "\n");
                break;
            case BACKSPACE:
                write_console(&con[cur_con], "\b");
                break;
            }
        }
    }
}
