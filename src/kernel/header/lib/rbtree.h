#ifndef __RBTREE_H__
#define __RBTREE_H__

#include "generic/typedefs.h"

struct _rb_node {
    uint key;
    uint parent_and_color;
#define RB_BLACK 0
#define RB_RED   1
    struct _rb_node *L;
    struct _rb_node *R;
} __attribute__((aligned(16)));

struct _rb_tree {
    struct _rb_node *root;
};
typedef struct _rb_node rb_node;
typedef struct _rb_tree rb_tree;

rb_node *rb_search(rb_node *x, uint key);
int      rb_insert(rb_tree *t, rb_node *n);
void     rb_remove(rb_tree *t, rb_node *n);
rb_node *rb_succ(rb_node *n);
rb_node *rb_pred(rb_node *n);

#endif // __RBTREE_H__
