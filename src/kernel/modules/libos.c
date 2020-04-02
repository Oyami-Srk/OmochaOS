#include "driver/misc.h"
#include "lib/stdlib.h"
#include "modules/systask.h"

uint query_proc(const char *name) {
    message msg;
    if (strlen(name) > 16)
        return 0;
    strcpy(msg.data.b16, (char *)name);
    uint pid         = 0;
    uint beats_begin = get_ticks_msg();
    while (pid == 0 &&
           ((get_ticks_msg() - beats_begin) * 1000 / BEATS_RATE) <
               QUERY_PROC_TIMEOUT) { // if cannot find proc, halt and retry
        msg.type     = QUERY_PROC;
        msg.receiver = SYSTASK_PID;
        send_msg(&msg);
        recv_msg(&msg, SYSTASK_PID);
        pid = msg.major;
        if (pid)
            break;
        delay_ms(QUERY_PROC_TIMEOUT / 10); // retry 10 times
    }
    return pid;
}

int printf(const char *fmt, ...) {
    message msg;
    int     i;
    char    buf[256];
    va_list arg   = (va_list)((char *)(&fmt) + 4);
    i             = vsprintf(buf, fmt, arg);
    buf[i]        = 0;
    msg.major     = (uint)buf;
    msg.type      = 1;
    uint task_tty = query_proc("TaskTTY");
    if (task_tty == 0)
        return 0;
    msg.receiver = task_tty;
    send_msg(&msg);
    recv_msg(&msg, task_tty);
    return strlen(buf);
}
