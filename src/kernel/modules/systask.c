/*
module:
  name: SysTask
  author: Shiroko
  summary: System Task's process
  entry: SysTask
  preferred_pid: 1
*/

#include "lib/stdlib.h"
#include "lib/string.h"

void SysTask() {
    kprintf("HiHi, This is systask yoooo~\n");
    while (1)
        ;
}
