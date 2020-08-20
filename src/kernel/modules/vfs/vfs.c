#include "vfs.h"
#include <modules/systask.h>

#include <modules/tty.h>

void Task_VFS() {
    message msg;
    if (reg_proc("TaskVFS") != 0)
        printf("Cannot register as TaskVFS!\n");
    while (1) {
    }
}
