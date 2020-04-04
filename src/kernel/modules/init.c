#include "lib/syscall.h"
#include "modules/memory.h"
#include "modules/systask.h"
#include "modules/tty.h"

// first actual process
void init(void) {
    printf("Init Process Start.\n");
    while (1)
        ;
    int ret = fork();
    if (ret)
        printf("I am parent, child is %d\n", ret);
    else
        printf("I am child.\n");
    while (1)
        ;
}
