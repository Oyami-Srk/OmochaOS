#include "tty.h"
#include "kbd.h"
#include "monitor.h"
#include <core/interrupt.h>
#include <core/process.h>
#include <driver/graphic.h>
#include <generic/asm.h>
#include <lib/stdlib.h>
#include <lib/string.h>
#include <lib/syscall.h>
#include <modules/memory.h>
#include <modules/systask.h>
#include <modules/tty.h>

uint       cur_con = 0;
const uint max_con = 4;

struct console con[4];

_Noreturn void Task_TTY() {
    message msg;
    if (reg_proc("TaskTTY") != 0)
        kprintf("Cannot register as TaskTTY!\n");
    uint key = 0;
    reg_int_msg(HW_IRQ_KBD);
    init_kbd();

    while (1) {
        recv_msg(&msg, PROC_ANY);
        switch (msg.type) {
        case MSG_INTERRUPT: {
            if (kbd_code_end()) {
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
            break;
        }
        case TTY_WRITE: {
            const char *buf =
                (const char *)proc_vir2phy(msg.sender, (char *)msg.major);
            write_console(&con[cur_con], buf);
            SEND_BACK(msg);
            break;
        }
        }
    }
}
