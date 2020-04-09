#ifndef __MODULE_MEMORY__
#define __MODULE_MEMORY__

#include "modules/systask.h"

#ifndef PG_SIZE
#define PG_SIZE 0x1000
#endif

#define MEM_ALLOC_PAGES  1
#define MEM_FREE_PAGES   2
#define MEM_FORK_PROC    3
#define MEM_DESTROY_PROC 4
#define MEM_WAIT_PROC    5

static inline char *mem_alloc_pages(uint pages) {
    uint    task_mem = query_proc("TaskMM");
    message msg;
    msg.type     = MEM_ALLOC_PAGES;
    msg.major    = pages;
    msg.receiver = task_mem;
    send_msg(&msg);
    recv_msg(&msg, task_mem);
    return (char *)msg.major;
}

static inline void mem_free_pages(char *block, uint pages) {
    uint    task_mem = query_proc("TaskMM");
    message msg;
    msg.type             = MEM_FREE_PAGES;
    msg.major            = pages;
    msg.data.uint_arr.d1 = (uint)block;
    msg.receiver         = task_mem;
    send_msg(&msg);
    recv_msg(&msg, task_mem);
}

uint fork();
int  wait(uint *);
void exit(uint);

#endif // __MODULE_MEMORY__
