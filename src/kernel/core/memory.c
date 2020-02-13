#include "core/memory.h"
#include "generic/asm.h"
#include "generic/typedefs.h"
#include "lib/string.h"

list *freemem;

// free a page
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

// allocate a page
char *kalloc(void) {
    list *current;
    current = freemem;
    if (current)
        freemem = current->next;
    return (char *)current;
}

// init memory for very first time being
void core_init_memory(void *vstart, void *vend) {
    // align to higher addr than vstart
    void *p = (void *)((uint)(vstart + PG_SIZE - 1) & ~(PG_SIZE - 1));
    for (; p + PG_SIZE <= vend; p += PG_SIZE)
        kfree(p);
}
