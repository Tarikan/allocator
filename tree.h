//
// Created by Tarikan on 12.03.2021.
//

#ifndef LAB1_TREE_H
#define LAB1_TREE_H

#include <stddef.h>
#include "header.h"

//typedef struct Header node;

struct node {
    size_t n;
    unsigned char mark;
    struct node *L, *R;
};

static struct node nil = {-1, 0, NULL, NULL};

static int int_cmp(const void *ai, const void *bi);

void tree_insert(struct node *root, struct node *target, bool checked);

void tree_remove(struct node *root, struct node *target);

struct node *tree_find_by_value(struct node *root, size_t size);

struct node *tree_find_by_ptr(struct node *root, struct node *target);

struct node *find_last_in_queue(struct node *target);

struct Header* get_header(struct node *node);

size_t get_header_size_from_node(struct node* node);

#endif //LAB1_TREE_H
