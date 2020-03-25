#include "core/memory.h"
#include "core/environment.h"
#include "core/multiboot.h"
#include "generic/asm.h"
#include "generic/typedefs.h"
#include "lib/string.h"

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

// init memory for very first time being
void core_init_memory(struct core_env *env) {
    multiboot_memory_map_t *mmap =
        KP2V((multiboot_memory_map_t *)env->boot_info.mmap_addr);
    for (uint i = 0; (uint)mmap < (uint)KP2V(env->boot_info.mmap_addr) +
                                      env->boot_info.mmap_length;
         mmap = (multiboot_memory_map_t *)((uint)mmap + mmap->size +
                                           sizeof(mmap->size))) {
        env->memory_zone[i].addr   = mmap->addr_low;
        env->memory_zone[i].length = mmap->len_low;
        env->memory_zone[i].type   = mmap->type;
        i++;
    }
    void *vstart = (void *)env->core_space_start;
    void *vend   = (void *)env->core_space_end;
    // align to higher addr than vstart
    void *p = (void *)((uint)(vstart + PG_SIZE - 1) & ~(PG_SIZE - 1));
    for (; p + PG_SIZE <= vend; p += PG_SIZE)
        kfree(p);
}
