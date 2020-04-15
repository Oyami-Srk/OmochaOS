#include "generic/asm.h"
#include "lib/stdlib.h"
#include "lib/syscall.h"
#include "modules/memory.h"
#include "modules/systask.h"
#include "modules/tty.h"

// first actual process
void init(void) {
    delay_ms(2000);
    process *p;
    printf("[INIT] Init Process Start.\n");

    int ret = fork();
    if (ret) {
        printf("I am parent, child pid is %d\n", ret);
    } else {
        printf("I am child.\n");
        exit(10);
    }
    printf("....%d\n", ret);

    while (1) {
        uint s  = 0;
        int  cp = wait(&s);
        printf("[INIT] Child %d exited with status %d.\n", cp, s);
    }
}
