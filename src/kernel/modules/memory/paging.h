#ifndef __MOD_MEMORY_PAGING_INC__
#define __MOD_MEMORY_PAGING_INC__

#include "memory.h"

static inline void set_page_attr(struct memory_info *mem, char *page,
                                 size_t pg_count, uint attr) {
    for (uint i = 0; i < pg_count; i++)
        mem->pages_info[GET_ID_BY_PAGE(mem, ((uint)page + i * PG_SIZE))].type =
            attr;
}

static inline void clear_page_info(struct memory_info *mem, char *page,
                                   size_t pg_count, uint attr) {
    for (uint i = 0; i < pg_count; i++) {
        struct page *p =
            &mem->pages_info[GET_ID_BY_PAGE(mem, ((uint)page + i * PG_SIZE))];
        p->type      = attr;
        p->reference = 0;
    }
}

int map_pages(struct memory_info *mem, pde_t *page_dir, void *va, void *pa,
              size_t size, uint pte_attr);
int unmap_pages(struct memory_info *mem, pde_t *page_dir, void *va,
                size_t size);

// create from sysmap
pde_t *create_page_dir(struct memory_info *mem);
// create a copy of parent, and set both of them read-only
// but references need increase by map_pages
pde_t *copy_page_dir(struct memory_info *mem, pde_t *parent);
void   delete_page_dir(struct memory_info *mem, pde_t *pg_dir);

#endif // __MOD_MEMORY_PAGING_INC__