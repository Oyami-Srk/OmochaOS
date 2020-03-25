#include "core/memory.h"
#include "core/environment.h"
#include "core/multiboot.h"
#include "core/paging.h"
#include "generic/asm.h"
#include "generic/typedefs.h"
#include "lib/stdlib.h"
#include "lib/string.h"

#define PGROUNDUP(sz)  (((sz) + PG_SIZE - 1) & ~(PG_SIZE - 1))
#define PGROUNDDOWN(a) (((a)) & ~(PG_SIZE - 1))

list *freemem;

// free pages
void kfree(char *p) {
    list *current;
    // only free page higher then Kernel's end and lower than 0xE00000 in phy.
    // and p must be start of a page.
    if ((uint)p % PG_SIZE || (uint)p < (uint)KERN_VEND ||
        (uint)KV2P(p) >= 0xE00000) {
        magic_break();
        while (1)
            ;
    }
    memset(p, 0, PG_SIZE);

    // insert to head of freememlist
    current       = (list *)p;
    current->next = freemem;
    freemem       = current;
}

// allocate pages
char *kalloc(size_t pages) {
    list *current;
    current = freemem;
    if (current)
        freemem = current->next;
    return (char *)current;
}

// page_dir and page_table must be countinous
// page_table is what you want to start (not entire table)
// page_dir is start of entire page_dir
// ret: pointer to next free page table entity
unsigned int *core_map_memory(unsigned int *page_dir, unsigned int *page_table,
                              void *vaddr, void *paddr, size_t pages) {
    unsigned int *p = page_table;
    unsigned int *d = &page_dir[(uint)vaddr >> 22];
    for (; p < page_table + pages; p++)
        *p = ((uint)paddr & 0xFFFFF000) | PG_Present | PG_User | PG_Writeable;
    return page_table;
}

// init memory for very first time being
void core_init_memory(struct core_env *env) {
    multiboot_memory_map_t *mmap =
        KP2V((multiboot_memory_map_t *)env->boot_info.mmap_addr);
    uint i                = 0;
    uint upper_mem_length = 0;
    for (; (uint)mmap <
           (uint)KP2V(env->boot_info.mmap_addr) + env->boot_info.mmap_length;
         mmap = (multiboot_memory_map_t *)((uint)mmap + mmap->size +
                                           sizeof(mmap->size))) {
        env->memory_zone[i].addr   = mmap->addr_low;
        env->memory_zone[i].length = mmap->len_low;
        env->memory_zone[i].type   = mmap->type;
        i++;
        if (mmap->addr_low == 0x100000) // upper memory higher than 1MB
            upper_mem_length = mmap->len_low;
    }

    env->memory_zone_count = i;

    /* unsigned int *page_dir = env->page_dir; */
    /* if ((uint)page_dir & 0xFFF) */
    /* panic("Env's page dir is not aligned."); */
    /* if (0x100000 + upper_mem_length < (uint)KV2P(env->core_space_end)) */
    /* panic("Not enough memory! Must bigger than 4MB"); */
    /* size_t pg_count = upper_mem_length / PG_SIZE; */
    /* memset(page_dir, 0, sizeof(uint) * 1024); // 1024 * 4Byte = 4KB page dir
     */

    void *vstart = (void *)env->core_space_free_start;
    void *vend   = (void *)env->core_space_free_end;
    // align to higher addr than vstart
    void *p = (void *)((uint)(vstart + PG_SIZE - 1) & ~(PG_SIZE - 1));
    for (; p + PG_SIZE <= vend; p += PG_SIZE)
        kfree(p);
}
