/*
module:
  name: Loader
  author: Shiroko
  summary: Loader Process
  entry: Task_Loader
*/

#include "core/memory.h"
#include "lib/stdlib.h"
#include "lib/string.h"
#include "lib/syscall.h"
#include "modules/systask.h"
#include "modules/tty.h"

#include "core/environment.h"

void Task_Loader(void) {
    /* delay_ms(1000); */
    printf("[LOADER] Loading Execs from Disk.\n");
    printf("[Loader] sizeof process is %d\n", sizeof(process));
    /* exit_proc(); */
    while (1)
        ;
}
