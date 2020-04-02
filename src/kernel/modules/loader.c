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
    printf("[LOADER] Loading Execs from Disk.\n");
    exit_proc();
}
