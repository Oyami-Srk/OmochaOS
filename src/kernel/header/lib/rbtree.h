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

#endif // __RBTREE_H__
