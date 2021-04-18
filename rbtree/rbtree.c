#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include "rbtree.h"

void fix_violation(struct Tree *tree, struct Node *z);

void rotate_left(struct Tree *tree, struct Node *x);

void rotate_right(struct Tree *tree, struct Node *x);

bool is_rbnil(struct Node *node) {
    return node == &NODENIL;
}

struct Node *init_node(void *node_place, size_t value) {\
    *(struct Node *) node_place = (struct Node) {
            .color = BLACK,
            .left = &NODENIL,
            .right = &NODENIL,
            .parent = &NODENIL,
            .next = NULL,
            .prev = NULL,
            .value = value,
    };
    return node_place;
}

void insert_item(struct Tree *tree, struct Node *z) {
    if (tree->root == NULL) {
        tree->root = z;
        z->color = BLACK;
        return;
    }
    struct Node *y = &NODENIL;
    struct Node *x = tree->root;
    z->color = RED;
    while (!is_rbnil(x)) {
        y = x;

        if (z->value < x->value) {
            x = x->left;
        } else if (z->value > x->value) {
            x = x->right;
        } else {
            if (x->next != NULL) {
                z->next = x->next;
                z->next->prev = z;
            }
            x->next = z;
            z->prev = x;
            return;
        }
    }

    z->parent = y;
    if (is_rbnil(y)) {
        tree->root = z;
    } else if (z->value < y->value) {
        y->left = z;
    } else {
        y->right = z;
    }

    // RB Insert Fixup
    fix_violation(tree, z);
}

void transplant(struct Tree *tree, struct Node *u, struct Node *v) {
    if (is_rbnil(u->parent)) {
        tree->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if (!is_rbnil(v)) {
        v->parent = u->parent;
    }
}

void remove_fixup(struct Tree *tree, struct Node *x) {
    struct Node *w;
    while (x != tree->root && x->color == BLACK) {
        if (x == x->parent->left) {
            w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotate_left(tree, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                x = x->parent;
                if (!is_rbnil(w)) {
                    w->color = RED;
                } else {
                    break;
                }
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rotate_right(tree, w);
                    w = w->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                rotate_left(tree, x->parent);
                x = tree->root;
            }
        } else {
            w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotate_right(tree, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                x = x->parent;
                if (!is_rbnil(w)) {
                    w->color = RED;
                } else {
                    break;
                }
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    rotate_left(tree, w);
                    w = w->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rotate_right(tree, x->parent);
                x = tree->root;
            }
        }
    }
    x->color = BLACK;
}

struct Node *tree_minimum(struct Node *z) {
    for (; !is_rbnil(z->left);
           z = z->left);
    return z;
}

void remove_item(struct Tree *tree, struct Node *node) {
    if (node->prev) {
        node->prev->next = node->next;
        if (node->next) { node->next->prev = node->prev; }
        return;

    } else if (node->next) {
        struct Node *next = node->next;
        next->color = node->color;
        next->left = node->left;
        if (!is_rbnil(next->left)) { next->left->parent = next; }
        next->right = node->right;

        if (!is_rbnil(next->right)) { next->right->parent = next; }
        next->parent = node->parent;
        next->prev = NULL;

        if (node == tree->root) {
            if (!is_rbnil(node->left)) { node->left->parent = next; }
            if (!is_rbnil(node->right)) { node->right->parent = next; }
            tree->root = next;
        } else if (!is_rbnil(node->parent)) {
            if (node->parent->left == node) { node->parent->left = next; }
            else { node->parent->right = next; }
        }
        return;
    }

    struct Node *y, *z, *x;
    uint8_t y_original_color;

    y = z = node;
    y_original_color = y->color;
    if (is_rbnil(z->left)) {
        x = z->right;
        transplant(tree, z, z->right);
    } else if (is_rbnil(z->right)) {
        x = z->left;
        transplant(tree, z, z->left);
    } else {
        y = tree_minimum(z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) { x->parent = y; }
        else {
            transplant(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(tree, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    if (y_original_color == BLACK) {
        remove_fixup(tree, x);
    }

    if (is_rbnil(x)) {
        x->parent = x;
        x->color = BLACK;
    }
}

void fix_violation(struct Tree *tree, struct Node *z) {
    struct Node *y;

    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            y = z->parent->parent->right;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    rotate_left(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotate_right(tree, z->parent->parent);
            }
        } else {
            y = z->parent->parent->left; // uncle
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rotate_right(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotate_left(tree, z->parent->parent);
            }
        }
    }
    tree->root->color = BLACK;
}

void rotate_left(struct Tree *tree, struct Node *x) {
    struct Node *y = x->right;
    x->right = y->left;
    if (!is_rbnil(y->left)) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (is_rbnil(x->parent)) {
        tree->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void rotate_right(struct Tree *tree, struct Node *x) {
    struct Node *y = x->left;
    x->left = y->right;
    if (!is_rbnil(y->right)) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (is_rbnil(x->parent)) {
        tree->root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }
    if (!is_rbnil(y)) {
        y->right = x;
    }
    x->parent = y;
}

struct Node *search(struct Tree *tree, size_t value) {
    struct Node *z = tree->root;

    while (!is_rbnil(z)) {
        if (z->value == value) {
            return z;
        }

        if (z->value < value) {
            z = z->right;
        } else {
            z = z->left;
        }
    }

    return NULL;
}

struct Node *search_suitable(struct Tree *tree, size_t value) {
    if (tree->root == NULL) {
        return NULL;
    }
    struct Node *z = tree->root;
    struct Node *res = NULL;

    while (!is_rbnil(z))
    {
        if (z->value == value)
            return z;

        if (z->value < value)
            z = z->right;
        else
        {
            res = z;
            z = z->left;
        }
    }

    return res;
}

void print_tree(struct Tree *tree) {
    struct Node *nodes[500] = {NULL};
    struct Node *nodes_next_level[500] = {NULL};
    nodes[0] = tree->root;
    int count_on_level = 1;
    printf("Tree:\n");
    while (count_on_level != 0) {
        int count_on_next_level = 0;
        for (int i = 0;
             i < count_on_level;
             i++) {
            if (nodes[i] && !is_rbnil(nodes[i])) {
                printf("%zu (%s), ", nodes[i]->value, nodes[i]->color ? "black" : "red");
                if (!is_rbnil(nodes[i]->left)) {
                    nodes_next_level[count_on_next_level] = nodes[i]->left;
                    count_on_next_level++;
                }
                if (!is_rbnil(nodes[i]->right)) {
                    nodes_next_level[count_on_next_level] = nodes[i]->right;
                    count_on_next_level++;
                }
            }
        }
        count_on_level = count_on_next_level;
        for (int i = 0;
             i < count_on_next_level;
             i++)
            nodes[i] = nodes_next_level[i];
        printf("\n");
    }

    printf("Tree END\n");
}