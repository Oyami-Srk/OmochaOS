/*
module:
  name: Memory
  author: Shiroko
  summary: Memory Process
  entry: Task_Memory
*/
#include "core/memory.h"
#include "core/environment.h"
#include "lib/bitset.h"
#include "lib/string.h"
#include "lib/syscall.h"
#include "modules/systask.h"
#include "modules/tty.h"

#define PAGE_TYPE_USABLE   0x01
#define PAGE_TYPE_RESERVED 0x02
#define PAGE_TYPE_SYSTEM   0x04
#define PAGE_TYPE_HARDWARE 0x08
#define PAGE_TYPE_FREE     0x10
#define PAGE_TYPE_INUSE    0x20

struct page {
    unsigned int type;
    int          reference;
    void *       vaddr;
};

struct _block_list {
    struct _block_list *prev;
    struct _block_list *next;
};

typedef struct _block_list block_list;

struct memory_info {
#define MAX_ORDER 11
    block_list *free_list[MAX_ORDER]; // max block size is 2^(MAX_ORDER-1)=4MB
    size_t      free_count[MAX_ORDER];
    bitset *    buddy_map[MAX_ORDER];
    uint        memory_start;
    uint        memory_end;
    uint        usable_end; // buddy map located on the end of memory
};

struct memory_info mem_info;

// return 1 means need to merge
static inline int attach_block_to_free(struct memory_info *mem,
                                       void *block_addr, uint order) {
    ;
    ;
    return 0;
}

// page_id = (page_paddr - pstart) / PG_SIZE
// pg_count = (end-start) / PG_SIZE
// buddy bits = (pg_count / (2^order)) / 2 = page_count >> (order+1)
// total buddy bits = ((2^MAX_ORDER)-1)*pg_count/(2^MAX_ORDER)

void init_memory(struct memory_info *mem) {
    printf("Init memory 0x%x - 0x%x\n", mem->memory_start, mem->memory_end);
    size_t pg_count = (mem->memory_end - mem->memory_start) / PG_SIZE;
    uint   buddy_total_bits =
        ((1 << (MAX_ORDER)) - 1) * pg_count / (1 << MAX_ORDER);
    size_t buddy_bytes = buddy_total_bits / 8;
    mem->usable_end    = PGROUNDDOWN(mem->memory_end - buddy_bytes);
    mem->buddy_map[0]  = (bitset *)(mem->memory_end - buddy_bytes);
    mem->free_list[0]  = (block_list *)NULL;
    mem->free_count[0] = 0;
    printf("Init buddy map on 0x%x, size %d(0x%x) bytes.\n", mem->usable_end,
           mem->memory_end - mem->usable_end);
    memset((void *)mem->usable_end, 0, mem->memory_end - mem->usable_end);
    for (uint i = 0; i < MAX_ORDER - 1; i++) {
        mem->buddy_map[i + 1] =
            (bitset *)(((uint)mem->buddy_map[i]) + (pg_count >> (i + 1)) / 8);
        mem->free_list[i + 1]  = (block_list *)NULL;
        mem->free_count[i + 1] = 0;
    }
    uint  max_block_size = (1 << (MAX_ORDER - 1)) * PG_SIZE;
    void *pg             = NULL;
    for (pg = (void *)mem->memory_start; pg < (void *)mem->usable_end;
         pg += max_block_size) {
        // free max block first
        block_list *current = (block_list *)pg;
        current->prev       = NULL;
        if (mem->free_list[MAX_ORDER - 1]) {
            block_list *next = mem->free_list[MAX_ORDER - 1];
            current->next    = next;
            next->prev       = current;
        } else
            current->next = NULL;
        mem->free_list[MAX_ORDER - 1] = current;
        mem->free_count[MAX_ORDER - 1]++;
    }
    // split reset memory into other size block
    uint rest_pages = (mem->usable_end - (uint)pg) / PG_SIZE;
    printf("Rest of pages is %d\n", rest_pages);
    return;
}

void Task_Memory(void) {
    struct core_env_memory_zone zone[10];
    size_t                   zone_count = query_env(ENV_KEY_MMAP, (ubyte *)zone,
                                  sizeof(struct core_env_memory_zone) * 10);
    struct core_memory_usage core_usage;
    query_env(ENV_KEY_MEMORY_USAGE, (ubyte *)&core_usage, sizeof(core_usage));

    /* struct page *pages_info = (struct page *)core_usage.core_space_end; */
    delay_ms(2000);

    mem_info.memory_start = (uint)KV2P(core_usage.core_space_end);
    mem_info.memory_end   = core_usage.memory_end;

    init_memory(&mem_info);

    while (1)
        ;
}
