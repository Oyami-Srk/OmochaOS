#ifndef __MODULE_MEMORY__
#define __MODULE_MEMORY__

#include "modules/systask.h"

#ifndef PG_SIZE
#define PG_SIZE 0x1000
#endif

#define MEM_ALLOC_PAGES 1
#define MEM_FREE_PAGES  2
#define MEM_FORK_PROC   3
#define MEM_EXIT_PROC   4
#define MEM_WAIT_PROC   5
#define MEM_FREE_PROC   6
#define MEM_MEMALLOC    7
#define MEM_MEMFREE     8

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

static inline char *kmalloc(size_t size) {
    uint    task_mem = query_proc("TaskMM");
    message msg;
    msg.type     = MEM_MEMALLOC;
    msg.major    = size;
    msg.receiver = task_mem;
    send_msg(&msg);
    recv_msg(&msg, task_mem);
    return (char *)msg.major;
}

static inline void kfree(char *mem) {
    uint    task_mem = query_proc("TaskMM");
    message msg;
    msg.type     = MEM_MEMFREE;
    msg.major    = (uint)mem;
    msg.receiver = task_mem;
    send_msg(&msg);
}

static inline int mem_free_proc(process *proc) {
    uint    task_mem = query_proc("TaskMM");
    message msg;
    msg.type     = MEM_FREE_PROC;
    msg.major    = (uint)proc;
    msg.receiver = task_mem;
    send_msg(&msg);
    recv_msg(&msg, task_mem);
    return msg.major;
}

uint fork();
int  wait(uint *);
void exit(uint);

#endif // __MODULE_MEMORY__
