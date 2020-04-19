#include "generic/asm.h"
#include "lib/stdlib.h"
#include "lib/syscall.h"
#include "modules/memory.h"
#include "modules/systask.h"
#include "modules/tty.h"

// first actual process
void init(void) {
    printf("[INIT] Init Process Start.\n");

    int ret = fork();
    if (ret) {
        printf("I am parent, child pid is %d\n", ret);
    } else {
        printf("I am child, going to exec test.out\n");
        const char *argv[] = {"arg1", "pworld", "helloi", "arg4", NULL};
        const char *env[]  = {"env1", "env2", NULL};
        execve("/test.out", argv, env);
        exit(10);
    }

    while (1) {
        uint s  = 0;
        int  cp = wait(&s);
        printf("[INIT] Child %d exited with status %d.\n", cp, s);
    }
}
