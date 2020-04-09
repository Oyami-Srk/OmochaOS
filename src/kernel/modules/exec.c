/*
module:
  name: Exec
  author: Shiroko
  summary: Exec Process
  entry: Task_Exec
*/

#include "modules/exec.h"
#include "modules/memory.h"
#include "modules/systask.h"
#include "modules/tty.h"

void Task_Exec(void) {
    printf("Exec proc start.\n");
    while (1)
        ;
}
