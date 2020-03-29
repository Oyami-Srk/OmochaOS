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

static inline uint round_down_power_2(uint x) {
    if (x == 0)
        return 0;
    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);
    x |= (x >> 16);
    return x - (x >> 1);
}

static inline uint round_up_power_2(uint x) {
    x--;
    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);
    x |= (x >> 16);
    return x + 1;
}

static inline uint trailing_zero(uint x) {
    uint c;
    asm volatile("bsfl %0, %0" : "=a"(c) : "0"(x));
    return c;
}

static block_list *remove_from_free_list(struct memory_info *mem, block_list *p,
                                         uint order) {
    if (p->prev) {
        p->prev->next = p->next;
        p->next->prev = p->prev;
    } else {
        if (mem->free_list[order] != p)
            panic("Block has no prev but not the first.");
        mem->free_list[order] = p->next;
        p->next->prev         = NULL;
    }
    p->next = NULL;
    p->prev = NULL;
    return p;
}

static block_list *attach_to_free_list(struct memory_info *mem, block_list *p,
                                       uint order) {
    // must attach a single node
    assert(p->prev == NULL);
    assert(p->next == NULL);
    p->next               = mem->free_list[order];
    p->next->prev         = p;
    mem->free_list[order] = p;
    return p;
}

static char *allocate_pages(struct memory_info *mem, size_t pages) {
    if (pages == 0)
        return NULL;
    uint   require_block_size = round_up_power_2(pages);
    uint   require_order      = trailing_zero(require_block_size);
    char * free_block         = NULL;
    size_t free_block_size    = 0; // in pages
    char * target_block       = NULL;
    if (mem->free_count[require_order] == 0) {
        for (uint o = require_order + 1; o < MAX_ORDER; o++)
            if (mem->free_count[o] != 0) {
                free_block =
                    (char *)remove_from_free_list(mem, mem->free_list[o], o);
                free_block_size = 1 << o;
                mem->free_count[o]--;
                uint free_page_idx =
                    ((uint)free_block - mem->memory_start) / PG_SIZE;
                xor_bit(mem->buddy_map[o], free_page_idx >> (o + 1), 1);
                break;
            }
        return NULL;
    } else {
        free_block = (char *)remove_from_free_list(
            mem, mem->free_list[require_order], require_order);
        free_block_size = 1 << require_order;
        mem->free_count[require_order]--;
        uint free_page_idx = ((uint)free_block - mem->memory_start) / PG_SIZE;
        xor_bit(mem->buddy_map[require_order],
                free_page_idx >> (require_order + 1), 1);
    }

    target_block    = free_block + (free_block_size - pages) * PG_SIZE;
    uint rest_pages = free_block_size - pages;
    for (uint s = rest_pages; s > 0; s -= round_down_power_2(s)) {
        block_list *current            = (block_list *)free_block;
        uint        current_block_size = round_down_power_2(s);
        uint        order              = trailing_zero(current_block_size);
        current->prev                  = NULL;
        if (mem->free_list[order]) {
            block_list *next = mem->free_list[order];
            current->next    = next;
            next->prev       = current;
        } else
            current->next = NULL;
        mem->free_list[order] = current;
        mem->free_count[order]++;
        free_block += (1 << order) * PG_SIZE;
    }
    return target_block;
}

void print_free_info(struct memory_info *mem) {
    printf("[MEM] free block is\n[MEM] ");
    for (uint i = 0; i < MAX_ORDER; i++)
        printf("%04d, ", 1 << i);
    printf("\n[MEM] ");
    for (uint i = 0; i < MAX_ORDER; i++)
        printf("%04d, ", mem->free_count[i]);
    printf("\n");
}

// page_id = (page_paddr - pstart) / PG_SIZE
// pg_count = (end-start) / PG_SIZE
// buddy bits = (pg_count / (2^order)) / 2 = page_count >> (order+1)
// total buddy bits = ((2^MAX_ORDER)-1)*pg_count/(2^MAX_ORDER)

void init_memory(struct memory_info *mem) {
    printf("[MEM] Init memory 0x%x - 0x%x\n", mem->memory_start,
           mem->memory_end);
    size_t pg_count = (mem->memory_end - mem->memory_start) / PG_SIZE;
    uint   buddy_total_bits =
        ((1 << (MAX_ORDER)) - 1) * pg_count / (1 << MAX_ORDER);
    size_t buddy_bytes = buddy_total_bits / 8;
    mem->usable_end    = PGROUNDDOWN(mem->memory_end - buddy_bytes);
    mem->buddy_map[0]  = (bitset *)(mem->memory_end - buddy_bytes);
    mem->free_list[0]  = (block_list *)NULL;
    mem->free_count[0] = 0;
    printf("[MEM] Init buddy map on 0x%x, size %d bytes.\n", mem->usable_end,
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
    pg -= max_block_size;
    // split reset memory into other size block
    uint rest_pages = (mem->usable_end - (uint)pg) / PG_SIZE;
    for (uint s = rest_pages; s > 0;
         s -= round_down_power_2(s)) { // actually those two loops can be merge
                                       // into one, but I'm lazy to do so
        block_list *current            = (block_list *)pg;
        uint        current_block_size = round_down_power_2(s);
        uint        order              = trailing_zero(current_block_size);
        current->prev                  = NULL;
        if (mem->free_list[order]) {
            block_list *next = mem->free_list[order];
            current->next    = next;
            next->prev       = current;
        } else
            current->next = NULL;
        mem->free_list[order] = current;
        mem->free_count[order]++;
        pg += (1 << (order)) * PG_SIZE;
    }
    printf("[MEM] Initialized.\n");
    print_free_info(mem);
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

    char *test = allocate_pages(&mem_info, 89);
    printf("Allocated 89 pages at 0x%x\n", test);
    print_free_info(&mem_info);

    while (1)
        ;
}
