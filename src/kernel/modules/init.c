#include "generic/asm.h"
#include "lib/stdlib.h"
#include "lib/syscall.h"
#include "modules/memory.h"
#include "modules/systask.h"
#include "modules/tty.h"

// first actual process
void init(void) {
    delay_ms(2000);
    printf("[INIT] Init Process Start.\n");
    uint *x = (uint *)0x3123000;
    printf("Access %x: ", x);
    *x = 1234;
    printf(" %d", *x);
    while (1) {
        uint s  = 0;
        int  cp = wait(&s);
        printf("[INIT] Child %d exited with status %d.\n", cp, s);
    }
}
