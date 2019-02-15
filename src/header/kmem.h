#ifndef __OMOCHA_OS__KMEM_H__
#define __OMOCHA_OS__KMEM_H__

#include "type.h"
#include "const.h"

struct __list {
  struct __list *next;
};

typedef struct __list list;

void kinit_mem(void *vsatrt, void *vend);
void kfree(void *v);
char *kalloc(void);

#endif
