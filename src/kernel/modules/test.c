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

#include "core/environment.h"

void Task_Test(void) {
    printf("Hello world!\n");
    uint mem_lower = query_env(ENV_KEY_MEMORY_LOWER, NULL, 0);
    uint mem_upper = query_env(ENV_KEY_MEMORY_UPPER, NULL, 0);
    printf("MEM_LOWER: 0x%x, MEM_UPPER: 0x%x\n", mem_lower, mem_upper);
    struct core_env_memory_zone zone[10];
    uint zone_count = query_env(ENV_KEY_MMAP, (ubyte *)zone,
                                sizeof(struct core_env_memory_zone) * 10);
    for (uint i = 0; i < zone_count; i++) {
        printf("Addr: 0x%x, Length: 0x%x, Type: %d\n", zone[i].addr,
               zone[i].length, zone[i].type);
    }

    while (1)
        ;
}
