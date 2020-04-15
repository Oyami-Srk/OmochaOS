#ifndef __MODULE_MEMORY_MEMORY_INC__
#define __MODULE_MEMORY_MEMORY_INC__

#include "generic/typedefs.h"
#include "lib/bitset.h"
#include "modules/memory.h"

#define PAGE_TYPE_USABLE   0x001
#define PAGE_TYPE_RESERVED 0x002
#define PAGE_TYPE_SYSTEM   0x004
#define PAGE_TYPE_HARDWARE 0x008
#define PAGE_TYPE_FREE     0x010
#define PAGE_TYPE_INUSE    0x020
#define PAGE_TYPE_PGTBL    0x040
#define PAGE_TYPE_USER     0x080
#define PAGE_TYPE_POOL     0x100

struct page {
    unsigned int type;
    int          reference;
};

struct _block_list {
    struct _block_list *prev;
    struct _block_list *next;
};

typedef struct _block_list block_list;

struct memory_info {
#define MAX_ORDER 11
    block_list * free_list[MAX_ORDER]; // max block size is 2^(MAX_ORDER-1)=4MB
    size_t       free_count[MAX_ORDER];
    bitset *     buddy_map[MAX_ORDER];
    uint         memory_start;
    uint         memory_end;
    uint         usable_end; // buddy map located on the end of memory
    uint         page_count; // only memory_start to memory_end
    struct page *pages_info;
};

#define GET_PAGE_BY_ID(mem, id)                                                \
    ((char *)(((mem)->memory_start + ((id)*PG_SIZE))))
#define GET_ID_BY_PAGE(mem, page)                                              \
    (((uint)page - ((mem)->memory_start)) / PG_SIZE)

uint     fork_proc(struct memory_info *mem, pid_t pid);         // fork.c
void     wait_proc(pid_t parent_pid);                           // exit.c
process *set_proc_exit(pid_t pid, uint exit_status);            // exit.c
process *free_proc(struct memory_info *mem, process *proc);     // exit.c
void     mem_exit_proc(process *proc);                          // exit.c
void     start_up_init(struct memory_info *mem);                // memory_init.c
void     init_memory(struct memory_info *mem);                  // memory_init.c
void do_page_fault(struct memory_info *mem, stack_frame *intf); // pagefault.c

void  mem_kfree(char *p);                                          // kmem.c
char *mem_kmalloc(size_t size);                                    // kmem.c
void  init_memory_pool(struct memory_info *mem, size_t pool_size); // kmem.c

#endif // __MODULE_MEMORY_MEMORY_INC__