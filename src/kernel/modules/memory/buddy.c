#include "buddy.h"
#include "generic/typedefs.h"
#include "lib/stdlib.h"
#include "memory.h"
#include "paging.h"

#include "modules/tty.h"

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
    p->prev               = NULL;
    mem->free_list[order] = p;
    return p;
}

static inline int xor_buddy_map(struct memory_info *mem, char *p, uint order) {
    uint page_idx = ((uint)p - mem->memory_start) / PG_SIZE;
    xor_bit(mem->buddy_map[order], page_idx >> (order + 1), 1);
    return check_bit(mem->buddy_map[order], page_idx >> (order + 1));
}

static char *allocate_pages_of_power_2(struct memory_info *mem, uint order,
                                       uint attr) {
    if (order >= MAX_ORDER)
        return NULL;
    char *block = NULL;
    if (mem->free_count[order] == 0) {
        block = allocate_pages_of_power_2(mem, order + 1, 0);
        if (block == NULL)
            return NULL;
        attach_to_free_list(mem, (block_list *)block, order);
        mem->free_count[order]++;
        /* printf("= PUT %x into order %d free list =\n", block); */
        block += ((1 << order) * PG_SIZE);
        xor_buddy_map(mem, block, order); // higher half is returned
    } else {
        block =
            (char *)remove_from_free_list(mem, mem->free_list[order], order);
        mem->free_count[order]--;
        xor_buddy_map(mem, block, order);
    }
    if (block != NULL) {
        for (uint i = 0; i < (1 << order); i++) {
            struct page *pg = &mem->pages_info[GET_ID_BY_PAGE(
                mem, ((uint)block + i * PG_SIZE))];
            pg->type        = attr;
            pg->reference   = 1;
        }
    }
    return block;
}

static int free_pages_of_power_2(struct memory_info *mem, char *p, uint order) {
    if (p == NULL)
        return 1; // free a NULL block
    if (!((uint)p >= mem->memory_start && (uint)p <= mem->usable_end))
        return 2; // free a block not managed by us
    uint buddy_bit = xor_buddy_map(mem, p, order);
    if (buddy_bit == 0 && order + 1 < MAX_ORDER) {
        char *buddy               = NULL;
        uint  page_idx            = ((uint)p - mem->memory_start) / PG_SIZE;
        uint  buddy_even_page_idx = (page_idx >> (order + 1)) << (order + 1);
        if (page_idx == buddy_even_page_idx)
            buddy = p + (1 << order) * PG_SIZE;
        else
            buddy = p - (1 << order) * PG_SIZE;
        // printf("= REMOVE %x from order %d free list =\n", buddy, order);
        remove_from_free_list(mem, (block_list *)buddy, order);
        mem->free_count[order]--;
        if (buddy < p)
            free_pages_of_power_2(mem, buddy, order + 1);
        else
            free_pages_of_power_2(mem, p, order + 1);
    } else {
        attach_to_free_list(mem, (block_list *)p, order);
        clear_page_info(mem, p, 1 << order, PAGE_TYPE_USABLE | PAGE_TYPE_FREE);
        mem->free_count[order]++;
    }
    return 0;
}

void print_free_info(struct memory_info *mem) {
    printf("[MEM] free blocks count is\n[MEM] ");
    for (uint i = 0; i < MAX_ORDER; i++)
        printf("%04d, ", 1 << i);
    printf("\n[MEM] ");
    for (uint i = 0; i < MAX_ORDER; i++)
        printf("%04d, ", mem->free_count[i]);
    printf("\n[MEM ]");
    for (uint i = 0; i < MAX_ORDER; i++)
        printf("0x%x,", mem->free_list[i]);
    printf("\n");
}

char *page_alloc(struct memory_info *mem_info, uint pages, uint attr) {
    return allocate_pages_of_power_2(
        mem_info, trailing_zero(round_up_power_2(pages)), attr);
}
int page_free(struct memory_info *mem_info, char *p, uint pages) {
    return free_pages_of_power_2(mem_info, (char *)p,
                                 trailing_zero(round_up_power_2(pages)));
}
