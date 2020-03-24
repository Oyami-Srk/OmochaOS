#include "lib/rbtree.h"
#include "lib/stdlib.h"
#include "lib/string.h"

#define GET_PARENT(n)                                                          \
    ((struct _rb_node *)(((struct _rb_node *)n)->parent_and_color &            \
                         0xFFFFFFFCL))
#define SET_PARENT(n, p)                                                       \
    do {                                                                       \
        ((struct _rb_node *)n)->parent_and_color &= 0x00000003;                \
        ((struct _rb_node *)n)->parent_and_color |= (unsigned int)p;           \
    } while (0)
#define GET_COLOR(n)                                                           \
    (unsigned int)(((struct _rb_node *)n)->parent_and_color & 0x00000003)
#define SET_COLOR(n, c)                                                        \
    do {                                                                       \
        ((struct _rb_node *)n)->parent_and_color &= 0xFFFFFFFC;                \
        ((struct _rb_node *)n)->parent_and_color |= (unsigned int)c;           \
    } while (0)

rb_node *rb_search(rb_node *x, uint key) {
    while (x != NULL) {
        if (x->key == key)
            break;
        else if (key < x->key)
            x = x->L;
        else
            x = x->R;
    }
    return x;
}

rb_node *rb_detach(rb_node *x) {
    rb_node *p = GET_PARENT(x);
    if (p) {
        if (p->L == x)
            p->L = NULL;
        else
            p->R = NULL;
    }
    SET_PARENT(x, NULL);
    return x;
}

rb_node *rb_transplant(rb_tree *rb_tree, rb_node *dst, rb_node *src) {
    if (rb_tree && rb_tree->root == dst)
        rb_tree->root = src;
    else if (dst == GET_PARENT(dst)->R)
        GET_PARENT(dst)->R = src;
    else if (dst == GET_PARENT(dst)->L)
        GET_PARENT(dst)->L = src;
    SET_PARENT(src, GET_PARENT(dst));
    return dst;
}

void rb_left_rotate(rb_tree *rb_tree, rb_node *x) {
    if (x->R == NULL)
        return;
    rb_node *a = rb_detach(x->R);
    rb_transplant(rb_tree, x, a);
    if (a->L) {
        rb_node *b = rb_detach(a->L);
        a->L       = x;
        SET_PARENT(x, a);
        x->R = b;
        SET_PARENT(b, x);
    } else {
        a->L = x;
        SET_PARENT(x, a);
    }
}

void rb_right_rotate(rb_tree *rb_tree, rb_node *x) {
    if (x->L == NULL)
        return;
    rb_node *a = rb_detach(x->L);
    rb_transplant(rb_tree, x, a);
    if (a->R) {
        rb_node *b = rb_detach(a->R);
        a->R       = x;
        SET_PARENT(x, a);
        x->L = b;
        SET_PARENT(b, x);
    } else {
        a->R = x;
        SET_PARENT(x, a);
    }
}

void rb_insert_fixup(rb_tree *t, rb_node *n) {
    if (GET_PARENT(n) == NULL) {
        SET_COLOR(n, RB_BLACK);
        return;
    }
    if ((GET_COLOR((GET_PARENT(n)))) == RB_BLACK) {
        ;
    } else {
        rb_node *parent = GET_PARENT(n);
        rb_node *grand  = GET_PARENT(parent);
        rb_node *uncle  = grand->R == parent ? grand->L : grand->R;
        if (uncle && GET_COLOR(uncle) == RB_RED) {
            SET_COLOR(parent, RB_BLACK);
            SET_COLOR(uncle, RB_BLACK);
            SET_COLOR(grand, RB_RED);
            rb_insert_fixup(t, grand);
        } else if (grand->L == parent) {
            if (parent->L == n) {
                SET_COLOR(parent, RB_BLACK);
                SET_COLOR(grand, RB_RED);
            } else {
                SET_COLOR(n, RB_BLACK);
                SET_COLOR(grand, RB_RED);
                rb_left_rotate(t, parent);
            }
            rb_right_rotate(t, grand);
        } else {
            if (parent->R == n) {
                SET_COLOR(parent, RB_BLACK);
                SET_COLOR(grand, RB_RED);
            } else {
                SET_COLOR(n, RB_BLACK);
                SET_COLOR(grand, RB_RED);
                rb_right_rotate(t, parent);
            }
            rb_left_rotate(t, grand);
        }
    }
}

void rb_insert(rb_tree *t, rb_node *n) {
    SET_COLOR(n, RB_RED);
    /* insert like BST */
    rb_node *x = t->root;
    if (x == NULL)
        t->root = n;
    else
        while (x != NULL) {
            if (x->key == n->key)
                return; // update value
            else if (x->key > n->key) {
                if (x->L == NULL) {
                    x->L = n;
                    break;
                } else
                    x = x->L;
            } else {
                if (x->R == NULL) {
                    x->R = n;
                    break;
                } else
                    x = x->R;
            }
        }
    SET_PARENT(n, x);
    /* END insert like BST */
    rb_insert_fixup(t, n);
    SET_COLOR(t->root, RB_BLACK); // root is always black
}

rb_node *rb_succ(rb_node *n) {
    if (n->R) {
        rb_node *x = n->R;
        while (x->L)
            x = x->L;
        return x;
    } else {
        rb_node *p = GET_PARENT(n);
        while (p && n == p->R) {
            n = p;
            p = GET_PARENT(p);
        }
        return p;
    }
}

rb_node *rb_pred(rb_node *n) {
    if (n->L) {
        rb_node *x = n->L;
        while (x->R)
            x = x->R;
        return x;
    } else {
        rb_node *p = GET_PARENT(n);
        while (p && n == p->L) {
            n = p;
            p = GET_PARENT(p);
        }
        return p;
    }
}

void rb_remove_fixup(rb_tree *t, rb_node *n, rb_node *parent) {
    rb_node *other;

    while ((!n || GET_COLOR(n) == RB_BLACK) && n != t->root) {
        if (parent->L == n) {
            other = parent->R;
            if (GET_COLOR(other) == RB_RED) {
                SET_COLOR(other, RB_BLACK);
                SET_COLOR(parent, RB_RED);
                rb_left_rotate(t, parent);
                other = parent->R;
            }
            if ((!other->L || GET_COLOR(other->L) == RB_BLACK) &&
                (!other->R || GET_COLOR(other->R) == RB_BLACK)) {
                SET_COLOR(other, RB_RED);
                n      = parent;
                parent = GET_PARENT(n);
            } else {
                if (!other->R || GET_COLOR(other->R) == RB_BLACK) {
                    SET_COLOR(other->L, RB_BLACK);
                    SET_COLOR(other, RB_RED);
                    rb_right_rotate(t, other);
                    other = parent->R;
                }
                SET_COLOR(other, GET_COLOR(parent));
                SET_COLOR(parent, RB_BLACK);
                SET_COLOR(other->R, RB_BLACK);
                rb_left_rotate(t, parent);
                n = t->root;
                break;
            }
        } else {
            other = parent->L;
            if (GET_COLOR(other) == RB_RED) {
                SET_COLOR(other, RB_BLACK);
                SET_COLOR(parent, RB_RED);
                rb_right_rotate(t, parent);
                other = parent->L;
            }
            if ((!other->L || GET_COLOR(other->L) == RB_BLACK) &&
                (!other->R || GET_COLOR(other->R) == RB_BLACK)) {
                SET_COLOR(other, RB_RED);
                n      = parent;
                parent = GET_PARENT(n);
            } else {
                if (!other->L || GET_COLOR(other->L) == RB_BLACK) {
                    SET_COLOR(other->R, RB_BLACK);
                    SET_COLOR(other, RB_RED);
                    rb_left_rotate(t, other);
                    other = parent->L;
                }
                SET_COLOR(other, GET_COLOR(parent));
                SET_COLOR(parent, RB_BLACK);
                SET_COLOR(other->L, RB_BLACK);
                rb_right_rotate(t, parent);
                n = t->root;
                break;
            }
        }
    }
    if (n)
        SET_COLOR(n, RB_BLACK);
}

void rb_remove(rb_tree *t, rb_node *n) {
    rb_node *r = n;
    rb_node *child;
    int      color = GET_COLOR(n);

    if (n->L == NULL) {
        child = n->R;
        rb_transplant(t, n, child);
    } else if (n->R == NULL) {
        child = n->L;
        rb_transplant(t, n, child);
    } else {
        r     = rb_succ(n);
        color = GET_COLOR(r);
        child = r->R;
        if (GET_PARENT(r) == n) { // replace is node->right
            if (child)
                SET_PARENT(child, r);
        } else { // replace is node->right->left(most)
            rb_transplant(t, r, r->R);
            r->R = n->R;
            SET_PARENT(r->R, r);
        }
        rb_transplant(t, n, r);
        r->L = n->L;
        SET_PARENT(r->L, r);
        SET_COLOR(r, GET_COLOR(n));
    }
    if (color == RB_BLACK)
        rb_remove_fixup(t, child, r);
}
