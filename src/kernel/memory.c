#include "kernel/memory.h"
#include "kernel/type.h"
#include "lib/string.h"

list *freemem;

void kinit_mem(void *vstart, void *vend, void *tss, void *ldt) {
  void *p = (void *)((uint)(vstart + PG_SIZE - 1) &
                     ~(PG_SIZE - 1)); // align to higher addr than vstart
  for (; p + PG_SIZE <= vend; p += PG_SIZE)
    kfree(p);
}

void kfree(void *v) {
  list *cur;
  if ((uint)v % PG_SIZE || (uint)v < (uint)KERN_END ||
      (uint)KV2P(v) >= 0xE00000)
    while (1)
      ; // panic
  memset(v, 0, PG_SIZE);
  cur = (list *)v;
  cur->next = freemem;
  freemem = cur;
}

char *kalloc(void) {
  list *cur;
  cur = freemem;
  if (cur)
    freemem = cur->next;
  return (char *)cur;
}