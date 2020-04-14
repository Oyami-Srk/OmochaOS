#include "paging.h"
#include "buddy.h"
#include "core/memory.h"
#include "core/paging.h"
#include "lib/stdlib.h"
#include "memory.h"

#include "driver/graphic.h"

// sys_mapping pa should not contained in free memory
unsigned int sys_mapping[][3] = {
    // paddr, vaddr, page size
    /* {0, 0, PG_SIZE * 1024}, */
    {0, KERN_BASE, PG_SIZE * 1024}};

pte_t *create_page_table(struct memory_info *mem, pde_t *page_dir, void *va,
                         uint pde_attr) {
    pte_t *pte = get_pte(page_dir, va);
    pde_t *pde = &page_dir[(uint)va >> 22];
    if (pte) {
        // mem->pages_info[GET_ID_BY_PAGE(mem, (uint)(*pde &
        // ~0xFFF))].reference++;
        return pte;
    }
    pte_t *pg_tab =
        (pte_t *)page_alloc(mem, 1, PAGE_TYPE_INUSE | PAGE_TYPE_PGTBL);
    if (pg_tab == NULL)
        return NULL;
    // if reference not 1, it definitly wrong
    // mem->pages_info[GET_ID_BY_PAGE(mem, (uint)pg_tab)].reference = 1;
    memset(pg_tab, 0, PG_SIZE);
    // memory proc cr3 is directly mapped, no need for V2P
    *pde = (uint)pg_tab | PG_Present | pde_attr;
    return &pg_tab[((uint)va >> 12) & 0x3FF];
}

// size in bytes
int map_pages(struct memory_info *mem, pde_t *page_dir, void *va, void *pa,
              size_t size, uint pte_attr) {
    va              = (void *)PGROUNDDOWN((uint)va);
    void * end      = (void *)PGROUNDDOWN(((uint)va) + size - 1);
    pte_t *pte      = NULL;
    uint   pde_attr = PG_Present | PG_User | PG_Writable;
    if (pte_attr & PG_OS_SYS)
        pde_attr = PG_Present | PG_OS_SYS;
    /* printf("Map %x-%x to %x\n", va, end, pa); */
    for (; (uint)va <= (uint)end; va += PG_SIZE, pa += PG_SIZE) {
        if ((pte = create_page_table(mem, page_dir, va, pde_attr)) == NULL)
            return -1;
        if (!(*pte & PG_Present)) {
            *pte = (uint)pa | PG_Present | pte_attr;
            if ((uint)pa < mem->memory_start || (uint)pa > mem->usable_end)
                continue; // not set page info for memory not managed by us
        }
        // mem->pages_info[GET_ID_BY_PAGE(mem, (uint)pa)].reference++;
    }
    return 0;
}

// size in bytes
int unmap_pages(struct memory_info *mem, pde_t *page_dir, void *va,
                size_t size) {
    va                   = (void *)PGROUNDDOWN((uint)va);
    void * end           = (void *)PGROUNDDOWN(((uint)va) + size - 1);
    pte_t *pte           = NULL;
    uint   contiunos_pte = 0;
    for (; (uint)va <= (uint)end; va += PG_SIZE) {
        pte = get_pte(page_dir, va);
        if (!(*pte & PG_Present)) {
            panic("memory not map");
            return 1;
        }

        char *pa  = (char *)((*pte & ~0xFFF) + ((unsigned int)va & 0xFFF));
        uint  ref = mem->pages_info[GET_ID_BY_PAGE(mem, (uint)pa)].reference--;
        if (ref == 1) {
            page_free(mem, pa, 1);
            kprintf("Free 1 page.");
        }

        *pte = 0;
        /* // unmap does need to free pagetab
        if (((uint)va & 0x3FF000) == 0)
            contiunos_pte = 0;
        contiunos_pte++;
        if (contiunos_pte == PTE_PER_PDE) {
            // free 1024 pte entities, then free the page dir
            pde_t *pde = &page_dir[(uint)va >> 22];
            if (!(*pde & PG_Present))
                panic("page dir entity not mapped wtf?");
            pte_t *pg_tab = (pte_t *)((*pde) & ~0xFFF);
            if (--(mem->pages_info[GET_ID_BY_PAGE(mem, (uint)pg_tab)]
                       .reference) == 0)
                page_free(mem, (char *)pg_tab, 1);
            contiunos_pte = 0;
        }
        */
    }

    return 0;
}

pde_t *create_page_dir(struct memory_info *mem) {
    pde_t *page_dir =
        (pde_t *)page_alloc(mem, 1, PAGE_TYPE_INUSE | PAGE_TYPE_PGTBL);
    if (page_dir == NULL)
        return NULL;
    memset(page_dir, 0, PG_SIZE);
    size_t sys_map_size = sizeof(sys_mapping) / sizeof(sys_mapping[0]);
    // sysmap is not writeable for users
    for (uint i = 0; i < sys_map_size; i++) {
        char * va   = (char *)sys_mapping[i][1];
        char * pa   = (char *)sys_mapping[i][0];
        size_t size = sys_mapping[i][2];
        if ((((uint)va & 0x3FFFFF) == 0) &&
            size % (PG_SIZE * PTE_PER_PDE) == 0) {
            // could use extened page size
            char *end = va + size;
            for (; va < end;
                 va += PG_SIZE * PTE_PER_PDE, pa += PG_SIZE * PTE_PER_PDE)
                page_dir[(uint)va >> 22] =
                    ((uint)pa) | PG_Present | PG_OS_SYS | PG_PS;

        } else {
            map_pages(mem, page_dir, va, pa, size, PG_Present | PG_OS_SYS);
        }
    }
    /* page_dir[KERN_BASE >> 22] = 0 | PG_Present | PG_PS; */
    return page_dir;
}
