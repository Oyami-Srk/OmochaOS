#ifndef __MOD_MEMORY_PAGING_INC__
#define __MOD_MEMORY_PAGING_INC__

#include "core/paging.h"
#include "lib/stdlib.h"
#include "memory.h"

// return NULL if cannot find.
static inline pte_t *get_pte(pde_t *page_dir, void *va) {
    pte_t *pg_tab = NULL;
    pde_t  pde    = page_dir[(uint)va >> 22];
    if (!(pde & PG_Present))
        return NULL;
    pg_tab = (pte_t *)(pde & ~0xFFF);
    return &pg_tab[((uint)va >> 12) & 0x3FF];
}

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

static inline int get_page_reference(struct memory_info *mem, char *page) {
    return mem->pages_info[GET_ID_BY_PAGE(mem, (uint)page)].reference;
}

// return a increased value
static inline int increase_page_ref(struct memory_info *mem, void *page) {
    return ++(mem->pages_info[GET_ID_BY_PAGE(mem, (uint)page)].reference);
}

// return a decreased value
static inline int decrease_page_ref(struct memory_info *mem, void *page) {
    return --(mem->pages_info[GET_ID_BY_PAGE(mem, (uint)page)].reference);
}

static inline int set_page_writable(pde_t *pg_dir, char *va, BOOL writable) {
    pte_t *pte = get_pte(pg_dir, va);
    if (pte && *pte & PG_Present) {
        if (writable)
            *pte |= PG_Writable;
        else
            *pte &= ~PG_Writable;
    }
    return writable;
}

int map_pages(struct memory_info *mem, pde_t *page_dir, void *va, void *pa,
              size_t size, uint pte_attr);
int unmap_pages(struct memory_info *mem, pde_t *page_dir, void *va,
                size_t size);

// create from sysmap
pde_t *create_page_dir(struct memory_info *mem);
pte_t *create_page_table(struct memory_info *mem, pde_t *page_dir, void *va,
                         uint pde_attr);

#endif // __MOD_MEMORY_PAGING_INC__