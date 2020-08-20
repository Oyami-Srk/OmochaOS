#ifndef __MOD_MEMORY_BUDDY_INC__
#define __MOD_MEMORY_BUDDY_INC__

#include <memory.h>

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

char *page_alloc(struct memory_info *mem_info, uint pages, uint attr);
int   page_free(struct memory_info *mem_info, char *p, uint pages);

#endif // __MOD_MEMORY_BUDDY_INC__