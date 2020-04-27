#include "core/interrupt.h"
#include "core/process.h"
#include "driver/graphic.h"
#include "generic/asm.h"
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
    // init_kbd();
    reg_int_msg(HW_IRQ_KBD);
    enable_irq(HW_IRQ_KBD);

    while (1) {
        recv_msg(&msg, PROC_ANY);
        kprintfc(BLUE, BLACK, "%d", msg.type);
        switch (msg.type) {
        case MSG_INTERRUPT: {
            /*
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
            break;
            */
            kprintfc(YELLOW, BLACK, "%x ", inb(0x60));
            break;
        }
        default: {
            const char *buf =
                (const char *)proc_vir2phy(msg.sender, (char *)msg.major);
            write_console(&con[cur_con], buf);
            SEND_BACK(msg);
            break;
        }
        }
    }
}
