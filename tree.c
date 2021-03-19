//
// Created by Tarikan on 12.03.2021.
//

#include "tree.h"



static int int_cmp(const void *ai, const void *bi) {

}

/*
// Some black magic
// get header from node
struct Header* get_header(struct node *node) {
    return (struct Header*)((char*)node - offsetof(struct Header, node));
}

size_t get_header_size_from_node(struct node* node) {
    return (size_t*)((char*)node - (offsetof(struct Header, node) - offsetof(struct Header, size)));
}
 */