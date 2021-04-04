#ifndef __RBTREE_H__
#define __RBTREE_H__

#define RED 1
#define BLACK 2

#include <stdint.h>

struct Node {
    uint8_t color;

    struct Node *parent;
    struct Node *left;
    struct Node *right;
    struct Node *next;
    struct Node *prev;
    int value;
};

static struct Node NODENIL = {
        .color = BLACK,
        .parent = &NODENIL,
        .left = &NODENIL,
        .right = &NODENIL,
        .prev = NULL,
        .next = NULL,
        .value = 0,
};

struct Tree {
    struct Node *root;
};

struct Node *init_node(void *node_place, size_t value);

void insert_item(struct Tree *tree, struct Node *z);

void remove_item(struct Tree *tree, struct Node *node);

struct Node *search_suitable(struct Tree *tree, size_t value);

struct Node *search(struct Tree *tree, size_t value);

void print_tree(struct Tree *tree);

void print_node(struct Node *node);

#endif