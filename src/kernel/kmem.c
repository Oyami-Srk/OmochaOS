#include "kernel/kmem.h"
#include "kernel/memlayout.h"
#include "string.h"

list *kmem_freelist;

// Initialize memblock
// write list item to range of free memory page
void kinit_mem(void *vstart, void *vend) {
  void* p = (void*)((unsigned int)(vstart + PG_SIZE - 1) & ~(PG_SIZE - 1)); // 4K align to higher addr than vstart
  for(; p + PG_SIZE <= vend; p += PG_SIZE)
    kfree(p);
}

void kfree(void *v){
  list *cur;
  if((uint)v % PG_SIZE || (uint)v < KERN_END || (uint)V2P(v) >= 0xE00000)
    while(1); // panic
  memset(v, 0, PG_SIZE);
  cur = (list*)v;
  cur->next = kmem_freelist;
  kmem_freelist = cur;
}

char *kalloc(void) {
  list *cur;
  cur = kmem_freelist;
  if(cur)
    kmem_freelist = cur->next;
  return (char*)cur;
}
