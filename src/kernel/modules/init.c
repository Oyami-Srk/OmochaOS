#include "generic/asm.h"
#include "lib/stdlib.h"
#include "lib/syscall.h"
#include "modules/memory.h"
#include "modules/systask.h"
#include "modules/tty.h"

// first actual process
void init(void) {
    /* printf("Init Process Start.\n"); */
    /* int ret = fork(); */
    int ret = 0;
    if (ret)
        printf("I am parent, child is %d\n", ret);
    else {
        printf("I am child.\n");
        /* int ret2 = fork(); */
        int ret2 = 0;
        if (ret2 == 0) {
            printf("I am child's child.\n");
        } else {
            printf("I am still child, my child pid is %d\n", ret2);
        }
    }

    while (1)
        ;
}
