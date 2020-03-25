/*
module:
  name: Test
  author: Shiroko
  summary: Test Process
  entry: Task_Test
*/

#include "core/memory.h"
#include "lib/stdlib.h"
#include "lib/string.h"
#include "lib/syscall.h"
#include "modules/systask.h"
#include "modules/tty.h"

void Task_Test(void) {
    printf("Hello world!\n");
    query_proc("Hi");
    while (1)
        ;
}
