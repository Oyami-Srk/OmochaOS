#ifndef __MODULE_MEMORY__
#define __MODULE_MEMORY__

#include "modules/systask.h"

#ifndef PG_SIZE
#define PG_SIZE 0x1000
#endif

#define MEM_ALLOC_PAGES 1
#define MEM_FREE_PAGES  2
#define MEM_FORK_PROC   3

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

static inline uint fork() {
    uint    task_mem = query_proc("TaskMM");
    message msg;
    msg.type     = MEM_FORK_PROC;
    msg.receiver = task_mem;
    send_msg(&msg);
    recv_msg(&msg, task_mem);
    return msg.major;
}

#endif // __MODULE_MEMORY__
